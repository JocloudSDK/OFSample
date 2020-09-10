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
		Chat = 1,       // Ƶ������
		Broadcast = 2,   // Ƶ���㲥
        BubbleChat = 3,  // ��������
	};
/**
   * @brief .��ȡ���������
   * @param .��
   * @return .���������
   */
	virtual QFont inputFont() const = 0;
/**
   * @brief .��ȡ�������ɫ
   * @param .��
   * @return .������ɫ
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
    * @brief �����Ƽ�Ƶ�������5��.
    * @param ��Ƶ��id.
    * @return �����Ƿ�ɹ�
    */
    virtual bool setRecommendedSubSid(UINT32 subSid) = 0;

    /**
    * @brief ȡ���Ƽ�Ƶ��.
    * @param ��Ƶ��id.
    * @return ȡ���Ƿ�ɹ�.
    */
    virtual bool cancelRecommendedSubSid(UINT32 subSid) = 0;

    /**
    * @brief ��ѯ��Ƶ���Ƿ�Ϊ�Ƽ�Ƶ��.
    * @param ��Ƶ��id.
    * @return ��Ƶ���Ƿ�Ϊ�Ӵ�Ƶ��.
    */
    virtual bool isRecommendedSubSid(UINT32 subSid) = 0;
};
