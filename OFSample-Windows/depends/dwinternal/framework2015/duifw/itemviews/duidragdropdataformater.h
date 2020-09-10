#pragma once

class DuiHPTreeItem;

struct DW_DUIFW_EXPORT DuiDragDropDataFormater
{
	virtual QMimeData* makeMimeData(const QList<QStandardItem*>& modelItems) = 0;
};

struct DW_DUIFW_EXPORT DuiHpTreeDragDropDataFormater
{
	virtual QMimeData* makeMimeData(const QList<DuiHPTreeItem*>& modelItems) = 0;
};