#pragma once

#include "orangefilter.h"
#include <string>
#include <vector>
#include <map>

class OrangeHelper
{
public:
    enum VenusType
    {
        VN_None = 0x00, 
        VN_Face = 0x01,   
        VN_Gesture = 0x02,  
        VN_BodySegment = 0x04, 
        VN_All = 0xFF     
    };
    
    enum EffectType
    {
        ET_BasicBeauty,			//��������
		ET_BasicBeauty5,		//��������
        ET_BasicBeautyType,		//��������
        ET_SeniorBeautyType,	//�߼�����
        ET_FilterHoliday,		//�����˾�
        ET_FilterClear,			//�����˾�
        ET_FilterWarm,			//ů���˾�
        ET_FilterFresh,			//�����˾�
        ET_FilterTender,		//�����˾�
						//20���˾����
		ET_FilterAdaier,		//��ɫ-������˾�
		ET_FilterDanya,			//��ɫ-�����˾�
		ET_FilterJiaZhou,		//��ɫ-�����˾�
		ET_FilterMojito,		//��ɫ-Ī�����˾�
		ET_FilterFennen,		//ůɫ-�����˾�
		ET_FilterPure,			//ůɫ-�ഺ�˾�
		ET_FilterSweet,			//ůɫ-�����˾�
		ET_FilterWenNuan,		//ůɫ-��ů�˾�
		ET_FilterSun,			//ůɫ-�����˾�
		ET_FilterGrilWhite,		//��Ů-����˾�
		ET_FilterGrilLuolita,	//��Ů-�������˾�
		ET_FilterGrilLiterature, //��Ů-�����˾�
		ET_FilterAmorous,		//��ǿ-�����˾�
		ET_FilterDelicious,		//��ǿ-�ɿ��˾�
		ET_FilterEnhanceClear,  //��ǿ-�����˾�
		ET_FilterEnhanceNoulan, //��ǿ-ŵ���˾�
		ET_FilterPeach,			//��ǿ-ˮ�����˾�
		ET_FilterCream,			//�ʸ�-������˾�
		ET_FilterFilm,			//�ʸ�-��Ƭ�˾�
		ET_FilterMagazine		//�ʸ�-��־�˾�
    };
    
	enum EffectParamType {
		EP_BasicBeautyIntensity,  //��������[0,100]
		EP_BasicBeautyOpacity,    //����ĥƤǿ��[0,100]
		EP_BasicBeautyIntensity5,   //��������[0,100]
		EP_BasicBeautyOpacity5,		//����ĥƤ[0,100]
		EP_FilterHolidayIntensity,  //�����˾�ǿ��[0,100]
		EP_FilterClearIntensity,    //�����˾�ǿ��[0,100]
		EP_FilterWarmIntensity,     //ů���˾�ǿ��[0,100]
		EP_FilterFreshIntensity,    //�����˾�ǿ��[0,100]
		EP_FilterTenderIntensity,   //�����˾�ǿ��[0,100]  
		//��������
		EP_BasicTypeIntensity,     	//��������ǿ��[0,100] 
		//�߼�����		
		EP_SeniorTypeThinFaceIntensity,      //�߼�����[0,100]     
		EP_SeniorTypeSmallFaceIntensity,     //�߼�С��[0,100] 
		EP_SeniorTypeSquashedFaceIntensity,   //�߼���ȧ��[0,100]
		EP_SeniorTypeForeheadLiftingIntensity, //�߼����[-50,50]
		EP_SeniorTypeWideForeheadIntensity,    //�߼����[-50,50]
		EP_SeniorTypeBigSmallEyeIntensity,     //�߼�����[-50,50]
		EP_SeniorTypeEyesOffsetIntensity,       //�߼��۾�[-50,50]        
		EP_SeniorTypeEyesRotationIntensity,    //�߼��۽�[-50,50]
		EP_SeniorTypeThinNoseIntensity,        //�߼��ݱ�[-50,50]
		EP_SeniorTypeLongNoseIntensity,         //�߼�����[-50,50]
		EP_SeniorTypeThinNoseBridgeIntensity,    //�߼�խ����[-50,50]
		EP_SeniorTypeThinmouthIntensity,         //�߼�С��[-50,50]
		EP_SeniorTypeMovemouthIntensity,         //�߼���λ[-50,50]
		EP_SeniorTypeChinLiftingIntensity,      //�߼��°�[-50,50]

