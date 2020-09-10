#pragma once

namespace GuildInfoCard
{
	enum ShowPolicy
	{
		MainFrame = 0,
		ChannelHeadLogo,
	};
}

class DuiFrameWindow;

/**
   * @brief ������Ϣ���ӿ�.
   */
DWDEFINE_INTERFACE(IYYGuildInfoCard) : public IUnk
{
/**
   * @brief ��ʾ������Ϣ��.
   * @param channelID Ƶ��id.
   * @param rect widget��frame�����λ��.
   * @param frame frame.
   * @param policy ��ʾ��policy.
   */
	virtual void showInfoCard(quint32 channelID, const QRect& rect, DuiFrameWindow *frame, GuildInfoCard::ShowPolicy policy) = 0;

/**
   * @brief �رչ�����Ϣ��.
   */
	virtual void closeCard() = 0;
};