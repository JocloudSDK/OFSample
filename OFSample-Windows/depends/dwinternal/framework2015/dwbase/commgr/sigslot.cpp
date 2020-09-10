#include "stable.h"
#include "sysbase\tool\sigslot.h"

namespace sigslot 
{
	has_slots::slot_wrapper::slot_wrapper(has_slots *pObject)
		:m_object(pObject), m_refcount(0)
	{
		ADD_LEAK_COUNTER_DETAIL(this);
	}

	has_slots::slot_wrapper::~slot_wrapper()
	{
		REMOVE_LEAK_COUNTER_DETAIL(this);
	}

	has_slots* has_slots::slot_wrapper::get() const
	{
		return m_object;
	}

	long has_slots::slot_wrapper::addref()
	{
		return ::InterlockedIncrement(&m_refcount);
	}

	long has_slots::slot_wrapper::release()
	{
		long val = ::InterlockedDecrement(&m_refcount);
		if(0 == val)
		{
			delete this;
		}
		return val;
	}

	has_slots::slot_holder::slot_holder(has_slots::slot_wrapper *target)
		:m_target(target)
	{
		if(m_target)
		{
			m_target->addref();
		}
	}

	has_slots::slot_holder::slot_holder(const has_slots::slot_holder &rhs)
		:m_target(rhs.m_target)
	{
		if(m_target)
		{
			m_target->addref();
		}
	}

	has_slots::slot_holder::~slot_holder()
	{
		if(m_target)
		{
			m_target->release();
		}
	}

	has_slots::slot_holder& has_slots::slot_holder::operator=(has_slots::slot_wrapper *target)
	{
		if(target != m_target)
		{
			if(m_target)
			{
				m_target->release();
			}
			m_target = target;
			if(m_target)
			{
				m_target->addref();
			}
		}
		return *this;
	}

	has_slots::slot_holder& has_slots::slot_holder::operator=(const has_slots::slot_holder &rhs)
	{
		return (*this) = rhs.m_target;
	}

	has_slots::slot_holder::operator has_slots::slot_wrapper*()
	{
		return m_target;
	}

	has_slots::slot_wrapper* has_slots::slot_holder::operator->() const
	{
		return m_target;
	}


	has_slots::slot_holder::operator const has_slots::slot_wrapper*() const
	{
		return m_target;
	}

	has_slots::has_slots()
		:receiever(new slot_wrapper(this))
	{
	}

	has_slots::~has_slots()
	{
		receiever->m_object = NULL;
	}

	const has_slots::slot_holder& has_slots::get_receiver()
	{
		return receiever;
	}

	void has_slots::disconnect_all() 
	{
		receiever->m_object = NULL;
		receiever = new slot_wrapper(this);
	}

	_connection_base::_connection_base()
		:m_pobject(), m_emitter(NULL)
	{
	}

	_connection_base::_connection_base(const has_slots::slot_holder &slot, void *emitter)
		:m_pobject(slot), m_emitter(emitter)
	{
	}

	_connection_base::_connection_base(const _connection_base&rhs)
		:m_pobject(rhs.m_pobject), m_emitter(rhs.m_emitter)
	{
	}

	_connection_base::~_connection_base()
	{
		int i = 0;
		i++;
	}

	has_slots* _connection_base::getdest() const
	{
		return m_pobject->get();
	}

	void* _connection_base::get_emitter() const
	{
		return m_emitter;
	}

	_connection_base::connection_holder::connection_holder(const _connection_base *conn)
		:m_conn(const_cast<_connection_base*>(conn)) {}
	_connection_base::connection_holder::connection_holder(const connection_holder &rhs)
		:m_conn(rhs.m_conn->clone()) {}
	_connection_base::connection_holder::~connection_holder()
	{
		m_conn->_connection_base::~_connection_base();
		operator delete(m_conn);
	}
	_connection_base* _connection_base::connection_holder::operator->()
	{
		return m_conn;
	}

	const _connection_base* _connection_base::connection_holder::operator->() const
	{
		return m_conn;
	}

	_connection_base::connection_holder::operator _connection_base*()
	{
		return m_conn;
	}

	_connection_base::connection_holder::operator const _connection_base*() const
	{
		return m_conn;
	}

}