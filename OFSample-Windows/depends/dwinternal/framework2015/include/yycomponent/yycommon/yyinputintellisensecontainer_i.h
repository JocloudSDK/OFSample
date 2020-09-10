#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
/**
* @brief ��Ƶ������,Ƶ������,IM,IMȺ,IMȺ����,IM��ʷ��¼����������ݼ��������ݵĽ���ʵ��
*/

/**
* @brief ���ܽ����ӿڣ������ʵ��.
*/
DWDEFINE_INTERFACE(IYYInputIntellisense) : public IUnk
{
	/**
   * @brief ����text�ַ���.
   * @param text ��Ҫ���������ַ���. 
   * @param startPos������ƥ�䵽���ַ�������ʼλ��(��˫�ֽ�wchar_tΪ��λ���ַ�����).
   * @param matchLength������ƥ�䵽���ַ����ĳ���(��˫�ֽ�wchar_tΪ��λ���ַ�����).
   * @return �����ɹ� true�����򷵻�false.
   */
	virtual bool intellisenseText( const std::wstring& text, int& startPos, int& matchLength ) = 0; 
};


/**
* @brief �������ӿڣ�ͨ��root->getServiceInterface<IYYInputIntellisenseContainer>(KSERVICECommonComponentLevel)����ȡ.
*/
DWDEFINE_INTERFACE(IYYInputIntellisenseContainer): public IUnk
{
	//������λ��
	enum WhereIntellisense
	{
		IM = 1,				//IM��Ⱥ�Ĵ���
		IMHistroy = 1 << 1,	//IM��ʷ��¼
		IMBulletin = 1 << 2,//Ⱥ����
		Channel = 1 << 3,   //Ƶ������
		ChannelBulletin = 1 << 4,//Ƶ������
		ErrorPosition,			 //�����λ��,����λ��һ��Ҫ���������ǰ�涨��				 
		ChatContent = IM | Channel,
		Bulletin = IMBulletin | ChannelBulletin,
		ALL = ChatContent | IMHistroy | Bulletin,
	};

	//������ʱ��
	enum WhenIntellisense 
	{
		Default,			//Ĭ��ʱ���Ľ�������ֱ�Ӽӵ����ܽ�������β��		
		BeforeWebLink,		//��webLink��������֮ǰ�����ܽ���[http://www.???]
		BeforeComponentLink,//���������֮ǰ����[yy://???]
		UrlLinkClicked,		//Url���ӱ������ʱ�����������ʱ�������ֻ������textȫ����ƥ���ʱ��Ż���Ӧ����text.length == matchLength, ��IYYInputIntellisense::intellisenseText��
	};

	enum Priority
	{
		PRIORITY_HIGHEST = 0x00,
		PRIORITY_NORMAL_BEFORE = 0x100,
		PRIORITY_NORMAL = 0x200,
		PRIORITY_NORMAL_AFTER = 0x400,
		PRIORITY_NORMAL_LOWEST = 0x800,
	};

/**
   * @brief ������ܽ�������.
   * @param intellisense ���ܽ�������ָ��. 
   * @param priority�����ȼ�--��ͬһ��WhenIntellisense��,�����ȼ������Ƚ��� ��Priority����.
   * @param pos�����������������������е�λ�ã���WhenIntellisense���塡.
   * @param whereNeedIntellisense ����Щ�ط���Ҫ����(��WhereIntellisense������WhereIntellisense��һ������.
   * @return ���intellisense�����Ѵ��ڣ�����false.
   */
	virtual bool addIntellisense( IYYInputIntellisensePtr intellisense, int priority, WhenIntellisense when, int whereNeedIntellisense ) = 0;

	/**
   * @brief ������ܽ�������.
   * @param regexp ���ܽ���������ʽ. 
   * @return ��������ʽ�Ѵ��ڸ�λ�÷���NULL, ���򷵻����ܽ�������ָ��.
   */
	virtual IYYInputIntellisensePtr addIntellisense2( const std::wstring& regexp, int priority, WhenIntellisense when, int whereNeedIntellisense ) = 0;

/**
   * @brief �Ƴ����ܽ�������.
   * @param .
   * @return .
   */
	virtual void removeIntellisense( IYYInputIntellisensePtr intellisense ) = 0;
};