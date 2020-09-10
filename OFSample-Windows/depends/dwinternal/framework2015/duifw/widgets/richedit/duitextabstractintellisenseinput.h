#pragma once
#include "dwglobal.h"

const int KHandledProperty = 10000;

class  InterllisenseInputData
{
public:
	InterllisenseInputData():isHandle(false){}
public:
	bool isHandle;
};

class DuiTextCursor;

class DuiTextAbstractIntellisenseInput : public QObject
{
public:
	explicit DuiTextAbstractIntellisenseInput(QObject* parent) 
		: QObject(parent)
		, m_nextChain(NULL)
		, m_isEnable(true)
	{

	}

	// handle inputing.
	virtual void intellisenseInputOneChar( InterllisenseInputData &data )
	{
		if ( m_nextChain )
		{
			m_nextChain->intellisenseInputOneChar(data);
		}
	}

	virtual void intellisenseInputText( int position, int addNumbers, int removedNumbers )
	{
		if ( m_nextChain )
		{
			m_nextChain->intellisenseInputText(position, addNumbers, removedNumbers);
		}
	}

	// handle read only.
	virtual void intellisenseInputText( const QString &text, DuiTextCursor &tc, const QTextCharFormat& charFormat)
	{
		if ( !m_isEnable && m_nextChain )
		{
			m_nextChain->intellisenseInputText(text, tc, charFormat);
			return;
		}

		int matchIndex = 0;
		int unmatchStartPos = 0;

		while(( matchIndex = m_regexp.indexIn(text, matchIndex)) != -1)
		{
			if( unmatchStartPos < matchIndex && nextChain())
			{
				m_nextChain->intellisenseInputText(text.mid(unmatchStartPos, matchIndex - unmatchStartPos), tc, charFormat);
				unmatchStartPos = matchIndex;
			}

			matchIndex += m_regexp.matchedLength();
			if ( onMatch(tc, charFormat) )
			{
				unmatchStartPos = matchIndex;
			}
		}

		if( text.length() > unmatchStartPos && m_nextChain )
		{
			m_nextChain->intellisenseInputText(text.mid(unmatchStartPos, text.length() - unmatchStartPos), tc, charFormat);
		}
	}
	
	virtual QString translateToText( int position, InterllisenseInputData &data )
	{
		Q_UNUSED(position);
		Q_UNUSED(data);
		return QString();
	}

	//true : if handled.
	//false: translate to next.
	virtual bool onMatch( DuiTextCursor&, const QTextCharFormat&)
	{
		return false;
	}

    void setEnable( bool bEnabled )
    {
        m_isEnable = bEnabled; 
    }

    bool isEnable() const 
	{ 
		return m_isEnable; 
	}

	void setNextChain( DuiTextAbstractIntellisenseInput *nextChain )
	{
		m_nextChain = nextChain;
	}

	DuiTextAbstractIntellisenseInput* nextChain()
	{
		return m_nextChain;
	}

	// 当插入非文字对象时，必须调用这个接口.
	//void setMatchedSize( QTextCharFormat& format, int len )
	//{
	//	format.setProperty(100, len);
	//}

protected:
	QRegExp			m_regexp;

private:	
	DuiTextAbstractIntellisenseInput *m_nextChain;
    bool			m_isEnable;
};