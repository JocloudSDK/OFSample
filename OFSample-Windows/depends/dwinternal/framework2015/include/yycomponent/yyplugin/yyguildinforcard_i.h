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
   * @brief 工会信息卡接口.
   */
DWDEFINE_INTERFACE(IYYGuildInfoCard) : public IUnk
{
/**
   * @brief 显示工会信息卡.
   * @param channelID 频道id.
   * @param rect widget在frame的相对位置.
   * @param frame frame.
   * @param policy 显示的policy.
   */
	virtual void showInfoCard(quint32 channelID, const QRect& rect, DuiFrameWindow *frame, GuildInfoCard::ShowPolicy policy) = 0;

/**
   * @brief 关闭工会信息卡.
   */
	virtual void closeCard() = 0;
};