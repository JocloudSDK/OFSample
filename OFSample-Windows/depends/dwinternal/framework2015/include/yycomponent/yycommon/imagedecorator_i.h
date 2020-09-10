#pragma once

#include "dwbase/dwcomex.h"

//��Ƶװ�β�
DWDEFINE_INTERFACE(IImageDecorateLayer) : public IUnk
{
	virtual ~IImageDecorateLayer(){}
	//���ü���ȡID������ͨ��IDʶ��ͼ��
	virtual void SetUserData(ULONG_PTR uID) = 0;
	virtual ULONG_PTR GetUserData() = 0;
	virtual void SetName(LPCTSTR pszName) = 0;
	virtual LPCTSTR GetName() = 0;
	//���ƽӿ�,ˢ��ͼ��֡ʱ����
	virtual void Draw(HDC hdc,LPCRECT pRect) = 0;
};

enum
{
	ZORDER_FIRST=0,
	ZORDER_LAST=-1,
};

//IDecoratorImagePtr image;
//coCreateComInstance(KSERVICEDecorateImage, image);
DWDEFINE_INTERFACE(IDecoratorImage) : public IImageDecorateLayer
{
	virtual BOOL LoadFile(LPCTSTR pszFileName) = 0;
	virtual void SetPos(POINT pt) = 0;
};

//IDecoratorFlashPtr flash;
//coCreateComInstance(KSERVICEDecorateFlash, flash);
DWDEFINE_INTERFACE(IDecoratorFlash) :public IImageDecorateLayer
{
	virtual BOOL LoadFile(LPCTSTR pszFileName) = 0;

	virtual BOOL Create(HWND hOwner) = 0;

	virtual void SetSize(int nWid,int nHei) = 0;
};

typedef enum
{
	CR_COLORIZE=0,
	CR_SHIFTRGB,
	CR_LIGHT
} CRMODE;

//IDecoratorColorizePtr colorize;
//coCreateComInstance(KSERVICEDecorateColorize, colorize);
DWDEFINE_INTERFACE(IDecoratorColorize) : public IImageDecorateLayer
{
	virtual BOOL SetColorize(BYTE hue,BYTE sat,float blend) = 0;
	virtual BOOL SetLight(long brightness,long constrast) = 0;
	virtual BOOL SetRGB(short r,short g,short b) = 0;
	virtual void SetMode(CRMODE uMode) = 0;
};

//����,���GPU�����ã����ռ�ýϸ�CPU
struct IEnhanceNotify
{
	//true: �Կ����ã� false: �Կ�������
	virtual void OnChkBeautyUsefull(bool isGpuUsefull) = 0;
};

//IDecoratorEnhencePtr enhence;
//coCreateComInstance(KSERVICEDecorateEnhence, enhence);
DWDEFINE_INTERFACE(IDecoratorEnhence) :public IImageDecorateLayer
{
	virtual void EnableEnhance(bool isEnhance) = 0;
	virtual void SetNoise(int noise) = 0;
	virtual void SetSharp(int sharp) = 0;
	virtual void SetStatusNotify(IEnhanceNotify* pNotify) = 0;
	virtual void EnableBeauty(bool isBeauty) = 0;
	virtual bool GetIsEnableEnhance() = 0;
	virtual bool GetIsSupportGpu() = 0;
};

typedef
enum 
{
	FILTER_FLIP_X=0,
	FILTER_FLIP_Y,
	FILTER_MIRROR_X,
	FILTER_MIRROR_Y,
}FILTERMODE;

//IDecoratorFilterPtr filter;
//coCreateComInstance(KSERVICEDecorateFilter, filter);
DWDEFINE_INTERFACE(IDecoratorFilter) : public IImageDecorateLayer
{
	virtual void SetMode(FILTERMODE uMode) = 0;
};

//IDecoratorLyricPtr lyric;
//coCreateComInstance(KSERVICEDecorateLyric, lyric);
DWDEFINE_INTERFACE(IDecoratorLyric) : public IImageDecorateLayer
{
	//������ʾ����
	virtual BOOL SetLyricLine(LPCTSTR up_line, LPCTSTR down_line, double rate, UINT line_flag)= 0;
	//����������ɫ
	virtual void SetLyricColor(COLORREF crBg,COLORREF crHighlight)= 0;
	//�������ֱ�Ե��ɫ
	virtual void SetLyricBorderColor(COLORREF crBgBorder,COLORREF crHighlightBorder)=0;
	//������ʾ����
	virtual void SetLyricFont(LPCTSTR pszFontFace)= 0;
	//����������ʾ��С��1.0����Ĭ�ϴ�С��
	virtual void SetLyricZoom(double dZoom)= 0;
};

#define RATIOXY_MAX		2.0
#define RATIOXY_MIN		0.5

//IDecoratorStretchPtr stretch;
//coCreateComInstance(KSERVICEDecorateStretch, stretch);
DWDEFINE_INTERFACE(IDecoratorStretch) : public IImageDecorateLayer
{
	virtual void SetRatioXY(double r)=0;
};

enum TextFontStyle
{
	Normal,
	Italic,
	Bold,
	BoldItalic
};

enum DrawTextBrush
{
	DRAW_TEXT_DEFAULT_BRUSH = 0, //ȱʡ
	DRAW_TEXT_BINFEN_BRUSH = 1,  //�ͷ�����
	DRAW_TEXT_XUANLAN_BRUSH = 2, //��������
	DRAW_TEXT_QICAI_BRUSH = 3    //�߲�����
};
enum TextDirection
{
	TEXT_DIRECT_NO,             //��ֹ����
	TEXT_DIRECT_RIGHT,          //�������ҹ���
	TEXT_DIRECT_LEFT            //�����������
};

