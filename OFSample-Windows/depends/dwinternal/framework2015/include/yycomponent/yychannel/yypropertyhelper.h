
#pragma once
#include "yyproperty.h"
#include "duifw/dwsettings.h"
#include <algorithm>
namespace yyproperty_helper
{
	

	template <typename T>
	struct remove_ref
	{
		typedef T type;
	};

	template <typename T>
	struct remove_ref<T&>
	{
		typedef T type;
	};

	template <typename T>
	struct remove_ref<const T&>
	{
		typedef T type;
	};


	template <typename TransferFun, typename Compare = std::equal_to<value_type> >
	class transfer;

	template<typename Res,typename Arg1,typename Compare>
	class transfer<void (Res&,Arg1),Compare>:public yyproperty::property<Res,Compare>,public xsignals::trackable
	{
	public:
		typedef typename transfer<void (Res&,Arg1),Compare> MyType;
		typedef typename remove_ref<Arg1>::type arg1;
		typedef typename property_read_only<arg1> Property1;

		transfer(Property1& pro1)
			: m_pro1(pro1)
			, m_transfer_fun(0)
		{
			pro1.add_observer(this,&MyType::arg_changed);
		}

		void set_transfer(void (*transfer_fun)(Res&,Arg1))
		{
			sig_transfer_fun.disconnect_all();
			m_transfer_fun = transfer_fun;
		}
		
		template <class T>
		void set_transfer(T * object, void (T::*transfer_fun)(Res&,Arg1))
		{
			sig_transfer_fun.disconnect_all();
			m_transfer_fun = 0;
			xsignals_helper::sig_connect(sig_transfer_fun,object,transfer_fun);
		}

	private:
		void arg_changed()
		{
			Res res;
			if(m_transfer_fun)
			{	
				m_transfer_fun(res,m_pro1.value());
			}
			else
			{				
				sig_transfer_fun(res,m_pro1.value());
			}
			set_value(res);
		}

		Property1& m_pro1;
		typedef typename _def_boost_signals2_signal_type<void (Res&,Arg1)>::type sig_transfer_fun;
		void (*m_transfer_fun)(Res&,Arg1);
	};


	template<typename Res,typename Arg1,typename Arg2,typename Compare>
	class transfer<void (Res&,Arg1,Arg2),Compare>:public yyproperty::property<Res,Compare>,public xsignals::trackable
	{
	public:
		typedef typename transfer<void (Res&,Arg1,Arg2),Compare> MyType;
		typedef typename remove_ref<Arg1>::type arg1;
		typedef typename property_read_only<arg1> Property1;
		typedef typename remove_ref<Arg2>::type arg2;
		typedef typename property_read_only<arg2> Property2;

		transfer(Property1& pro1, Property2& pro2)
			: m_pro1(pro1)
			, m_pro2(pro2)
			, m_transfer_fun(0)
		{
			pro1.add_observer(this,&MyType::arg_changed);
			pro2.add_observer(this,&MyType::arg_changed);
		}

		void set_transfer(void (*transfer_fun)(Res&,Arg1,Arg2))
		{
			sig_transfer_fun.disconnect_all();
			m_transfer_fun = transfer_fun;
		}
		
		template <class T>
		void set_transfer(T * object, void (T::*transfer_fun)(Res&,Arg1,Arg2))
		{
			sig_transfer_fun.disconnect_all();
			m_transfer_fun = 0;
			xsignals_helper::sig_connect(sig_transfer_fun,object,transfer_fun);
		}

	private:
		void arg_changed()
		{
			Res res;
			if(m_transfer_fun)
			{
				m_transfer_fun(res,m_pro1.value(),m_pro2.value());
			}
			else
			{
				sig_transfer_fun(res,m_pro1.value(),m_pro2.value());
			}
			set_value(res);
		}

		Property1& m_pro1;
		Property2& m_pro2;
		typedef typename _def_boost_signals2_signal_type<void (Res&,Arg1,Arg2)>::type sig_transfer_fun;
		void (*m_transfer_fun)(Res&,Arg1,Arg2);
	};


