#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiSettingPageBase;
/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
  *IYYUserInfoFrameManagerPtr im_tree_data = root->getServiceInterface<IYYUserInfoFrameManager>(KSERVICECommon);
*/
DWDEFINE_INTERFACE(IYYUserInfoFrameManager): public IUnk
{
/**
   * @brief ��ʾ���ѻ���İ���˵ĸ�����Ϣ��.
   * @param uid �û�uid.
   * @param type Ĭ����ʾ��ҳ������ҳ��0��ʾ"Y����Ϣ"��1��ʾ"������Ϣ".
   * @return.
   */
    virtual void showFrame(UINT64 uid, quint32 type) = 0;

/**
   * @brief ��ʾ���ѻ���İ���˵ĸ�����Ϣ��.
   * @param strArgs ������ʽ{uid:xxx,src:xxx,bigT:xxx}, uid��ʾ�û�uid; src��ʾ��Դ����ʾ����Ӻ���ʱ�ġ�������xxx����;bigT���ڼӺ���ʱͳ�Ƶ�id.
   * @param type Ĭ����ʾ��ҳ������ҳ��0��ʾ"Y����Ϣ"��1��ʾ"������Ϣ".
   * @return.
   */
    virtual void showFrame2(const std::wstring& strArgs, quint32 type) = 0;
};

class DuiWidget;
DWDEFINE_INTERFACE(IYYUserInfoFrameManagerEx): public IDWQComponent
{
	/**
	* @brief ��ʾ���ѻ���İ���˵ĸ�����Ϣ��.
	* @param uid �û�uid.
	* @param type Ĭ����ʾ��ҳ������ҳ��0��ʾ"Y����Ϣ"��1��ʾ"������Ϣ".
	* @return.
	*/
	virtual void showFrame(UINT64 uid, quint32 type) = 0;

	/**
	* @brief ��ʾ���ѻ���İ���˵ĸ�����Ϣ��.
	* @param strArgs ������ʽ{uid:xxx,src:xxx,bigT:xxx}, uid��ʾ�û�uid; src��ʾ��Դ����ʾ����Ӻ���ʱ�ġ�������xxx����;bigT���ڼӺ���ʱͳ�Ƶ�id.
	* @param type Ĭ����ʾ��ҳ������ҳ��0��ʾ"Y����Ϣ"��1��ʾ"������Ϣ".
	*/
	virtual void showFrame2(const std::wstring& strArgs, quint32 type) = 0;

	//�Ժ���Ҫ�Ļ����ٰѵ���userinfoFrame�ӿڿ�����
	enum UserInfoPos
	{
		POS_USERINFO_BEHIND_IMID = 0
	};
	virtual bool insertWidget(UINT32 uid, DuiWidget* widget, UserInfoPos pos) = 0;

    virtual bool insertPage(UINT32 uid, const QString& strTabText, const QString& strIconPath, const QString& strPageName, DuiSettingPageBase* page) = 0;

	/**
	* @brief ���ô����Ͽ�Ƥ��.
	* @param uid �û�uid.
	* @param imagePath ͼƬ����·��
	* @return.
	*/
	virtual void setInfocardImage(quint32 uid, const QString &imagePath) = 0;
};