		//20���˾����
		EP_FilterAdaierIntensity,			//��ɫ-������˾�ǿ��[0,100]
		EP_FilterDanyaIntensity,			//��ɫ-�����˾�ǿ��[0,100]
		EP_FilterJiaZhouIntensity,			//��ɫ-�����˾�ǿ��[0,100]
		EP_FilterMojitoIntensity,			//��ɫ-Ī�����˾�ǿ��[0,100]
		EP_FilterFennenIntensity,			//ůɫ-�����˾�ǿ��[0,100]
		EP_FilterPureIntensity,				//ůɫ-�ഺ�˾�ǿ��[0,100]
		EP_FilterSweetIntensity,			//ůɫ-�����˾�ǿ��[0,100]
		EP_FilterWenNuanIntensity,			//ůɫ-��ů�˾�ǿ��[0,100]
		EP_FilterSunIntensity,				//ůɫ-�����˾�ǿ��[0,100]
		EP_FilterGrilWhiteIntensity,		//��Ů-����˾�ǿ��[0,100]
		EP_FilterGrilLuolitaIntensity,		//��Ů-�������˾�ǿ��[0,100]
		EP_FilterGrilLiteratureIntensity,	//��Ů-�����˾�ǿ��[0,100]
		EP_FilterAmorousIntensity,			//��ǿ-�����˾�ǿ��[0,100]
		EP_FilterDeliciousIntensity,		//��ǿ-�ɿ��˾�ǿ��[0,100]
		EP_FilterEnhanceClearIntensity,		//��ǿ-�����˾�ǿ��[0,100]
		EP_FilterEnhanceNoulanIntensity,	//��ǿ-ŵ���˾�ǿ��[0,100]
		EP_FilterPeachIntensity,			//��ǿ-ˮ�����˾�ǿ��[0,100]
		EP_FilterCreamIntensity,			//�ʸ�-������˾�ǿ��[0,100]
		EP_FilterFilmIntensity,				//�ʸ�-��Ƭ�˾�ǿ��[0,100]
		EP_FilterMagazineIntensity			//�ʸ�-��־�˾�ǿ��[0,100]
	};

	enum LogLevel {
		LG_Info = 0x01,
		LG_Warn = 0x02,
		LG_Error = 0x04,
		LG_Debug = 0x08,
		LG_Verbose = 0xFF,
	};
    
    struct EffectParam
    {
        int curVal;
        int maxVal;
        int minVal;
        int defVal;
    };
    
    struct GLTexture
    {
        int textureId; //OpenGL texture id
        int width;  //OpenGL texture width.
        int height; //OpenGL texture height.
        int format; //OpenGL texture format, e.g. GL_RGBA.
        int target; //OpenGL texture target, e.g. GL_TEXTURE_2D.
    };
    

	struct ImageInfo
	{
		int deviceType;      //ͼ�����Ĭ��Ϊ���ͼ��0���û��Զ��崫��ͼ��Ϊ1.ʹ�÷����ͼ�����������Ҫ����
		int facePointDir;    //�� deviceType == 1ʱ���ñ�����������������Ⱦ��Ч�ķ��򡣷���ʹ�õײ�Ĭ�ϵķ���ȥ��Ⱦ��Ĭ�Ϸ����Ǹ�������ͷ�豸��Ϣ���õġ�
		unsigned char* data;  //�û������ʶ�����ݣ�ͨ������������
		int dir;   			  //ͼ��ķ���ͨ���������Ƿ���
		int orientation;    //�������ͷ����ͨ�����豸�����Ϣ���ȡ��������ΪdeviceType=1ʱ����ֵ����Ч��
		int width;   		//ͼ���
		int height;    		//ͼ���
		int format;     	//ͼ���ʽ
		bool frontCamera;   //�Ƿ���ǰ������ͷ
		float timestamp;    //��������ʶ���ʱ�������λ�룬��������ã��ײ�ʹ��Ĭ��ֵ��
	};

public:
    bool createContext(
           const std::string& serialNumber,
           const std::string& licensePath,
           const std::string& resDir,
           VenusType aiType = VN_All);

    void destroyContext();
    
    bool isContextValid();
    
    bool enableEffect(EffectType effectType, bool enabled);
    
    bool releaseEffect(EffectType effectType);
    
    bool enableSticker(const std::string& path, bool enabled);
    
    bool releaseSticker(const std::string& path);

    bool enableGesture(const std::string& path, bool enabled);
    
    bool releaseGesture(const std::string& path);
    
    int getEffectParam(EffectParamType paramType);
    
    bool getEffectParamDetail(EffectParamType paramType, EffectParam& paramVal);
    
    bool setEffectParam(EffectParamType paramType, int value);
    
    bool updateFrameParams(const GLTexture& textureIn, const GLTexture& textureOut, const ImageInfo& image);

    bool checkStickerResult(const std::vector<std::string>& paths, std::vector<int>& results);

	bool setLogLevel(int level);

	bool setLogCallback(void(*callback)(const char* msg));

private:
    struct EffectInfo
    {
        std::string path;
        int effectId;
        bool enabled;
        int result;
        EffectInfo() : effectId(0), enabled(false) {}
        EffectInfo(const std::string& p, int i, bool e) : path(p), effectId(i), enabled(e), result(0) {}
    };
    
    int getEffectId(EffectParamType ep);
    int getFilterId(EffectParamType ep);
    std::string getParamName(EffectParamType ep);
	void copyLicenseFromBundle(const std::string &licenseName, const std::string &licensePath);
	bool isNeedCheckSerialNumberOnline(const std::string &licensePath);
    
    OFHandle _context;
    std::string _resDir;
    std::vector<EffectInfo> _effects;
    EffectInfo _sticker;
    OF_Texture _inTex;
    OF_Texture _outTex;
    OF_FrameData _frameData;
};