	template<typename Res,typename Arg1,typename Arg2,typename Arg3,typename Compare>
	class transfer<void (Res&,Arg1,Arg2,Arg3),Compare>:public yyproperty::property<Res,Compare>,public xsignals::trackable
	{
	public:
		typedef typename transfer<void (Res&,Arg1,Arg2,Arg3),Compare> MyType;
		typedef typename remove_ref<Arg1>::type arg1;
		typedef typename property_read_only<arg1> Property1;
		typedef typename remove_ref<Arg2>::type arg2;
		typedef typename property_read_only<arg2> Property2;
		typedef typename remove_ref<Arg3>::type arg3;
		typedef typename property_read_only<arg3> Property3;

		transfer(Property1& pro1, Property2& pro2, Property3 &pro3)
			: m_pro1(pro1)
			, m_pro2(pro2)
			, m_pro3(pro3)
			, m_transfer_fun(0)
		{
			pro1.add_observer(this,&MyType::arg_changed);
			pro2.add_observer(this,&MyType::arg_changed);
			pro3.add_observer(this,&MyType::arg_changed);
		}

		void set_transfer(void (*transfer_fun)(Res&,Arg1,Arg2,Arg3))
		{
			sig_transfer_fun.disconnect_all();
			m_transfer_fun = transfer_fun;
		}
		
		template <class T>
		void set_transfer(T * object, void (T::*transfer_fun)(Res&,Arg1,Arg2,Arg3))
		{
			sig_transfer_fun.disconnect_all();
			m_transfer_fun = 0;
			xsignals_helper::sig_connect(sig_transfer_fun,object,transfer_fun);
		}

	private:
		void arg_changed()
		{
			Res res;
			if(m_transfer_fun)
			{
				m_transfer_fun(res,m_pro1.value(),m_pro2.value(),m_pro3.value());
			}
			else
			{
				sig_transfer_fun(res,m_pro1.value(),m_pro2.value(),m_pro3.value());
			}
			set_value(res);
		}
		Property1& m_pro1;
		Property2& m_pro2;
		Property3& m_pro4;
		typedef typename _def_boost_signals2_signal_type<void (Res&,Arg1,Arg2,Arg3)>::type sig_transfer_fun;
		void (*m_transfer_fun)(Res&,Arg1,Arg2,Arg3);
	};


	class IConnection
	{
	public:
		virtual void valueChanged() = 0;
	};

	template <typename ValueType>
	class IValueSource
	{
	public:
		IValueSource()
			:m_connect(0)
		{
		}
		virtual ValueType value() = 0;
		virtual void setValue(const ValueType& value) = 0;
		
		void valueChanged() { m_connect ? m_connect->valueChanged() : 0 ; }
		IConnection *m_connect;
	};


	template<typename ValueType,typename Compare>
	class Connection:public IConnection,public xsignals::trackable
	{
	public:
		Connection(yyproperty::property<ValueType,Compare>& pro, IValueSource<ValueType>& src)
			:m_pro(pro)
			,m_src(src)
		{
			pro.add_observer(this,&Connection::onPropertyChanged);
			src.m_connect = this;
		}
		void onPropertyChanged()
		{
			m_src.setValue(m_pro);
		}
		virtual void valueChanged()
		{
			m_pro = m_src.value();
		}
	private:
		
		yyproperty::property<ValueType,Compare>& m_pro;
		IValueSource<ValueType>& m_src;

	};

	class Connecter
	{
	public:
		enum InitDirection{ Pro_Src, Src_Pro };

		~Connecter()
		{
			dis_connect_all();
		}

		template<typename ValueType,typename Compare>
		void connect(yyproperty::property<ValueType,Compare>& pro, IValueSource<ValueType>& src, InitDirection dir = Pro_Src)
		{
			Connection<ValueType,Compare> * connection = new Connection<ValueType,Compare>(pro,src);
			dir == Pro_Src ? connection->onPropertyChanged() : connection->valueChanged();
			m_connectList.push_back(connection);
		}

		void dis_connect_all()
		{
			std::for_each(m_connectList.begin(), m_connectList.end(), _destroy);
			m_connectList.clear();
		}
	private:
		static void _destroy(IConnection* p) { delete p; }
		std::vector<IConnection*> m_connectList;

	};

}