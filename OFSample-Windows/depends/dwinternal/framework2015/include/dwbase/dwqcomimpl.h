#pragma once

#include <QPointer>
#include <QPointer>

#include "dwglobal.h"
#include "dwcomex.h"

#ifdef _DEBUG
    #include "sysbase/detail/util.h"
#endif

template<typename T>
struct DWCOM_NO_VTABLE DWQTComponentImpl: public T
{
public:
	explicit DWQTComponentImpl(QObject* obj = NULL):m_qobject(obj) 
	{
#ifdef _DEBUG
		struct msg
		{
			static void f() {class T_must_be_derived_from_IDWQComponent_msg {} _e; CompileTimeChecker<BaseClassDerivedClass(IDWQComponent, T)> checker(_e);}	
		};
		int n = int(msg::f);(n);
#endif
	}

	void setQObject(QObject *q)
	{
		m_qobject = q;
	}

public:
	virtual bool connectSignal(const char *sigName, const QObject *recv, const char *slotName, const IDWQComponent::ConnectType type)
	{
		if (m_qobject == NULL)
		{
			DW_ASSERT(false);
			return false;
		}
		return QObject::connect(m_qobject, sigName, recv, slotName, static_cast<Qt::ConnectionType>(type));
	}

	virtual bool disconnectSignal(const char *sigName, const QObject *recv, const char *slotName) 
	{
		if (m_qobject == NULL)
		{
			DW_ASSERT(false);
			return false;
		}
		return QObject::disconnect(m_qobject, sigName, recv, slotName);
	}

private:
	virtual bool _callMethod(const char *method, void **args, int num)
	{
		if (m_qobject == NULL)
		{
			DW_ASSERT(false);
			return false;
		}

		Q_UNUSED(num);
		const QMetaObject *meta = m_qobject->metaObject();
		int index = meta->indexOfMethod(method);
		if (index == -1)
		{
			return false;
		}

		m_qobject->qt_metacall(QMetaObject::InvokeMetaMethod, index, args);

		return true;

	}

private:
	QPointer<QObject> m_qobject;
};