#pragma once

#include "dwbase/dwcomex.h"

/**
* @brief IM���촰�ϵ�tipģ��.
  
  tip��ʾ����:
	ͬһmoduleName()��tip,ͨ�����ȼ�priority( )���Կ�����ʾ��ֻ��ʾ������ȼ���һ��tip

  model��ȡ��ʹ�÷���( ͳһ��IChatFrame�ӿ����� )
	IYYImChatFrameManagerPtr frameMgr = comMgr()->getServiceInterface<IYYImChatFrameManager>(KSERVICEImGroup);
	IYYImChatFramePtr charFrame = frameMgr->findImFrame( one.uid );
	IYYImFrameTipInfoModelPtr model = charFrame->appendNoticeTipModel( );
	
	model->setModuleName( L"channel" );
	model->setIconUrl( QString::fromStdString(one.url).toStdWString() );
	model->setText( QString::fromStdString(one.text).toStdWString() );
	model->setObjectName( LSTR(defIM_GameParty) );
	model->setPriority( TIP_PRIORITY_GAMEPARTY ); //�ɸ��Ե�module����
	model->setNeedCloseButton( true );
	model->setAcceptClick( true );

	model->update();    //��ص��˽ӿ�ˢ�½���. 

  model��ѯ:
	IYYImChatFrameManagerPtr frameMgr = comMgr()->getServiceInterface<IYYImChatFrameManager>(KSERVICEImGroup);
	IYYImChatFramePtr charFrame = frameMgr->findImFrame( one.uid );
	IYYImFrameTipInfoModelPtr model = charFrame->getNoticeTipModel( objectname or modulename );
	
	��ͨ��moduleName()��ѯʱ�� ����������ڸ�ģ���tip���򷵻ص�ǰ������ȼ�tip����ĵ�һ��tip����ָ��
	��ͨ��objectname()��ѯʱ�� ����objectname��Ӧ�Ķ���ָ��
	
  ע������:
	1.���������ݺ�,��ص���update�ӿ�ˢ�½���
	2.��modulenameΪ�յ�ʱ��,�������ȼ��ж�(��priority()�ֶα�����)������tip��ȫ����ʾ
*/

DWDEFINE_INTERFACE(IYYImFrameTipInfoModel):public IUnk
{
public:

	/**
	* @brief objectname ����ѯtipInfoModel����ʹ�� -- [��ѡ]
	*/
	virtual void setObjectName( const std::wstring& objectname ) = 0;
	
	/**
	* @brief ����objectname 
	*/
	virtual const std::wstring& objectName( )const = 0;
	
	/**
	* @brief ����Tip��ʾ�����ȼ�.�鿴 DEF_PRIORITY
	*/
	virtual void setPriority( int priority ) = 0;

	/**
	* @brief ������ʾ�����ȼ�.ֻ��ʾ�����ȼ�
	*/
	virtual int priority( ) = 0;
	
	/**
	* @brief ����Tip������ģ��.(PS:ͬһ��ģ���Tip,ֻ��ʾ������ȼ���,�ϵ����ȼ����򲻸���ʾ,
			һ��modelnameΪ�����, һ��modulename ���Զ�Ӧ������ȼ���tipInfoModel)
	*/
	virtual void setModuleName( const std::wstring& modulename ) = 0;

	/**
	* @brief ����tip������ģ��
	*/
	virtual const std::wstring& moduleName()const	= 0;
	
	/**
	* @brief ������ʾ��tip����ͼ���url
	*/
	virtual void setIconUrl( const std::wstring& url ) = 0;

	/**
	* @brief ������ʾ��tip����ͼ���url.
	*/
	virtual const std::wstring& iconUrl()const = 0;

	/**
	* @brief ����Tip����ʾ���ı�.
	*/
	virtual void setText( const std::wstring& text ) = 0;
	
	/**
	* @brief ����Tip����ʾ���ı�.
	*/
	virtual const std::wstring& text()const	= 0;

	/**
	* @brief �����Ƿ���Ҫ��tip����ʾ�رհ�ť.
	*/
	virtual void setNeedCloseButton( bool need ) = 0;
	
	/**
	* @brief �����Ƿ���Ҫ��tip����ʾ�رհ�ť.
	*/
	virtual bool needCloseButton() = 0;

	/**
	* @brief ����tip���ı���ʾ����ɫ.
	*/
	virtual void setTextColor( const QColor& color ) = 0;
	
	/**
	* @brief ����tip���ı���ʾ����ɫ.
	*/
	virtual const QColor& textColor()const = 0;
	
	/**
	* @brief ����tip�ı�����ɫ.
	*/
	virtual void setBackgroundColor( const QColor& bkColor, const QColor& topLineColor, const QColor& bottomLineColor ) = 0;
	
	/**
	* @brief ����tip�ı�����ɫ.
	*/
	virtual void backgroundColor( QColor& bkColor, QColor& topLineColor, QColor& bottomLineColor )const = 0;

	/**
	* @brief ����tip�ı�����ɫ.
	*/
	virtual void setAcceptClick( bool accept ) = 0;
	
	/**
	* @brief ����tip�ı�����ɫ.
	*/
	virtual bool acceptClick(  )const = 0;

	/**
	* @brief ������չ����.
	*/
	virtual void setValue( const std::wstring& strValue ) = 0;

	/**
	* @brief ������չ����.
	*/
	virtual const std::wstring& value()const = 0;

	/**
	* @brief �޸���tipinfoModel����Ҫִ��update��֪ͨ���������������model���ݵĲ���
	*/
	virtual void update() = 0;

	typedef xsignals::signal<void (IUnkPtr)>  _SIG_TIP_NOTIFY;
	/**
	* @brief model����Ϣ���º��֪ͨ
	*/
	virtual _SIG_TIP_NOTIFY* getUpdateSignal() = 0;

	/**
	* @brief Tip���������ź�.
	*/
	virtual _SIG_TIP_NOTIFY* getClickedSignal() = 0;

    /**
    * ����������true����ͬһ��λ���Ϲ����ֲ�
    */
    virtual void setRolling(bool rolling) = 0;
    virtual bool isRolling() const = 0;
};