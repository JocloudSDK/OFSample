#include "effectmanager.h"
#include "easy.h"
#include "pathutils.h"
#include "fileutils.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

#include <stdio.h>
#include <mutex>
#include <io.h>
#include <iostream>
#include <cstring>
#include "zlib.h"

using namespace rapidjson;
using namespace std;

std::recursive_mutex g_effectMutex;
struct MemoryStruct
{
    char *memory;
    size_t size;
    MemoryStruct()
    {
        memory = (char *)malloc(1);
        size = 0;
    }
    ~MemoryStruct()
    {
        free(memory);
        memory = NULL;
        size = 0;
    }
};

static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	//test unzip
	const char* gzfname = ".tempOfcam";
	FILE *f = fopen(gzfname, "wb+");//gz
	if (f)
	{
		fwrite(ptr, size, nmemb, f);
		fflush(f);
		fclose(f);
	}
	// TO:直接解压文件流
	std::string strdata = "";
	if (!gzLoadFile(gzfname, strdata)) 
	{
		//faile to unzip
		return 0;
	}

    size_t realsize = strdata.length();
    struct MemoryStruct *mem = (struct MemoryStruct *)data;

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);

    if (mem->memory)
    {
        memcpy(&(mem->memory[mem->size]), strdata.c_str(), realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}

static int Effect_progress_func(void *bar, double t, double d, double ultotal, double ulnow)
{
    printf_s("%f / %f / (%f )\n", d, t, d*100.0/t);
    return 0;
}

static bool checkMD5File(const char * md5FilePath, Document &md5Doc)
{
    bool firstDownload = true;
    FILE * md5File = fopen(md5FilePath, "a+");
    if (md5File)
    {
        fseek(md5File, 0, SEEK_END);
        long lSize = ftell(md5File);
        fseek(md5File, 0, SEEK_SET);
        char* buffer = (char*)malloc(sizeof(char)*lSize +1 );
        memset(buffer, 0x00, (sizeof(char)*lSize + 1));// rapidjson parse string must have EOF
        size_t readSize = fread(buffer, 1, lSize, md5File);
        if (md5Doc.Parse<kParseDefaultFlags>(buffer).HasParseError())
        {
            firstDownload = true;
        }
        else
        {
            firstDownload = false;
        }
        free(buffer);
        fclose(md5File);
    }
    return firstDownload;
}

static bool IsdigitAll(string str)
{
	for (int i = 0; i<str.size(); i++)
	{
		if (!isdigit(str[i]))
		{
			return false;
		}
	}
	return true;
}

EffcctManager::EffcctManager()
:m_pCurl(NULL),
m_pList(NULL)
{
    m_effectCategoryList.clear();
}

EffcctManager::~EffcctManager()
{
    curl_slist_free_all(m_pList);
    curl_easy_cleanup(m_pCurl);
    curl_global_cleanup();
    m_pCurl = NULL;
    m_pList = NULL;
    m_effectCategoryList.clear();
}

bool EffcctManager::writeFile(const char* filePath, const char * fileData, const unsigned dataSize)
{
    FILE* fTmpMem = (FILE*)fileData;
    if (!fTmpMem) {
        printf("data is null");
        return false;
    }

    FILE *fp = fopen(filePath, "wb");
    if (!fp)
    {
        printf("open file failed");
        return false;
    }

    fwrite(fTmpMem, 1, dataSize, fp);
    fclose(fp);
    return true;
}

bool EffcctManager::downLoadThumb(const char * url, const char* thumbDir)
{
    MemoryStruct oDataChunk;
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &oDataChunk);
    curl_easy_setopt(m_pCurl, CURLOPT_URL, url);
    CURLcode res = curl_easy_perform(m_pCurl);
    long res_code = 0;
    res = curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &res_code);

    if ((res == CURLE_OK) && (res_code == 200 || res_code == 201))
    {
        std::string thumbFilePath;
        PathJoinA(thumbFilePath, thumbDir, "thumb.png");
        return writeFile(thumbFilePath.c_str(), oDataChunk.memory, oDataChunk.size);
    }
    return false;
}

