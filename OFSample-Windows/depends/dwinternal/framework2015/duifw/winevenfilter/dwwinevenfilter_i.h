#pragma once

class DW_DUIFW_EXPORT DWAbstractWinEvenFilter : public QObject
{
	Q_DISABLE_COPY(DWAbstractWinEvenFilter)

public:
	explicit DWAbstractWinEvenFilter(QObject *parent) : QObject(parent){}
	virtual ~DWAbstractWinEvenFilter(){}

	virtual bool winEvent(MSG *message, long *result) = 0;
};