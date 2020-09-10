#include "stable.h"

#include "duitextutility.h"

void duitextutils::formatName( QString& text )
{
	text = singleLineTextFilter( text );
}

QString duitextutils::singleLineTextFilter( const QString& text )
{
	QString modifiedText = text.trimmed();
	modifiedText.replace("\t", " ");

	static unsigned short s_BreakChars[] = {
    1, 10, 11, 12, 13, 
	1608, 1711, 
    8232, 8233, 8238, 
    61451, 61452, 61453, 0xfffc, 0xfeff };

	for(unsigned short i=0; i<DW_ARRAY_SIZE(s_BreakChars); ++i)
	{
		modifiedText.remove(QChar(s_BreakChars[i]));
	}

	return multiLineTextFilter(modifiedText);
}

void duitextutils::removeNewLine( QString& text )
{
	text.remove(QChar(10));
	text.remove(QChar(13));
}

bool duitextutils::fixupStringByBytes( QString& addText, int remainBytes, int pos /* = -1*/ )
{
	if ( remainBytes > 0 )
	{
		int index = 0;
		int bytes = 0;

		for ( ; index < addText.size(); ++index )
		{
			bytes += addText[index].unicode() < 128 ? 1 : 2;
			if ( bytes > remainBytes )
			{
				if (-1 != pos)
				{
					addText.remove(pos - 1, 1);
				}

				break;
			}
		}

		if ( bytes <= remainBytes )
		{
			return false;
		}
		addText = addText.left(index);
	}
	else
	{
		addText.clear();
	}
	return true;
}

int duitextutils::measureStringByBytes( QString& text )
{
	int bytes = 0;

	for ( int index = 0; index < text.size(); ++index )
	{
		bytes += text[index].unicode() < 128 ? 1 : 2;
	}

	return bytes;
}


//------------------------------------------------------------------------------
/**
可以提供每个状态的Default值, 
如果没有提供Default ，states中又没有包含相应的状态，则会返回一个空状态
*/
QStringList duitextutils::resolvingStates(const QString& states, 
								   const QString& stateNormal/*=""*/, 
								   const QString& stateHover/*=""*/, 
								   const QString& statePressDown/*=""*/, 
								   const QString& stateDisable/*=""*/, 
								   const QString& stateCheckOrFocus/*=""*/)
{
	QStringList images = states.split('|');
	switch (images.count())
	{
	case 0:
		images.append(stateNormal);
	case 1:
		images.append(stateHover);
	case 2:
		images.append(statePressDown);
	case 3:
		images.append(stateDisable);
	case 4:
		images.append(stateCheckOrFocus);
	default:
		;
	}
	return images;
}

QString duitextutils::multiLineTextFilter( const QString& text )
{
	QString modifiedText = text;
    // 0x0787 : Arab character, ALIFU
	// 0xFDF2 : Arab character, 上帝
    // 0x200E : LRM , Left To Right Mark
    // 0x200F : RLM , Right To Left Mark
    // 0xD800 - 0xDBFF : High-half zone of UTF-16
    // 0xDC00 - 0xDFFF: Low-half zone of UTF-16 
    // 0x202A - 0x202E Left to Right and Right to Left , Embedding/Overriding/Formating
	// 0x0E31 0x05E4 : Layouts: Thai (th) 
	// 0x0793 0x0799 x0788: Layouts: Divehi (dv)
	// 0xFEEC : Layouts: Persian (fa)
	// 0x0332 : Combining Low Line
	// 0x032F : Combining Inverted Breve Below
	// 0x0361 : Combining Diacritical Marks
	// 0x06AA : KAF 希伯莱语的第十一个字母(相当于英语中的k)
	
	static QRegExp reg(QString::fromWCharArray(L"[\x0787\x0788\x200F\xFDF2\x0E31\
\x05E4\x0332\x032F\x0793\x0799\xFEEC\x06AA\x0361]\
|[\xD800-\xDFFF]|[\x202A-\x202E]"));
    modifiedText.remove(reg);

	return modifiedText;
}
