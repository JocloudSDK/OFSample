#pragma once

enum JumpFrom
{
	JumpFrom_YYMain,
	JumpFrom_Channel,
	JumpFrom_Message,
	JumpFrom_GameTool,
	JumpFrom_Web,
	JumpFrom_IM,
	JumpFrom_banner,
	JumpFrom_tabApp,
	NoUse,
	jumpFrom_Shortcut
};

enum OpenProxy
{
	IPCWindowHostWidget,
	GameProxy,
};

struct UrlData;

DWDEFINE_INTERFACE(IYYAppCommonFrame) : public IDWQComponent
{
	virtual void showPrivilegeDialog(const QString& strRedirect, const QString& ticket) = 0;
	virtual void closePrivilegeDialog() = 0;

	virtual void showGameChargeDialog(const QString& strRedirect, JumpFrom jump, quint32 channelID) = 0;

	virtual void setPrivilegeRedirect(const QString& redirect) = 0;
	virtual void setPrivilegeOpenProxy(OpenProxy proxy) = 0;

	virtual quint32 nativeWindowId() = 0;

	// �򿪷���
	virtual void showDiscovery(UrlData& urlData, const QString& data) = 0;

	// ����������
	virtual QVariant query(const QString &searchString) = 0;

	// ģ�´���������Ƶ��
	virtual void joinChannel(quint32 sid) = 0;

	// ����������ҳ�棬keyĬ��Ϊ���ַ���
	virtual void openSearchPage(const QString& pageName, const QString& key) = 0;
};
