#include "fileutils.h"
#include <windows.h>
#include "zlib.h"

bool doesFileExist(const std::string& filename)
{
    DWORD attr = GetFileAttributesA(filename.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES);
}

bool isDirectoryExists(const std::string& filename)
{
    DWORD attr = GetFileAttributesA(filename.c_str());
    return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));
}

bool makeDirectory(const std::string& folderPath)
{
    DWORD attr = GetFileAttributesA(folderPath.c_str());
    if ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY))
    {
        printf_s("Directory already exists.");
    }
    else
    {
        return CreateDirectoryA(folderPath.c_str(), NULL);
    }
    return true;
}

#define GZ_BUF_SIZE 1024

bool gzLoadFile(const char * gzfn, std::string &out)
{
	gzFile gzfp = gzopen(gzfn, "rb");
	if (!gzfp)
	{
		return false;
	}
	unsigned char buf[GZ_BUF_SIZE];
	int have;
	while ((have = gzread(gzfp, buf, GZ_BUF_SIZE)) > 0)
	{
		out.append((const char*)buf, have);
	}
	gzclose(gzfp);
	return true;
}

bool readBuildDisplayName(const char * packfile, std::string & out)
{
	FILE *p = fopen(packfile, "rt");
	if (p) 
	{
		char buf[GZ_BUF_SIZE] = {0};
		fseek(p,0, SEEK_END);
		size_t len = ftell(p);
		fseek(p, 0, SEEK_SET);
		while (fread(buf, 1, len, p) > 0)
		{
			out = out.append(buf);
		}
		fclose(p);
		return true;
	}
	return false;
}
