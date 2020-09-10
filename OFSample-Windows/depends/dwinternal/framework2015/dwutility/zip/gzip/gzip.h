#ifndef __CONTENT_DECODER_H__
#define __CONTENT_DECODER_H__

#include <string>

class DwGzip
{
public:
    DwGzip(size_t buffLen = 1024);
    ~DwGzip();

    bool ungzip(unsigned char* gzdata, size_t len, std::string& out);

    bool gzipex(unsigned char* gzdata, size_t len, std::string& out);
    bool ungzipex(unsigned char* gzdata, size_t len, std::string& out);

private:
    size_t m_buffLen;
    unsigned char* m_buff;
};

#endif