//IDecoratorTextPtr text;
//coCreateComInstance(KSERVICEDecorateText, text);
DWDEFINE_INTERFACE(IDecoratorText) : public IImageDecorateLayer
{
	virtual void SetText(LPCTSTR lpszText)=0;        //����
	virtual void SetBKColor(COLORREF cr)=0; //����
	virtual void SetTextColor(COLORREF cr)=0;
	virtual void SetTextFont(int fontSize, TextFontStyle fontStyle, LPCTSTR fontType, bool ifStrikeout, bool ifUnderline)=0;
	virtual void SetTextSpeed(float speedText)=0;
	virtual void SetTextMoveType(TextDirection moveTextType)=0;
	virtual void SetTextPos(int textPos)=0;
	virtual void SetTextTransLevel(double textTransLevel)=0;
	virtual void SetDrawTextBrush(DrawTextBrush brush)=0; 
};

DWDEFINE_INTERFACE(ImageDecorationMgr) : public IUnk
{
	virtual void InsertDecoration(IImageDecorateLayerPtr pDecoration, UINT zOrder) = 0;

	virtual BOOL RemoveDecoration(IImageDecorateLayerPtr pDecoration) = 0;

	virtual void RemoveAllDecoration() = 0;

	virtual int GetLayerCount() = 0;

	virtual IImageDecorateLayerPtr GetLayer(UINT iLayer) = 0;

	virtual void OnUpdate(HDC hdc,LPCRECT prect) = 0;
};

enum
{
	LAYER_TRANFORM=0,	//���β�
	LAYER_BACKGROUND,	//������
	LAYER_MIDDLE,		//�м��
	LAYER_FOREGROUND,	//ǰ����
};

//������Ч
DWDEFINE_INTERFACE(IMoodMusicPlayer) : public IUnk
{
	virtual void PlayFile(LPCTSTR fileName) = 0;
	virtual void SetDevice(LPCTSTR szName) = 0; //���������Ƶ�豸��
	virtual LPCTSTR GetDevice() = 0;
	virtual void SetVolume(double volPercent) = 0;  //��������
	virtual double GetVolume() = 0;
	virtual BOOL IsPlaying() = 0;
};

//��ʽ���ģ��
#define MAX_LYRICLINE	500
enum
{
	HL_FIRST=0,	//���ŵ�һ��
	HL_SECOND,	//���ŵڶ���
	HL_TITLE,	//�Ա�����ʽ���ŵ�һ��
};

enum LyricSegType
{
	LST_SEP=0,		//���
	LST_SINGING,	//������
};

struct LyricSegment
{
	int nStart;			//��ʼʱ��
	int nEnd;			//����ʱ��
	LyricSegType type;	//�ֶ�����
	int iLineIndex;		//����к�
};

DWDEFINE_INTERFACE(ILyricEngine) : public IUnk
{
	virtual BOOL playKrcLyric(LPCTSTR pszFileName)=0;
	virtual BOOL playLrcLyric(LPCTSTR pszFileName)=0;
	virtual BOOL playLrLyric(LPCTSTR pszFileName)=0;
	virtual LPCWSTR getSongTitle()=0;
	virtual LPCWSTR getSingerName()=0;
	virtual int getStartTimeStamp()=0;
	virtual BOOL getPlayingContent(int currPlayTime, wchar_t szFirstLine[MAX_LYRICLINE], wchar_t szSecondLine[MAX_LYRICLINE], double& rate, UINT & uPlayFlag)=0;
	virtual void offsetTimeStamp(int dOffset)=0;
	virtual int getTimeStampOffset()=0;
	//��ȡ��ʳ���ʱ����������ƫ����Ϣ
	virtual int getSongLength()=0;
	virtual int getSegmentInfo(int dStart, int dLength, LyricSegment *pSegs, int nCount)=0;
};

DWDEFINE_INTERFACE(ILyricDecoder) : public IUnk
{
	//����΢�����
	virtual BOOL KrcFromWesing(LPCTSTR pszWesing, LPCTSTR pszKrc) = 0;

	//���ܿṷ���
	virtual BOOL KrcFromKugou(LPCTSTR pszKugou, LPCTSTR pszKrc) = 0;
};

//ͼƬ����ӿ�
/*��ȡ�ӿ�
IImageDecoratorPtr spImageDecorator;
coCreateComInstance(KSERVICEImageDecorator, spImageDecorator);//ʵ�����ӿ�
*/
DWDEFINE_INTERFACE(IImageDecorator) : public IUnk
{
	//����ͼƬ��С
	virtual bool Init(int nWid, int nHei, int nBitPixel) = 0;

	virtual SIZE GetImageSize() = 0;

	virtual int GetBitPixel() = 0;

	//����ͼƬ֡����
	virtual void OnSetImage(const LPVOID pBuf, BOOL bTop2Bottom) = 0;

	//���ͼƬ��������
	virtual void Image2Buffer(LPBYTE pBuf, BOOL bTop2Bottom) = 0;

	//��ͼƬ���Ƶ�DC
	virtual void Draw(HDC hdc, LPRECT prect) = 0;

	//���װ��ͼ��������ӿ�
	virtual ImageDecorationMgrPtr GetDecorationMgr(UINT nLayer) = 0;

	//���moodmusic�ӿ�
	virtual IMoodMusicPlayerPtr GetMoodMusicPlayer() = 0;

	//���ILyricEngine�ӿ�
	virtual ILyricEnginePtr GetLyricEngine() = 0;

	//��ø�ʱ���ת������
	virtual ILyricDecoderPtr GetLyricDecoder() =0;

	//�ڶ��߳���ʹ��ʱ���߳�ͬ��ʱʹ�á�
	virtual void LockUpdate() = 0;

	virtual void UnlockUpdate() = 0;
};