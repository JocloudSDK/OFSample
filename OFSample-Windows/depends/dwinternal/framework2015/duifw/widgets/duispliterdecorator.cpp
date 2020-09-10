#include "stable.h"

#include "duispliterdecorator.h"

DuiSpliterMediator* g_spliterMediator = NULL;

class DuiSpliterMediatorImpl
{
public:
	void addDecorator(DuiSpliterDecorator *decorator)
	{
		m_lst.push_back(decorator);
	}

	void removeDecorator(DuiSpliterDecorator *decorator)
	{
		for (DECORATOR_LST::iterator it = m_lst.begin(); it != m_lst.end(); ++it)
		{
			if (*it == decorator)
			{
				m_lst.erase(it);
				break;
			}
		}
	}

	void clearAll()
	{
		m_lst.clear();
	}

	bool getPreferredSize(const QString& objectName, int idx, int& w, int& h)
	{
		DuiSpliterDecorator *decorator;
		for (DECORATOR_LST::const_iterator it = m_lst.begin(); it != m_lst.end(); ++it)
		{
			decorator = *it;
			DW_ASSERT(decorator != NULL);
			if (decorator->getPreferredSize(objectName, idx, w, h))
			{
				return true;
			}
		}
		return false;
	}

	bool doPaint(const QString& objectName, int idx, QPainter * painter, const QRect& rect)
	{
		DuiSpliterDecorator *decorator;
		for (DECORATOR_LST::const_iterator it = m_lst.begin(); it != m_lst.end(); ++it)
		{
			decorator = *it;
			DW_ASSERT(decorator != NULL);
			if (decorator->doPaint(objectName, idx, painter, rect))
			{
				return true;
			}
		}
		return false;
	}

private:
	typedef QList<DuiSpliterDecorator*> DECORATOR_LST;
	DECORATOR_LST m_lst;
};

DuiSpliterDecorator::~DuiSpliterDecorator()
{
	DuiSpliterMediator::inst()->removeDecorator(this);
}

bool DuiSpliterDecorator::getPreferredSize(const QString& objectName, int idx, int& w, int& h)
{
	Q_UNUSED(objectName);
	Q_UNUSED(idx);
	Q_UNUSED(w);
	Q_UNUSED(h);
	return false;
}

bool DuiSpliterDecorator::doPaint(const QString& objectName, int idx, QPainter * painter, const QRect& rect)
{
	Q_UNUSED(objectName);
	Q_UNUSED(idx);
	Q_UNUSED(painter);
	Q_UNUSED(rect);
	return false;
}

DuiSpliterMediator* DuiSpliterMediator::inst()
{
	if (g_spliterMediator == NULL)
	{
		g_spliterMediator = new DuiSpliterMediator();
		DuiSpliterWidget::setSpliterPolicy(g_spliterMediator);
	}
	return g_spliterMediator;
}

void DuiSpliterMediator::release()
{
	if (g_spliterMediator != NULL)
	{
		delete g_spliterMediator;
		g_spliterMediator = NULL;
	}
}

DuiSpliterMediator::DuiSpliterMediator()
{
	m_impl = new DuiSpliterMediatorImpl();
}

DuiSpliterMediator::~DuiSpliterMediator()
{
	delete m_impl;
}

void DuiSpliterMediator::addDecorator(DuiSpliterDecorator *decorator)
{
	m_impl->addDecorator(decorator);
}

void DuiSpliterMediator::removeDecorator(DuiSpliterDecorator *decorator)
{
	m_impl->removeDecorator(decorator);
}

void DuiSpliterMediator::clearAll()
{
	m_impl->clearAll();
}

void DuiSpliterMediator::getPreferredSize(const QString& objectName, int idx, int& w, int& h)
{
	m_impl->getPreferredSize(objectName, idx, w, h);
}

bool DuiSpliterMediator::doPaint(const QString& objectName, int idx, QPainter * painter, const QRect& rect)
{
	return m_impl->doPaint(objectName, idx, painter, rect);
}

bool DuiSpliterNoLine::doPaint(const QString& objectName, int idx, QPainter * painter, const QRect& rect)
{
	if (objectName == "showlayout")
	{
		QBrush brush( QColor(252, 254, 255) );
		painter->fillRect(rect, brush);
		return true;	
	}
	return DuiSpliterDecorator::doPaint(objectName, idx, painter, rect);
}