bool EffcctManager::downLoadEffectZip(const char *url, const char* effectZipDir)
{
    MemoryStruct oDataChunk;
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &oDataChunk);
    curl_easy_setopt(m_pCurl, CURLOPT_URL, url);
    CURLcode res = curl_easy_perform(m_pCurl);
    long res_code = 0;
    res = curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &res_code);

    if ((res == CURLE_OK) && (res_code == 200 || res_code == 201))
    {
        std::string effectZipFilePath;
        PathJoinA(effectZipFilePath, effectZipDir, "effect.zip");
        return writeFile(effectZipFilePath.c_str(), oDataChunk.memory, oDataChunk.size);
    }
    return false;
}

bool EffcctManager::downLoadFile(const char *url, const char*filePath)
{
    MemoryStruct oDataChunk;
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &oDataChunk);
    curl_easy_setopt(m_pCurl, CURLOPT_URL, url);
    CURLcode res = curl_easy_perform(m_pCurl);
    long res_code = 0;
    res = curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &res_code);

    if ((res == CURLE_OK) && (res_code == 200 || res_code == 201))
    {
        return writeFile(filePath, oDataChunk.memory, oDataChunk.size);
    }
    return false;
}

bool EffcctManager::InitEffectManager(const char* runDir)
{
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != res)
    {
        printf_s("curl init failed");
        return false;
    }
    m_pCurl = curl_easy_init();

    if (NULL == m_pCurl)
    {
        printf_s("Init CURL failed...");
        return false;
    }

    std::string certificatePath;
    PathJoinA(certificatePath, runDir, "cacert-2020-01-01.pem");
    
    curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 3L);//请求超时时长
    curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, 10L); //连接超时时长 
    curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1L);//允许重定向
    curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 0L);  //若启用，会将头文件的信息作为数据流输出
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);  //得到请求结果后的回调函数
    curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, Effect_progress_func);
    curl_easy_setopt(m_pCurl, CURLOPT_CAINFO, certificatePath.c_str());

    m_pList = curl_slist_append(m_pList, "Accept-Encoding:gzip, deflate, sdch");
    m_pList = curl_slist_append(m_pList, "Accept-Language:zh-CN,zh;q=0.8");
    m_pList = curl_slist_append(m_pList, "Connection:keep-alive");
    curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_pList);


    curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L); //关闭中断信号响应
    curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L); //启用时会汇报所有的信息
    return true;
}

bool EffcctManager::FindLocalDir(const char* runDir, vector<string> &dirList, const char *format)
{
	if (NULL == runDir)return false;
	string scanPath;
	PathJoinA(scanPath, runDir, format);
	intptr_t handle;
	_finddata_t findData; // _finddata_t是存储文件各种信息的结构体
	handle = _findfirst(scanPath.c_str(), &findData);
	if (handle == -1)return false; // 检查是否成功
	do
	{
		if (findData.attrib & _A_SUBDIR && strcmp(format, "*") == 0) // 目录
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;
			std::string categoryDir;
			PathJoinA(categoryDir, scanPath, findData.name);
			if (!doesFileExist(categoryDir))
			{
				dirList.push_back(findData.name);
			}
		}
		else if (strcmp(format, "*.zip") == 0)
		{
			dirList.push_back(findData.name);
		}
	} while (_findnext(handle, &findData) == 0);
	_findclose(handle);    // 关闭搜索句柄*/
	return true;
}

