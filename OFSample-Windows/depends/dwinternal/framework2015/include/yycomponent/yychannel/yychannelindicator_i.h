#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yychannel/yytreeindicatorhandler_i.h"
#include "yycomponent/yychannel/yyweideng_i.h"

/*
    use getObject function, have to release the smart pointer when component onUnload
*/
/**
* @brief .Ƶ��β�ƽӿ�
*/
DWDEFINE_INTERFACE(IYYChannelIndicator) : public IUnk
{
/**
   * @brief ���û��б�ע��β��.
   * @param hanlder��β�ƴ�����.
   * @return ��ʱ����.
   */
	virtual int registerIndicatorHandler( ITreeIndicatorHandler * hanlder ) = 0;
/**
   * @brief ���û��б�ע��β��.
   * @param hanlder��β�ƴ�����.
   * @return ��ʱ����.
   */
	virtual int unRegisterIndicatorHandler( ITreeIndicatorHandler * hanlder ) = 0;

/**
   * @brief �������б�ע��β��.
   * @param hanlder��β�ƴ�����.
   * @return ��ʱ����.
   */
	virtual int registerMaixuIndicatorHandler( ITreeIndicatorHandler * hanlder ) = 0;
/**
   * @brief �������б�ע��β��.
   * @param hanlder��β�ƴ�����.
   * @return ��ʱ����.
   */
	virtual int unRegisterMaixuIndicatorHandler( ITreeIndicatorHandler * hanlder ) = 0;

/**
   * @brief ע��Ƶ����ͷ�ƴ�����
   * @param priority ��ѯ���ȼ� (���õ���vip-100, MobileLight-200, WebyyLight-300)
   * @param handler ������ָ��
   */
	virtual int registerHeadLightIndicator(int priority, IHeadLightIndicatorHandler *handler) = 0;

	/**
   * @brief ��ע��Ƶ����ͷ�ƴ�����
   * @param handler ������ָ��
   */
	virtual int unRegisterHeadLightIndicator(IHeadLightIndicatorHandler *handler) = 0;

/**
   * @brief ���û��б�ע��β��.
   * @param hanlder��β�ƴ�����.
   * @return ��ʱ����.
   */
    virtual int registerIndicatorHandlerById( quint32 id, ITreeIndicatorHandler * handler ) = 0;
/**
   * @brief ���û��б�ע��β��.
   * @param hanlder��β�ƴ�����.
   * @return ��ʱ����.
   */
    virtual int unRegisterIndicatorHandlerById( quint32 id ) = 0;
};

//ȫ����UI����Ҫ����ӿ�
DWDEFINE_INTERFACE(IYYChannelIndicatorInner): public IDWQComponent 
{
	virtual QList<ITreeIndicatorHandler *> userListIndicators() const = 0;
	virtual QList<ITreeIndicatorHandler *> maixuListIndicators() const = 0;
    virtual QMap<quint32, ITreeIndicatorHandler*> treeIndicatorsMap() const = 0;

	/*
	signals:
	void userInfoUpdated(const QSet<quint32> &);
	void needUpdate();
	*/
};