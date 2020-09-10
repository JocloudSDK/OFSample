#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yychannel/yyproperty.h"

class DuiWidget;
class DuiMenu;
struct ITreeIndicatorHandler;

DWDEFINE_INTERFACE(IChannelUserTreeWidget) : public IDWQComponent 
{
	
	enum FoldState
	{
		Expand // READ:չ���� WRITE:չ��
		,Collapse // READ:����� WRITE:����
	};

	typedef yyproperty::property<FoldState> FoldStatePro;

	/**
	* @brief ��ȡ��Ӧwidget.
	* @return widget.
	*/
	virtual DuiWidget*	widget() = 0;

	/**
	* @brief ע��Ƶ����β��.
	* @param handle,β�ƴ�����.
	* @return ע��ɹ�����true������false.
	*/
	virtual bool registerIndicatorHandler( ITreeIndicatorHandler * handler ) = 0;


	/**
	* @brief ��ע��Ƶ����β��.
	* @param handle,β�ƴ�����.
	* @return ��ע��ɹ�����true������false.
	*/
	virtual bool unRegisterIndicatorHandler( ITreeIndicatorHandler *handler) = 0;


	/**
	* @brief Ƶ�����Ƿ���չ��״̬.
	* @return Ƶ��������չ��״̬������true�����򷵻�false.
	*/
	virtual bool isExpanded() = 0; /* # obsolete , �� pro_foldState() == Expand ����*/

	/**
	* @brief չ��Ƶ����.
	*/
	virtual void expandTree() = 0; /* # obsolete , �� pro_foldState() = Expand ����*/

	/**
	* @brief ����Ƶ����.
	*/
	virtual void collapseTree() = 0; /* # obsolete , �� pro_foldState() = Collapse ���� */


	/**
	* @brief ��Ƶ������λĳ���û������䴦�ڿɼ�λ��.
	* @param uid���û�uid.
	* @return �ɹ�����0����Чid����-1.
	*/
	virtual int locateUser(quint32 uid) = 0;

	/**
	* @brief ��Ƶ������λĳ����Ƶ�������䴦�ڿɼ�λ��.
	* @param sid����Ƶ��id.
	* @return �ɹ�����0����Чid����-1.
	*/
	virtual int locateChannel(quint32 sid) = 0;

	/**
	* @brief ��ȡ��Ƶ��������ʾ�������������һ����Ƶ�����򲻰���������Ƶ������.
	* @param sid����Ƶ��id.
	* @param recursive�������Ƿ����������Ƶ������.
	* @return ��Ƶ������.
	*/
	virtual int subChanneUserCount( quint32 sid, bool recursive) = 0;

	/**
   * @brief Ƶ����չ��״̬, READ/WRITE.
   * @return FoldState.
   */
	virtual FoldStatePro& pro_foldState() = 0;

	typedef struct SChannelOnlineUserCount
	{
		quint32 userOnlineCount;//��ǰƵ��������������Ƶ��������.
		quint32 totalUserOnlineCount;//����������Ƶ����������.
	}TChannelOnlineUserCount;
	/**
   * @brief �ṩ��ģ��ҵ��ʹ�õ�����Ƶ�����������ӿ�,�ӿڵ�һ�λᴥ��һ��Ƶ����ˢ���߼�����������ÿ��3������һ��.
   * @param componentId������ҵ�������id.
   * @param onlineUserDataList��keyΪ��Ƶ��id,valueΪ��Ӧ���������������ÿ��б�ȡ���������.
   * @return bool.
   */
	virtual bool setAppChannelOnlineUserCount(quint32 componentId,const QMap<quint32,TChannelOnlineUserCount>& onlineUserDataList) = 0;

	/**
	* @brief �ṩ��ģ��ҵ��ʹ��,������ȡ�Ҽ����ָ�������û��ǳƺ�Ҫ��ʾ�Ĳ˵�.
	* @param uid������ҵ�񷽱�����û���uid.
	*/

	virtual DuiMenu* getMenu(quint32 uid) = 0;
};