bool EffcctManager::LoadAssetStoreFormXml(const char* runDir)
{
	string strXmlPath;
	PathJoinA(strXmlPath, runDir, "EffectAssetsInfo.xml");
	tinyxml2::XMLDocument m_xmlDoc;
	if (m_xmlDoc.LoadFile(strXmlPath.c_str())!=0)
		return false;

	tinyxml2::XMLElement *pNodeEle= m_xmlDoc.selectSingleNode("//RootNode");
	while (pNodeEle)
	{
		string strNodeName = string(pNodeEle->Attribute("NodeName"));
		string strAssetsPath;
		PathJoinA(strAssetsPath, runDir, strNodeName);

		tinyxml2::XMLElement *pSubEle = pNodeEle->selectSingleNode("FilterNode");
		while (pSubEle)
		{
			string strFilterNode = string(pSubEle->Attribute("FilterNodeName"));
			string strFilterPath;
			PathJoinA(strFilterPath, strAssetsPath, strFilterNode);

			tinyxml2::XMLElement *pItem = pSubEle->selectSingleNode("FilterItem");
			EffectCategory currentCategory;
			if (!IsdigitAll(strFilterNode))continue;
			currentCategory.id = std::stoi(strFilterNode);
			currentCategory.name = strFilterNode;
			while (pItem)
			{
				string strFilterItem = string(pItem->Attribute("FilterItemName"));
				tinyxml2::XMLElement * pItemFileName = pItem->FirstChildElement("ItemFileName");
				EffectItem currentItem;
				memset(&currentItem, 0x00, sizeof(currentItem));
				//currentItem.name = FilterItem;
				if (!IsdigitAll(strFilterItem))continue;
				currentItem.id = std::stoi(strFilterItem);
				if (pItemFileName != NULL)
				{
					const char* text = pItemFileName->GetText();
					if (text != NULL && strcmp(text,"") != 0)
						currentItem.effectName = string(text);
				}

				pItemFileName = pItem->FirstChildElement("AnotherName");
				if (pItemFileName != NULL)
				{
					const char* text = pItemFileName->GetText();
					if (text != NULL)
						currentItem.name = string(text);
				}
				/*string zipPath;
				PathJoinA(zipPath, strFilterPath, strFilterItem);
				vector<string> zipList;
				if (FindLocalDir(zipPath.c_str(), zipList, "*.zip"))
				{
					currentItem.effectName = zipList[0];
				}
				else
				{
					currentItem.effectName = "filter";
				}*/
				currentCategory.effectItemList.push_back(currentItem);
				pItem = pItem->NextSiblingElement("FilterItem");
			}
			m_effectCategoryList.push_back(currentCategory);
			pSubEle = pSubEle->NextSiblingElement("FilterNode");
		}
		pNodeEle = pNodeEle->NextSiblingElement("FilterNode");
	}
	return true;
}

