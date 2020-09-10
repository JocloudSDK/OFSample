/*
Notes:
	by wangmingliang 2013/2/27
	yy:909073178
	wangmingliang@yy.com

English:

property:
A property is a wrapper of a certain object.
- features of property:
	- rule: a rule can prevent a property from changing.
	- observer: a observer is actived when property changed.
	note: a rule or a observer should be a object of class
	derived from xsignals::trackable.
- arguments:
	- ValueType:the type of the inner object.
	- Compare:the type of the function object which is used 
	to judge whether the value changed, return true if not.
- members:
	- value_type:the type of the inner object.
	- read_only_type:the read only type of the property
	, through which you can't modify the property.
	- value():return the value of inner object.
	- set_value():set the obejct to a new value.
	- operator const value_type& (): same as value().
	- operator = (): same as set_value().

����:

property:
property �Ƕ�һ������ļ򵥷�װ.
- property ������:
	- rule: rule ������ֹ property ���ı�.
	- observer: observer ���� property ���ı�ʱ����.
	ע��: rule �� observer ��Ӧ�ô� xsignals::trackable ����.
- ����:
	- ValueType:����װ���������.
	- Compare:�������������,�����жϱ���װ�����Ƿ�ı�,û�䷵��true.
- ��Ա:
	- value_type:����װ��������.
	- read_only_type: property ��ֻ������,�����ܸı�property.
	- value():���ر���װ����.
	- set_value():���ö���Ϊ��ֵ.
	- operator const value_type& (): �� value().
	- operator = (): �� set_value().
*/
#pragma once
#include "stable.h"
#include <functional>
#include "biz/biz_types.h"

namespace yyproperty
{

	struct Rule
	{
		Rule()
			:m_refuse(false)
		{
		}
		void refuse() { m_refuse = true; }
		bool refused() { return m_refuse; }
	private:
		bool m_refuse;
	};

	template <typename ValueType>
	class property_read_only
	{
	public:
		typedef ValueType value_type;

		property_read_only(const value_type& value)
			:m_value(value)
		{
		}

		const value_type& value()const
		{
			return m_value;
		}
		
		template <class T,class Res>
		void add_observer(T* observer,Res (T::*mem_fun)()) const
		{
			xsignals_helper::sig_connect(sig_value_changed,observer,mem_fun);
		}

		template <class T,class Res>
		void add_observer(T* observer,Res (T::*mem_fun)(const value_type&)) const
		{
			xsignals_helper::sig_connect(sig_value_changed_to,observer,mem_fun);
		}

		template <class T>
		void remove_observer(T *observer) const
		{
			xsignals_helper::sig_disconnect(sig_value_changed,observer);
			xsignals_helper::sig_disconnect(sig_value_changed_to,observer);
		}

		operator const value_type& () const
		{
			return value();
		}

	protected:	
		typedef typename _def_boost_signals2_signal_type<void (const value_type&, Rule &)>::type	_SIG_TRY_TO_CHANGED_TO;
		_SIG_TRY_TO_CHANGED_TO  sig_try_to_changed_to;

		typedef typename _def_boost_signals2_signal_type<void (const value_type&)>::type	_SIG_VALUE_CHANGED_TO;
		mutable _SIG_VALUE_CHANGED_TO  sig_value_changed_to;

		typedef typename _def_boost_signals2_signal_type<void ()>::type	_SIG_VALUE_CHANGED;
		mutable _SIG_VALUE_CHANGED sig_value_changed;

		value_type m_value;
	};

	template <typename ValueType, typename Compare = std::equal_to<ValueType> >
	class property:public property_read_only<ValueType>
	{
	public:
		typedef property_read_only<value_type> read_only_type;
		property(const value_type& value = value_type())
			:read_only_type(value)
		{
		}

		void set_value(const value_type& value)
		{
			if( !m_cmp(m_value,value))
			{
				Rule rule;
				emit sig_try_to_changed_to(value, rule);
				if(!rule.refused())
				{
					m_value = value;
					emit sig_value_changed();
					emit sig_value_changed_to(m_value);
				}
			}
		}
		
		template <class T,class Res>
		void add_rule(T* ruler,Res (T::*mem_fun)(const value_type &newValue, Rule &canChange))
		{
			xsignals_helper::sig_connect(sig_try_to_changed_to,ruler,mem_fun);
		}

		template <class T>
		void remove_rule(T* ruler)
		{
			xsignals_helper::sig_disconnect(sig_try_to_changed_to,ruler);
		}

		property& operator = (const value_type& value)
		{
			set_value(value);
			return *this;
		}
	private:
		Compare m_cmp;
	};




}