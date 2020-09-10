#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
   * @brief YY ticket interface.
   */
DWDEFINE_INTERFACE( IYYStdTicket ) : public IUnk 
{
	enum TextRole
	{
		NoText = 0,
		Description = 1,
		ScecePlainText = 2,
		CopyText = 3
	};

	enum ImageRole
	{
		NoImage,
		Icon,
		PastedIcon
	};

	enum TicketAction
	{
		NoAction,
		PasteAction	= 1,
		ClickedAction	= 2,
		GetSendDescriptionAction = 3,
		CheckReceiveDescriptionAction = 4,
		WebJoinAction = 5
	};

	enum UseScene
	{
		NoScene,
		ChannelScene,
		ImScene,
		GroupScene,
		WebScene
	};

	enum TicketStyle
	{
		SortStyle,
		LongStyle
	};

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setText(TextRole scene, const std::wstring& description ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setImage(ImageRole scene, const uchar* bytes, UINT32 size  ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setStyle(TicketStyle style) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setEnableCopy( bool canCopy ) = 0;

	typedef _def_boost_signals2_signal_type< void (int scene, int actionType, const std::wstring& data) >::type SIG_TICKET_ACTION;
	virtual SIG_TICKET_ACTION* sigTicketAction() = 0;
};