bool EffcctManager::LoadAssetStore(const char *url, const char* runDir)
{
    std::lock_guard<std::recursive_mutex> lock(g_effectMutex);

    std::string md5FilePath;
    PathJoinA(md5FilePath, runDir, "md5.info");
    Document md5Doc;
    md5Doc.SetObject();
    Document::AllocatorType& allocator = md5Doc.GetAllocator();
    bool firstDownload = checkMD5File(md5FilePath.c_str(), md5Doc);
    bool isLoadSuccess = false;

    if (url)
    {
        MemoryStruct oDataChunk;  //请求结果的保存格式
        curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &oDataChunk);
        curl_easy_setopt(m_pCurl, CURLOPT_URL, url);
        CURLcode res = curl_easy_perform(m_pCurl);
        long res_code = 0;
        res = curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &res_code);

        if ((res == CURLE_OK) && (res_code == 200 || res_code == 201))
        {
            Document doc;
            std::string tempstring = oDataChunk.memory;
            bool parseFlag = doc.Parse<kParseDefaultFlags>(oDataChunk.memory).HasParseError();
            if ((!parseFlag) && doc.HasMember("data"))
            {
                std::string effectAssets;
                PathJoinA(effectAssets, runDir, "Assets");
                makeDirectory(effectAssets);
                const Value & data = doc["data"];
                for (SizeType i = 0; i < data.Size(); i++)
                {
                    unsigned int categoryID = data[i]["id"].GetUint();
                    if (categoryID != 297 && categoryID != 301)//Eliminate beauty
                    {
                        std::string sCategory = std::to_string(categoryID);
                        std::string categoryDir;
                        PathJoinA(categoryDir, effectAssets, sCategory);
                        makeDirectory(categoryDir);
                        std::string categoryThumbPath;
                        std::string seletedCategoryThumbPath;
                        std::string categoryThumbURL;
                        std::string seletedCategoryThumbURL;
                        PathJoinA(categoryThumbPath, categoryDir, "categoryThumb.png");
                        PathJoinA(seletedCategoryThumbPath, categoryDir, "seletedCategoryThumb.png");
                        if (!doesFileExist(categoryThumbPath))
                        {
                            categoryThumbURL = data[i]["thumb"].GetString();
                            if (!categoryThumbURL.empty())
                            {
                                downLoadFile(categoryThumbURL.c_str(), categoryThumbPath.c_str());
                            }
                        }
                        if (!doesFileExist(seletedCategoryThumbPath))
                        {
                            seletedCategoryThumbURL = data[i]["selectedThumb"].GetString();
                            if (!seletedCategoryThumbURL.empty())
                            {
                                downLoadFile(seletedCategoryThumbURL.c_str(), seletedCategoryThumbPath.c_str());
                            }
                        }

                        EffectCategory currentCategory;
                        currentCategory.id = categoryID;
                        currentCategory.name = data[i]["name"].GetString();
                        const Value & effectItems = data[i]["icons"];
                        for (SizeType j = 0; j < effectItems.Size(); j++)
                     {
                            unsigned int effectItemID = effectItems[j]["id"].GetUint();
                            if (effectItemID != 9222)//Eliminate facelifting opt effect zip
                            {
                                EffectItem currentItem;
                                memset(&currentItem, 0x00, sizeof(currentItem));
                                currentItem.id = effectItemID;
                                std::string sItemID = std::to_string(effectItemID);
                                currentItem.name = effectItems[j]["name"].GetString();
                                unsigned md5Length = effectItems[j]["md5"].GetStringLength();
                                memcpy(currentItem.md5 ,effectItems[j]["md5"].GetString(), md5Length);
                                currentItem.thumbURL = effectItems[j]["thumb"].GetString();
                                currentItem.effectZipURL = effectItems[j]["url"].GetString();
                                currentItem.isNeedUpdateEffectZip = false;
                                if (!firstDownload && (!md5Doc.IsNull())  && md5Doc.HasMember(sItemID.c_str()))
                                {
                                    std::string fileMD5 = md5Doc[sItemID.c_str()].GetString();
                                    if (strcmp(fileMD5.c_str(), currentItem.md5))//if md5 is different then re-download
                                    {
                                        md5Doc[sItemID.c_str()].SetString(currentItem.md5, md5Length);
                                        currentItem.isNeedUpdateEffectZip = true;
                                    }
                                }
                                else
                                {
                                    Value key;
                                    Value md5Value;
                                    size_t idlength = sItemID.size();
                                    key.SetString(sItemID.c_str(), sItemID.size(),allocator);
                                    md5Value.SetString(currentItem.md5, md5Length, allocator);
                                    md5Doc.AddMember(key, md5Value, allocator);
                                    currentItem.isNeedUpdateEffectZip = true;
                                }
                                currentCategory.effectItemList.push_back(currentItem);
                                if (currentItem.isNeedUpdateEffectZip)
                                {
                                    currentItem.isStartingDownLoadThumb = true;
                                    currentItem.isStartingDownEffectZip = true;
                                    std::string effectDir;
                                    PathJoinA(effectDir, categoryDir, sItemID);
                                    makeDirectory(effectDir);
                                    currentItem.isFinshedDownLoadThumb = downLoadThumb(currentItem.thumbURL.c_str(), effectDir.c_str());
                                    currentItem.isFinshedDownEffectZip = downLoadEffectZip(currentItem.effectZipURL.c_str(), effectDir.c_str());
                                    currentItem.isNeedUpdateEffectZip = false;
                                }
                            }
                        }
                        m_effectCategoryList.push_back(currentCategory);
                    }
                }
                isLoadSuccess = true;
            }
        }
    }

    StringBuffer sbuffer;
    Writer<StringBuffer> writer(sbuffer);
    md5Doc.Accept(writer);
    writeFile(md5FilePath.c_str(), sbuffer.GetString(), sbuffer.GetLength());

    return isLoadSuccess;
}

