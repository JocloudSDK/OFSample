#ifndef ZIP_TEMPLATE_H_
#define ZIP_TEMPLATE_H_

#include <zlib.h>

#ifndef WIN32
#include <sys/time.h>
#include <unistd.h> 
#endif

template<class M, int TU> class ZipMarshal: public M{
public:
	enum {uri = TU};
	mutable uint32_t zSize;
	mutable std::string zipData;

	void marshal(sox::Pack & p) const{
		sox::PackBuffer pb;
		sox::Pack tmp(pb);

		M::marshal(tmp);


    int comressItor = 1;
#ifndef WIN32
    struct timeval enter_time;
    struct timeval leave_time;
    gettimeofday(&enter_time, NULL);

    if(!access("./non-compress.txt",F_OK)) 
      comressItor = 0;

    if(!access("./fast-compress.txt",F_OK)) 
      comressItor = 1;
#endif

		zSize = (uint32_t)tmp.size();
		uLong dlength = compressBound(zSize);
		Bytef *buf = new Bytef[dlength];
		memset(buf, 0, dlength);
		int ret = compress2(buf, &dlength, (const Bytef *)tmp.data(), (uLong)tmp.size(), comressItor);

#ifndef WIN32
    gettimeofday(&leave_time, NULL);
    int diff_tick = (leave_time.tv_sec - enter_time.tv_sec)*1000 + (leave_time.tv_usec - enter_time.tv_usec)/1000;
    //log(Debug, "compress %u --> %u cmd type: %u time:%d comressItor:%d", zSize, dlength, uri, diff_tick, comressItor);
#endif 

		if(ret == Z_OK){
			zipData.assign((const char *)buf, dlength);
		}else{
			//log(Info, "zip error");
		}
		delete[] buf;
		p << zSize;
		p.push_varstr32(zipData.data(), zipData.length());
	}

	void unmarshal(const sox::Unpack &p){
		p >> zSize;
		zipData = p.pop_varstr32();
		if(zipData.empty()){
			throw sox::UnpackError("zip data is zero");
		}else{
			Bytef *buf = new Bytef[zSize];
			uLong dLength = zSize;
			int ret = uncompress(buf, &dLength, (const Bytef *)zipData.data(), (uLong)zipData.length());

			if(ret != Z_OK){
				delete[] buf;
				throw sox::UnpackError("unzip error");
			}else{
				sox::Unpack up2(buf, dLength);
				M::unmarshal(up2);
				delete[] buf;
			}
		}

	}

};

#endif

