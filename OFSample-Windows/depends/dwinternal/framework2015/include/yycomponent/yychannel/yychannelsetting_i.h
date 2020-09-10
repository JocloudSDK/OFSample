#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
	void inputFontChanged( const QFont& font );
	void inputFontColorChanged( const QColor& color );
*/
DWDEFINE_INTERFACE(IYYChannelSetting) : public IDWQComponent
{
	enum Purpose
	{
		Chat = 1,       // 频道聊天
		Broadcast = 2,   // 频道广播
        BubbleChat = 3,  // 气泡聊天
	};
/**
   * @brief .获取输入的字体
   * @param .无
   * @return .输入的字体
   */
	virtual QFont inputFont() const = 0;
/**
   * @brief .获取输入的颜色
   * @param .无
   * @return .输入颜色
   */
	virtual QColor inputFontColor() const = 0;

	virtual QFont font(Purpose p) const = 0;

	virtual QColor fontColor(Purpose p) const = 0;

	virtual void setFont(const QFont &f, Purpose p) = 0;

	virtual void setFontColor(const QColor &c, Purpose p) = 0;

	virtual void saveConfig(unsigned int purpose) = 0;

	virtual QStringList quickReplyMessages() = 0;

	virtual void	saveQuickReplyMessage(const QStringList& messageList) = 0;

    /**
    * @brief 设置推荐频道，最多5个.
    * @param 子频道id.
    * @return 设置是否成功
    */
    virtual bool setRecommendedSubSid(UINT32 subSid) = 0;

    /**
    * @brief 取消推荐频道.
    * @param 子频道id.
    * @return 取消是否成功.
    */
    virtual bool cancelRecommendedSubSid(UINT32 subSid) = 0;

    /**
    * @brief 查询子频道是否为推荐频道.
    * @param 子频道id.
    * @return 子频道是否为接待频道.
    */
    virtual bool isRecommendedSubSid(UINT32 subSid) = 0;
};
