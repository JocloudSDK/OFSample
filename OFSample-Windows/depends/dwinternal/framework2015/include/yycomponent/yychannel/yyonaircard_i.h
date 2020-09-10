#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yychannel/yytreeindicatorhandler_i.h"

typedef ITreeIndicatorHandler IYYOnAirCardIndicatorHandler;

class DuiWidget;

DWDEFINE_INTERFACE(IYYOnAirCardDataProvider) : public IDWQComponent
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void queryInfo(const UINT32 uid) = 0;

// signals:
// 	void photoUpdated(quint32 user, const QString &url);
//  void multiPhotoUpdated(quint32 user, const QStringList &url);
};

DWDEFINE_INTERFACE(IYYOnAirCard) : public IDWQComponent
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void registerIndecator(IYYOnAirCardIndicatorHandler *handler) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unregisterIndecator(IYYOnAirCardIndicatorHandler *handler) = 0;
	// This function will take the ownership of widget
	// Widget's life cycle is same to this interface.
	// Please make sure your sizeHint and sizePolicy is right.
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool insertCustomWidget(DuiWidget *widget, int index) = 0;
	// Get Photo geometry, in widget coordinate system.
	// The widget should be inserted by insertCustomWidget or child of a widget that inserted.
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QRectF getPhotoGeometry(DuiWidget *widget) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void registerContentWidget(DuiWidget* widget) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unregisterContentWidget(DuiWidget* widget) = 0;
	//NULL means active default content widget
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void activeContentWidget(DuiWidget* widget) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void registerLeftAreaWidget(DuiWidget* widget) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unregisterLeftAreaWidget(DuiWidget* widget) = 0;
	//NULL means active default left area widget
	//NOTE must active default content widget before calling it
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void activeLeftAreaWidget(DuiWidget* widget) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual IYYOnAirCardDataProviderPtr dataProvider() = 0;

	//Do NOT call unless you know what they'are about to do
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void insertCustomWidgetEx(DuiWidget *widget) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void insertCustomWidgetExContainer(DuiWidget *widget) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void showSettingFrame(QWidget *parent) = 0;

// signals:
	// void currentOnAirChanged(quint32 user);
	//
};