bool  EffcctManager::LoadAssetStore(const char* runDir)
{
    std::lock_guard<std::recursive_mutex> lock(g_effectMutex);
    bool isLoadSuccess = false;
    std::string effectAssets;
    PathJoinA(effectAssets, runDir, "Assets");
    std::string categoryListFilePath;
    PathJoinA(categoryListFilePath, effectAssets, "CategoryList.txt");
    FILE *fp = fopen(categoryListFilePath.c_str(), "rb");
    if (!fp)
    {
        printf("open categoryList.txt file failed");
        return isLoadSuccess;
    }
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    //
    // don't read file content if it is empty.
    //
    if (size == 0)
    {
        printf("file GetContents size empty: %s", categoryListFilePath.c_str());
        fclose(fp);
        return isLoadSuccess;
    }

    char *buf = new char[size + 1];
    size_t sizeRead = fread(buf, 1, size, fp);
    buf[size] = '\0';
    Document doc;
    bool parseFlag = doc.Parse<kParseDefaultFlags>(buf).HasParseError();
    if ((!parseFlag) && doc.HasMember("data"))
    {
        std::string effectAssets;
        PathJoinA(effectAssets, runDir, "Assets");
        makeDirectory(effectAssets);
        const Value & data = doc["data"];
        for (SizeType i = 0; i < data.Size(); i++)
        {
            unsigned int categoryID = data[i]["id"].GetUint();
            if (categoryID != 285 && categoryID != 288)//Eliminate beauty category and debug
            {
                EffectCategory currentCategory;
                currentCategory.id = categoryID;
                currentCategory.name = data[i]["name"].GetString();
                const Value & effectItems = data[i]["icons"];
                for (SizeType j = 0; j < effectItems.Size(); j++)
                {
                    unsigned int effectItemID = effectItems[j]["id"].GetUint();
                    if (effectItemID != 9222)//Eliminate facelifting opt effect zip
                    {
                        EffectItem currentItem;
                        memset(&currentItem, 0x00, sizeof(currentItem));
                        currentItem.id = effectItemID;
                        std::string sItemID = std::to_string(effectItemID);
                        currentItem.name = effectItems[j]["name"].GetString();
                        unsigned md5Length = effectItems[j]["md5"].GetStringLength();
                        memcpy(currentItem.md5, effectItems[j]["md5"].GetString(), md5Length);
                        currentItem.thumbURL = effectItems[j]["thumb"].GetString();
                        currentItem.effectZipURL = effectItems[j]["url"].GetString();
                        currentItem.isNeedUpdateEffectZip = false;
                        currentCategory.effectItemList.push_back(currentItem);
                    }
                }
                m_effectCategoryList.push_back(currentCategory);
            }
        }
        isLoadSuccess = true;
    }
    delete[] buf;
    buf = nullptr;
    fclose(fp);
    return isLoadSuccess;
}

std::vector<EffectCategory> EffcctManager::GetEffectCategoryList()
{
    std::lock_guard<std::recursive_mutex> lock(g_effectMutex);
    return m_effectCategoryList;
}

bool EffcctManager::DownLoadThumb(const char * url, const char* thumbDir)
{
    std::lock_guard<std::recursive_mutex> lock(g_effectMutex);
    downLoadThumb(url, thumbDir);
    return true;
}

bool EffcctManager::DownLoadEffectZip(const char *url, const char* effectZipDir)
{
    std::lock_guard<std::recursive_mutex> lock(g_effectMutex);
    downLoadEffectZip(url, effectZipDir);
    return true;
}