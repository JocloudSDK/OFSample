#pragma once
#include "curl.h"
#include <string>
#include <vector>
#include "tinyxml2.h"

using namespace std;
struct EffectItem
{
    std::string name;
	std::string effectName;
    unsigned int id;
    char md5[128];
    bool isStartingDownLoadThumb;
    bool isFinshedDownLoadThumb;
    bool isStartingDownEffectZip;
    bool isFinshedDownEffectZip;
    bool isNeedUpdateEffectZip;
    std::string thumbURL;
    std::string effectZipURL;
};

struct EffectCategory
{
    std::string name;
    unsigned int id;
    unsigned int effectItemNumber;
    std::vector<EffectItem> effectItemList;
};

class EffcctManager
{
public:
    EffcctManager();
    ~EffcctManager();
    bool InitEffectManager(const char* runDir);
    bool DownLoadThumb(const char * url, const char* thumbDir);
    bool DownLoadEffectZip(const char *url, const char* effectZipDir);
    bool LoadAssetStore(const char *url, const char* runDir);
    bool LoadAssetStore( const char* runDir);
	bool FindLocalDir(const char* runDir, vector<string> &dirList,const char *format);
	bool LoadAssetStoreFormXml(const char* runDir);
    std::vector<EffectCategory> GetEffectCategoryList();
private:
    bool downLoadThumb(const char * url, const char* thumbDir);
    bool downLoadEffectZip(const char *url, const char* effectZipDir);
    bool downLoadFile(const char *url, const char*filePath);
    bool writeFile(const char* filePath, const char * fileData, const unsigned dataSize);
    CURL * m_pCurl;
    curl_slist *m_pList;
    std::vector<EffectCategory> m_effectCategoryList;
};