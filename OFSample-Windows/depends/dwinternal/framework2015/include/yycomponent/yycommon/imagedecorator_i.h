#pragma once

#include "dwbase/dwcomex.h"

//视频装饰层
DWDEFINE_INTERFACE(IImageDecorateLayer) : public IUnk
{
	virtual ~IImageDecorateLayer(){}
	//设置及获取ID，方便通过ID识别图层
	virtual void SetUserData(ULONG_PTR uID) = 0;
	virtual ULONG_PTR GetUserData() = 0;
	virtual void SetName(LPCTSTR pszName) = 0;
	virtual LPCTSTR GetName() = 0;
	//绘制接口,刷新图象帧时调用
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

//美白,如果GPU不可用，则会占用较高CPU
struct IEnhanceNotify
{
	//true: 显卡可用， false: 显卡不可用
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
	//设置显示内容
	virtual BOOL SetLyricLine(LPCTSTR up_line, LPCTSTR down_line, double rate, UINT line_flag)= 0;
	//设置文字颜色
	virtual void SetLyricColor(COLORREF crBg,COLORREF crHighlight)= 0;
	//设置文字边缘颜色
	virtual void SetLyricBorderColor(COLORREF crBgBorder,COLORREF crHighlightBorder)=0;
	//设置显示字体
	virtual void SetLyricFont(LPCTSTR pszFontFace)= 0;
	//设置文字显示大小。1.0代表默认大小。
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
	DRAW_TEXT_DEFAULT_BRUSH = 0, //缺省
	DRAW_TEXT_BINFEN_BRUSH = 1,  //缤纷文字
	DRAW_TEXT_XUANLAN_BRUSH = 2, //炫蓝文字
	DRAW_TEXT_QICAI_BRUSH = 3    //七彩文字
};
enum TextDirection
{
	TEXT_DIRECT_NO,             //禁止不动
	TEXT_DIRECT_RIGHT,          //从左往右滚动
	TEXT_DIRECT_LEFT            //从右往左滚动
};

//IDecoratorTextPtr text;
//coCreateComInstance(KSERVICEDecorateText, text);
DWDEFINE_INTERFACE(IDecoratorText) : public IImageDecorateLayer
{
	virtual void SetText(LPCTSTR lpszText)=0;        //内容
	virtual void SetBKColor(COLORREF cr)=0; //背景
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
	LAYER_TRANFORM=0,	//变形层
	LAYER_BACKGROUND,	//背景层
	LAYER_MIDDLE,		//中间层
	LAYER_FOREGROUND,	//前景层
};

//声音特效
DWDEFINE_INTERFACE(IMoodMusicPlayer) : public IUnk
{
	virtual void PlayFile(LPCTSTR fileName) = 0;
	virtual void SetDevice(LPCTSTR szName) = 0; //设置输出音频设备名
	virtual LPCTSTR GetDevice() = 0;
	virtual void SetVolume(double volPercent) = 0;  //设置音量
	virtual double GetVolume() = 0;
	virtual BOOL IsPlaying() = 0;
};

//歌词解析模块
#define MAX_LYRICLINE	500
enum
{
	HL_FIRST=0,	//播放第一行
	HL_SECOND,	//播放第二行
	HL_TITLE,	//以标题形式播放第一行
};

enum LyricSegType
{
	LST_SEP=0,		//间隔
	LST_SINGING,	//唱歌中
};

struct LyricSegment
{
	int nStart;			//开始时间
	int nEnd;			//结束时间
	LyricSegType type;	//分段类型
	int iLineIndex;		//歌词行号
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
	//获取歌词持续时长，不包含偏移信息
	virtual int getSongLength()=0;
	virtual int getSegmentInfo(int dStart, int dLength, LyricSegment *pSegs, int nCount)=0;
};

DWDEFINE_INTERFACE(ILyricDecoder) : public IUnk
{
	//解密微唱歌词
	virtual BOOL KrcFromWesing(LPCTSTR pszWesing, LPCTSTR pszKrc) = 0;

	//解密酷狗歌词
	virtual BOOL KrcFromKugou(LPCTSTR pszKugou, LPCTSTR pszKrc) = 0;
};

//图片管理接口
/*获取接口
IImageDecoratorPtr spImageDecorator;
coCreateComInstance(KSERVICEImageDecorator, spImageDecorator);//实例化接口
*/
DWDEFINE_INTERFACE(IImageDecorator) : public IUnk
{
	//设置图片大小
	virtual bool Init(int nWid, int nHei, int nBitPixel) = 0;

	virtual SIZE GetImageSize() = 0;

	virtual int GetBitPixel() = 0;

	//更新图片帧数据
	virtual void OnSetImage(const LPVOID pBuf, BOOL bTop2Bottom) = 0;

	//输出图片到缓冲区
	virtual void Image2Buffer(LPBYTE pBuf, BOOL bTop2Bottom) = 0;

	//将图片绘制到DC
	virtual void Draw(HDC hdc, LPRECT prect) = 0;

	//获得装饰图层管理器接口
	virtual ImageDecorationMgrPtr GetDecorationMgr(UINT nLayer) = 0;

	//获得moodmusic接口
	virtual IMoodMusicPlayerPtr GetMoodMusicPlayer() = 0;

	//获得ILyricEngine接口
	virtual ILyricEnginePtr GetLyricEngine() = 0;

	//获得歌词编码转换对象
	virtual ILyricDecoderPtr GetLyricDecoder() =0;

	//在多线程中使用时，线程同步时使用。
	virtual void LockUpdate() = 0;

	virtual void UnlockUpdate() = 0;
};