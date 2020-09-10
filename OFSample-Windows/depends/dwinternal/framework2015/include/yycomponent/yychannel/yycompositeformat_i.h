#pragma once
#include "yycomponent/yychannellogic/yychannelmessage_i.h" 

DWDEFINE_INTERFACE(ICompositeFormat) : public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void appendText(const QString& text, const QColor& textColor, const QFont& font) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void appendSimpleLink( const QString& linkKey, QString& linkValue, const QString& linkName ) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void appendLink(const QString& linkKey, const QString& linkValue, const QString& linkName ,
		const QColor& linkColor, const QFont& font, bool underline) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void appendImage( const QString& imageKey, const QString& imagePath ) = 0;


	virtual YYMessageData convertToMessageData() = 0;
};
