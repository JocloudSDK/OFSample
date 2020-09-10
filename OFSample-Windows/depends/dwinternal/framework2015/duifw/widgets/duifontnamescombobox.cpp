#include "duifontnamescombobox.h"

bool fontNamesLessThan(const QString &s1, const QString &s2)
{
	if (s1.isEmpty())
	{
		return true;
	}
	else if (s2.isEmpty())
	{
		return false;
	}

	bool bEn1 = s1.at(0).unicode() < 127;
	bool bEn2 = s2.at(0).unicode() < 127;
	if (bEn1 ^ bEn2)
	{
		return !bEn1;
	}
	else
	{
		return s1.toLower() < s2.toLower();
	}
}

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiFontNamesComboBox)

DuiFontNamesComboBox::DuiFontNamesComboBox(QGraphicsItem* parent/* = NULL*/)
:DuiComboBox(parent)
{
	init();
}

DuiFontNamesComboBox::~DuiFontNamesComboBox()
{

}

void DuiFontNamesComboBox::init()
{
	QStandardItemModel* model = new QStandardItemModel();

	QFontDatabase database;

	QStringList stringList = database.families();
	qSort(stringList.begin(), stringList.end(), fontNamesLessThan);

	foreach (QString family, stringList)
	{
		QStandardItem *familyItem = new QStandardItem(family);
		model->appendRow(familyItem);
	}

	setModel(model);
}