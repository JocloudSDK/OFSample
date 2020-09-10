#pragma once

#include "dwbase/dwcomex.h"

//����ģʽ���Լ̳�IYYImBuddyTreeItemInfoAreaHandler�ӿ�ʵ��һ��handler
//-��Ϊ����handlerȻ���ṩһЩ�ӿڲ��빲��handler�Ľӿ�


DWDEFINE_INTERFACE(IYYImBuddyTreeItemInfoAreaHandler)
{
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual UINT32 level() = 0; //���ȼ�
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool bInTurn() = 0; //��level�ȼ��Ƿ�������ʾ
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual UINT32 uid() = 0; // ��Ҫ��ʾ�����Ϣ�ĺ���uid
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual QString handleName() = 0; //Ψһ��ʶ���handle���ַ���
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool offlineShow() = 0; //���߻��������ʱ���Ƿ���ʾ

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, QRectF infoAreaRect ) = 0;

	/*tips:
	������¼���Χ������item������handlerʵ���⼸��������ʱ����Ҫ���ж��Ƿ����������������Լ�paint�Լ�������
	*/
	virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event, QRectF itemRect ) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event, QRectF itemRect ) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event, QRectF itemRect ) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event, QRectF itemRect ) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event, QRectF itemRect ) = 0;
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event, QRectF itemRect ) = 0;
};

DWDEFINE_INTERFACE(IYYImBuddyTreeItemInfoAreaModel) : public IUnk
{
	/**
	* @brief . ��ʾ���ȼ�����ͬһ���û���ǩ������ͬһ���Ż�������һ����ʾ
	* @param . ��
	* @return. ���ȼ���
	*/
	virtual UINT32	level() = 0; //���ȼ�
	/**
	* @brief . ����ʾǩ������modelΨһ����
	* @param . ��
	* @return. ������ʾǩ������model����
	*/
	virtual QString handleName() = 0; 
	/**
	* @brief . ����ʾmodel���Կ��������߻��������ʱ���Ƿ���ʾ
	* @param . ��
	* @return. ����trueΪ��ʾ���ݣ�falseΪ����ʾ
	*/
	virtual bool	offlineShow() = 0;
	/**
	* @brief . ��level�ȼ��Ƿ�������ʾ
	* @param . ��
	* @return. ����trueΪ��ת��falseΪ����ת
	*/
	virtual bool	bInTurn() = 0; 

	/**
	* @brief . ����model����Ϣ
	* @param . painter ��ͼ��handle, QT��������ͼ����
	* @param . option  ������ͼ��һЩ��������鿴Qt˵���ĵ�QStyleOptionGraphicsItem
	* @param . widget  ��ͼ�Ĵ�����
	* @param . infoAreaRect ��ͼ������
	* @return. ��
	*/
	virtual void	paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, QRectF infoAreaRect ) = 0;

	/**
	* @brief . �û������������ʱ��Ӧ
	* @param . event hoverenterʱ���¼����������Qt�ĵ��е�QGraphicsSceneHoverEvent
	* @param . itemRect hoverenterʱ����Ӧ������
	* @return. ��
	*/
	virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event, QRectF itemRect ) = 0;

	/**
	* @brief . �û�����ڸ��������ƶ�ʱ��Ӧ
	* @param . event hovermoveʱ���¼����������Qt�ĵ��е�QGraphicsSceneHoverEvent
	* @param . itemRect hovermoveʱ����Ӧ������
	* @return. ��
	*/
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event, QRectF itemRect ) = 0;

	/**
	* @brief . �û�����Ƴ�����ʱ��Ӧ
	* @param . event hoverleaveʱ���¼����������Qt�ĵ��е�QGraphicsSceneHoverEvent
	* @param . itemRect hoverleaveʱ����Ӧ������
	* @return. ��
	*/
	virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event, QRectF itemRect ) = 0;

	/**
	* @brief . �û������꣬ѹ�¶���ʱ��Ӧ
	* @param .event	 �������Qt�ĵ��е�QGraphicsSceneHoverEvent
	* @param . itemRect ��Ӧ������
	* @return. ��
	*/
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event, QRectF itemRect ) = 0;

	/**
	* @brief . �û������꣬�ɿ�����ʱ��Ӧ
	* @param . event	 �������Qt�ĵ��е�QGraphicsSceneHoverEvent
	* @param . itemRect ��Ӧ������
	* @return. ��
	*/
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event, QRectF itemRect ) = 0;

	/**
	* @brief . �û�˫�����ʱ��Ӧ
	* @param . event	 �������Qt�ĵ��е�QGraphicsSceneHoverEvent
	* @param . itemRect ��Ӧ������
	* @return. ��
	*/
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event, QRectF itemRect ) = 0;

	/**
	* @brief . ��Ҫ��ʾ�����Ϣ�ĺ���uid
	* @param . ��
	* @return. �û�uid
	*/
	virtual UINT32	uid() = 0; 

	/**
	* @brief . �����б�ǰuid���ڵķ���id
	* @param . ��
	* @return. Ŀ¼uid
	*/
	virtual UINT32	folderId() = 0;

	/**
	* @brief . ������Ҫ��ʾ�����Ϣ�ĺ���uid
	* @param . �û�uid
	* @return. ��
	*/
	virtual void setUid( const UINT32 uUid ) = 0;

	/**
	* @brief . ���ú����б�ǰuid���ڵķ���id
	* @param . Ŀ¼uid
	* @return. ��
	*/
	virtual void setFolderId( const UINT32 folderId ) = 0;
};