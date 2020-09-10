#pragma once

#include "dwglobal.h"

class DW_DUIFW_EXPORT DuiStandardItem
{
public:
	DuiStandardItem(const QString &text);

protected:
	DuiStandardItem();
	virtual ~DuiStandardItem();

public:
	virtual void release();

	int rowCount() const;
	int row() const;
	void appendRow(DuiStandardItem *item);
	void insertRow(int row, DuiStandardItem *item);
	void removeRow(int row);
	
	bool hasChildren() const;
	DuiStandardItem *takeChild(int row);
	DuiStandardItem *child(int row) const;
	DuiStandardItem *parent() const;

	virtual QString text() const;
	void setText(const QString &text);

private:
	friend class DuiStandardItemPrivate;
	DuiStandardItemPrivate* m_pImpl;
};

class DuiStandardItemModel : public QObject
{
public:
	DuiStandardItemModel();
	~DuiStandardItemModel();

	void appendRow(DuiStandardItem *item);
	void clear();
	DuiStandardItem *item(int row) const;

private:
	QList<DuiStandardItem *> m_items;
};
