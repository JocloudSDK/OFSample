#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycommon/yynoticetipwidget_i.h"
#include "yycomponent/yyim/yyimframetipinfomodel_i.h"
#include "duifw/duitextedit.h"

class DuiFrameWindow;
class DuiWidget;
class DuiTextAbstractIntellisenseInput;

DWDEFINE_INTERFACE(IYYImChatWidget) : public IDWQComponent
{
	//聊天窗口的位置
	enum CHATFRAMEPOS
	{
		POS_BEFORE_NICKNAME,
		POS_BEHIND_NICKNAME
	};

    enum SOUND_ALERT_TYPE
    {
        SAT_TEXT = 0x1, //文字消息
        SAT_VOICE = 0x2, //语音消息
        SAT_OTHER = 0x4, //其它的消息提示，如自动回复、文件传输完成等等
        SAT_1vs1 = 0x8, //1对1的会话
        SAT_1vsX = 0x10, //1对多的会话
        SAT_VIDEO = 0x20,    // 视频消息
    };
	/**
	* @brief . IM对话框中正在聊天对方的uid
	* @param . 无
	* @return .对方的uid
	*/
	virtual quint32 peerId() = 0;

	/**
	* @brief . 返回IM聊天对话框的真实的窗口类。
	* @param . 无
	* @return. 聊天对话框的真实窗口类。
	*/
	virtual DuiWidget* realWidget()	= 0;

	/**
	* @brief . 得到聊天框内的noticetip widget  已废弃
	* @return. noticetipwidget
	*/
	virtual IYYNoticeTipWidgetPtr getNoticeTipWidgetWrap() = 0;
	
	/**
	* @brief . 向IM窗口新增一个tip.
	* @return. 新增的tip对象指针
	*/
	virtual IYYImFrameTipInfoModelPtr appendNoticeTipModel() = 0;

	/**
	* @brief . 通过TipInfoModel 的objectname属性 查找到对应的Model对象, 
			   也可传入modulename查询，当传入的形参为modulename时，返回的是此modelname对应的优先级最高的Model对象
	* @return. 返回tipInfoModel对象指针
	*/
	virtual IYYImFrameTipInfoModelPtr getNoticeTipModel( const std::wstring& objectname ) = 0;

	/**
	* @brief 用户自己创建的对象
	* @brief 插入位置
	*/
	virtual bool insertWidget(DuiWidget* pWidget, CHATFRAMEPOS pos) = 0;
    
    /**
	* @brief 让窗口在任务栏闪烁
	* @brief 闪烁类型 SOUND_ALERT_TYPE
	*/
    virtual void flashWindow(int msgAlertType) = 0;

    /**
	* @brief 设置im窗口的皮肤
	* @brief 皮肤的路径，如果是空表示使用当前im设置的皮肤
	*/
    virtual void setDecorationImage(const QString& imagePath) = 0;

	/**
	* @brief 添加文本解析器
	* @brief 
	*/
	virtual void addIntellisenseInput(DuiTextAbstractIntellisenseInput *) = 0;
	virtual void insertIntellisenseInput( const QString &beforeObjectName, DuiTextAbstractIntellisenseInput *input ) = 0;

	/*
	* @ brief 获取消息显示editor
	*/
	virtual DuiTextEdit* displayEditor() = 0;

	/**
	* @brief 已存在im p2p video后，通知其他imchat更新UI
	* @brief
	*/
	virtual void onlyOneVideoInstantError() = 0;

    virtual void removeNoticeTipModel(IYYImFrameTipInfoModelPtr spTipInfo) = 0;
};