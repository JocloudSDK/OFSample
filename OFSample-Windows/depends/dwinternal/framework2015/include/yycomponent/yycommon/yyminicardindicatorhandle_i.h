#pragma once

#include "dwbase/dwcomex.h"
#include "qvector.h"
#include "duifw/duiwidget.h"

/**
* @brief Mini card infomation interface.
*/
struct IMiniCardInfo
{
	/**
	* @brief �û�uid.
	* @param .
	* @return.
	*/
	virtual quint32 uid() const = 0;

	/**
	* @brief �û��������id���ҵĺ��ѷ���id=1�������ϵ�˷���-2.
	* @param .
	* @return .
	*/
	virtual quint32 foldId() const = 0;

	/**
	* @brief �û��ȼ�.
	* @param .
	* @return .
	*/
	virtual int userLevel() const = 0;

	/**
	* @brief �û��ǳ�.
	* @param .
	* @return.
	*/
	virtual QString userNick() const = 0;

	/**
	* @brief Ƶ��id, ����0��ʾû���κ�Ƶ��.
	* @param .
	* @return.
	*/
	virtual quint32 channelId() const = 0;

	/**
	* @brief ���������Ϸ �ձ�ʾû����.
	* @return .
	*/
	virtual QString playGame() const = 0;
	/**
	* @brief �Ƿ��ֻ��û�.
	* @return.
	*/
	virtual bool bMobile() const = 0; 

    /**
    * @brief �Ƿ���vip�û�.
    * @return .
    */
    virtual bool bVipMember() const = 0;  

    /**
    * @brief vip�ȼ�.
    * @return .
    */
    virtual quint32 vipGrade() const = 0;

    /**
    * @brief vip��ֵ����.
    * @return .
    */
    virtual quint32 vipType() const = 0;

	/**
    * @brief �Ա𣬷ֱ�Ϊmale,female.
    * @return ����Ϊ�մ�.
    */
    virtual QString gender() const = 0;

    /**
    * @brief ��Դ.
    * @return ��yyminicard_i.h��minicard::From.
    */
    virtual int from() const = 0;
};

DWDEFINE_INTERFACE(IYYMiniCardIndicatorHandle)
{
	virtual ~IYYMiniCardIndicatorHandle(){}

	/**
	* @brief handle���֣�ÿ��handle��һ�����֣��ڽ���click�¼���ʱ����������.
	* @return.
	*/
	virtual QString handleName() = 0;

	/**
	* @brief �Ƿ�Ҫ��ʾiconͼ��.
	* @param . 
	* @return true��ʾ��ʾ��������ʾ.
	*/
	virtual bool	isShowIcon(IMiniCardInfo *userInfo) = 0;

	/**
	* @brief ��ɫ��ʾ��ͼƬ���������Ͽ��ϵ�ҵ��ͼ�������֣�һ���ǵ����ģ�һ����û�����ģ�Ҳ���ǻ�ɫ�ģ�mini���Ͽ�����ҵ�񷵻ص��Ƿ�չʾ����˵�ҵ��ͼ�����ж�ȡ����ͼ�껹�ǲ�����ͼ��.
	* @param userInfo ���㿨��Ϣ�������뿴IMiniCardInfo.
	* @return ͼƬ��·��.
	*/
	virtual QString hideStatusPic(IMiniCardInfo *userInfo) = 0;

	/**
	* @brief ��ɫ��ʾ��ͼƬ.
	* @param userInfo ���㿨��Ϣ�������뿴IMiniCardInfo.
	* @return ͼƬ��·��.
	*/
	virtual QString showStatusPic(IMiniCardInfo *userInfo) = 0;	

	/**
	* @brief tooltip����ʾ����.
	* @param .
	* @return. 
	*/
	virtual QString toolTipInfo(IMiniCardInfo *userInfo) = 0;

	/**
	* @brief �����Զ���tooltip��ʽ��widget.
	* @param userInfo ���㿨��Ϣ�������뿴IMiniCardInfo.
	* @return .
	*/
	virtual DuiWidget *tooltipWidget(IMiniCardInfo *userInfo) = 0;

	/**
	* @brief ��ʾ�Զ����tooltip��ʽ.
	* @param parentWidget �������㿨�ĸ�Qwidget.
	* @param itemInfo ���㿨��Ϣ�������뿴IMiniCardInfo. 
	* @param offset tooltip��ʾ���λ��.
	* @return.
	*/
	virtual void showCustomTooltip(QWidget* parentWidget, IMiniCardInfo* itemInfo, const QPointF& offset) = 0;

	/**
	* @brief Ԥ����ȡ���ݣ�������������ȥ������Ӧ��.
	* @param . 
	* @return .
	*/
	virtual void preFetchData(QVector<quint32> userId) = 0;

	/**
	* @brief ��ȡ��λ�����û��ʾ��ע��˳��������� �����ӵĽӿڷ����.
	* @param uid �û�uid.
	* @return ��������λ��.
	*/
	virtual int   getPreferredPosition(quint32 uid) = 0;

	virtual QString showStatusText(IMiniCardInfo *userInfo) = 0;

	virtual QString expandData(IMiniCardInfo *userInfo) = 0;

};
