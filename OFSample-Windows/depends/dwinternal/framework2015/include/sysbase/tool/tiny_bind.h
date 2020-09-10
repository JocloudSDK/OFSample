#pragma once

#include "../detail/yystd.h"

namespace __tiny_bind_impl
{
	//所有_tiny_bind_data_N的基类
	template <class R>
	class _tiny_bind_data_base
	{
		STD_REF<_DEF_COMOBJ_INTERLOCK_REF> _ref;
		_tiny_bind_data_base(const _tiny_bind_data_base&);
	protected:
		_tiny_bind_data_base() : _ref(1) {}
		virtual ~_tiny_bind_data_base() {}
	public:
		virtual R invoke() const = 0;
	public:
		void addref() {_ref.Inc();}
		void release() {if (_ref.Dec() == 0) delete this;}
	};

	//智能指针
	template <class R>
	class _tiny_bind_data_ptr
	{
		_tiny_bind_data_base<R>* p;
	public:
		explicit _tiny_bind_data_ptr(_tiny_bind_data_base<R>* _p) : p(_p) {}
		_tiny_bind_data_ptr(const _tiny_bind_data_ptr& r) : p(r.p) {p->addref();}
		~_tiny_bind_data_ptr() {p->release();}
		void operator =(const _tiny_bind_data_ptr& r) {p->release(); p = r.p; p->addref();}
		_tiny_bind_data_base<R>* get() const {return p;}
	};

	//函数对象，类似于boost::function0
	template <class R>
	class tiny_function0
	{
		_tiny_bind_data_ptr<R> spBinding;
	public:
		tiny_function0() : spBinding() {}
		tiny_function0(const _tiny_bind_data_ptr<R>& _spBinding) : spBinding(_spBinding) {}
		bool empty() const {return spBinding.get() == NULL;}
		R operator()() const {return spBinding.get()->invoke();}
	};



	//_tiny_bind_data_N，所有_tiny_bind_data_N必须自_tiny_bind_data_base派生
	template <class CP, class R, class MFN, 
			class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	class _tiny_bind_data_9 : public _tiny_bind_data_base<R>
	{
		CP pThis;
		MFN mfn;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
		A5 a5;
		A6 a6;
		A7 a7;
		A8 a8;
		A9 a9;
	public:
		_tiny_bind_data_9(CP _pThis, MFN _mfn, A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6, A7 _a7, A8 _a8, A9 _a9)
			: pThis(_pThis), mfn(_mfn), a1(_a1), a2(_a2), a3(_a3), a4(_a4), a5(_a5), a6(_a6), a7(_a7), a8(_a8), a9(_a9) {}
		virtual R invoke() const {(pThis->*mfn)(a1, a2, a3, a4, a5, a6, a7, a8, a9);}
	};

	template <class CP, class R, class MFN, 
			class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	class _tiny_bind_data_8 : public _tiny_bind_data_base<R>
	{
		CP pThis;
		MFN mfn;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
		A5 a5;
		A6 a6;
		A7 a7;
		A8 a8;
	public:
		_tiny_bind_data_8(CP _pThis, MFN _mfn, A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6, A7 _a7, A8 _a8)
			: pThis(_pThis), mfn(_mfn), a1(_a1), a2(_a2), a3(_a3), a4(_a4), a5(_a5), a6(_a6), a7(_a7), a8(_a8) {}
		virtual R invoke() const {(pThis->*mfn)(a1, a2, a3, a4, a5, a6, a7, a8);}
	};

	template <class CP, class R, class MFN, 
			class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	class _tiny_bind_data_7 : public _tiny_bind_data_base<R>
	{
		CP pThis;
		MFN mfn;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
		A5 a5;
		A6 a6;
		A7 a7;
	public:
		_tiny_bind_data_7(CP _pThis, MFN _mfn, A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6, A7 _a7)
			: pThis(_pThis), mfn(_mfn), a1(_a1), a2(_a2), a3(_a3), a4(_a4), a5(_a5), a6(_a6), a7(_a7) {}
		virtual R invoke() const {(pThis->*mfn)(a1, a2, a3, a4, a5, a6, a7);}
	};

	template <class CP, class R, class MFN, 
			class A1, class A2, class A3, class A4, class A5, class A6>
	class _tiny_bind_data_6 : public _tiny_bind_data_base<R>
	{
		CP pThis;
		MFN mfn;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
		A5 a5;
		A6 a6;
	public:
		_tiny_bind_data_6(CP _pThis, MFN _mfn, A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6)
			: pThis(_pThis), mfn(_mfn), a1(_a1), a2(_a2), a3(_a3), a4(_a4), a5(_a5), a6(_a6) {}
		virtual R invoke() const {(pThis->*mfn)(a1, a2, a3, a4, a5, a6);}
	};

	template <class CP, class R, class MFN, 
			class A1, class A2, class A3, class A4, class A5>
	class _tiny_bind_data_5 : public _tiny_bind_data_base<R>
	{
		CP pThis;
		MFN mfn;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
		A5 a5;
	public:
		_tiny_bind_data_5(CP _pThis, MFN _mfn, A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5)
			: pThis(_pThis), mfn(_mfn), a1(_a1), a2(_a2), a3(_a3), a4(_a4), a5(_a5) {}
		virtual R invoke() const {(pThis->*mfn)(a1, a2, a3, a4, a5);}
	};

	template <class CP, class R, class MFN, 
			class A1, class A2, class A3, class A4>
	class _tiny_bind_data_4 : public _tiny_bind_data_base<R>
	{
		CP pThis;
		MFN mfn;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
	public:
		_tiny_bind_data_4(CP _pThis, MFN _mfn, A1 _a1, A2 _a2, A3 _a3, A4 _a4)
			: pThis(_pThis), mfn(_mfn), a1(_a1), a2(_a2), a3(_a3), a4(_a4) {}
		virtual R invoke() const {(pThis->*mfn)(a1, a2, a3, a4);}
	};

	template <class CP, class R, class MFN, 
			class A1, class A2, class A3>
	class _tiny_bind_data_3 : public _tiny_bind_data_base<R>
	{
		CP pThis;
		MFN mfn;
		A1 a1;
		A2 a2;
		A3 a3;
	public:
		_tiny_bind_data_3(CP _pThis, MFN _mfn, A1 _a1, A2 _a2, A3 _a3)
			: pThis(_pThis), mfn(_mfn), a1(_a1), a2(_a2), a3(_a3) {}
		virtual R invoke() const {(pThis->*mfn)(a1, a2, a3);}
	};

	template <class CP, class R, class MFN, 
			class A1, class A2>
	class _tiny_bind_data_2 : public _tiny_bind_data_base<R>
	{
		CP pThis;
		MFN mfn;
		A1 a1;
		A2 a2;
	public:
		_tiny_bind_data_2(CP _pThis, MFN _mfn, A1 _a1, A2 _a2)
			: pThis(_pThis), mfn(_mfn), a1(_a1), a2(_a2) {}
		virtual R invoke() const {(pThis->*mfn)(a1, a2);}
	};

	template <class CP, class R, class MFN, 
			class A1>
	class _tiny_bind_data_1 : public _tiny_bind_data_base<R>
	{
		CP pThis;
		MFN mfn;
		A1 a1;
	public:
		_tiny_bind_data_1(CP _pThis, MFN _mfn, A1 _a1)
			: pThis(_pThis), mfn(_mfn), a1(_a1) {}
		virtual R invoke() const {(pThis->*mfn)(a1);}
	};

	template <class CP, class R, class MFN>
	class _tiny_bind_data_0 : public _tiny_bind_data_base<R>
	{
		CP pThis;
		MFN mfn;
	public:
		_tiny_bind_data_0(CP _pThis, MFN _mfn)
			: pThis(_pThis), mfn(_mfn) {}
		virtual R invoke() const {return (pThis->*mfn)();}
	};



	//各版本tiny_bind
	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4, class B5, class B6, class B7, class B8, class B9, 
			class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4,B5,B6,B7,B8,B9), C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6, A7 _a7, A8 _a8, A9 _a9)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_9<C*, R, R (CC::*)(B1,B2,B3,B4,B5,B6,B7,B8,B9), 
													A1, A2, A3, A4, A5, A6, A7, A8, A9>(_pThis, _mfn, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4, class B5, class B6, class B7, class B8, 
			class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4,B5,B6,B7,B8), C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6, A7 _a7, A8 _a8)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_8<C*, R, R (CC::*)(B1,B2,B3,B4,B5,B6,B7,B8), 
													A1, A2, A3, A4, A5, A6, A7, A8>(_pThis, _mfn, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4, class B5, class B6, class B7,  
			class A1, class A2, class A3, class A4, class A5, class A6, class A7> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4,B5,B6,B7), C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6, A7 _a7)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_7<C*, R, R (CC::*)(B1,B2,B3,B4,B5,B6,B7), 
													A1, A2, A3, A4, A5, A6, A7>(_pThis, _mfn, _a1, _a2, _a3, _a4, _a5, _a6, _a7));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4, class B5, class B6,  
			class A1, class A2, class A3, class A4, class A5, class A6> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4,B5,B6), C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_6<C*, R, R (CC::*)(B1,B2,B3,B4,B5,B6), 
													A1, A2, A3, A4, A5, A6>(_pThis, _mfn, _a1, _a2, _a3, _a4, _a5, _a6));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4, class B5,  
			class A1, class A2, class A3, class A4, class A5> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4,B5), C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_5<C*, R, R (CC::*)(B1,B2,B3,B4,B5), 
													A1, A2, A3, A4, A5>(_pThis, _mfn, _a1, _a2, _a3, _a4, _a5));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4,   
			class A1, class A2, class A3, class A4> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4), C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_4<C*, R, R (CC::*)(B1,B2,B3,B4), 
													A1, A2, A3, A4>(_pThis, _mfn, _a1, _a2, _a3, _a4));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3,   
			class A1, class A2, class A3> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3), C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_3<C*, R, R (CC::*)(B1,B2,B3), 
													A1, A2, A3>(_pThis, _mfn, _a1, _a2, _a3));
	}

	template <class CC, class R, class C, 
			class B1, class B2,   
			class A1, class A2> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2), C* _pThis, 
								A1 _a1, A2 _a2)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_2<C*, R, R (CC::*)(B1,B2), 
													A1, A2>(_pThis, _mfn, _a1, _a2));
	}

	template <class CC, class R, class C, 
			class B1,   
			class A1> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1), C* _pThis, 
								A1 _a1)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_1<C*, R, R (CC::*)(B1), 
													A1>(_pThis, _mfn, _a1));
	}

	template <class CC, class C, class R> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(), C* _pThis)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_0<C*, R, R (CC::*)()>(_pThis, _mfn));
	}


	//支持const成员函数
	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4, class B5, class B6, class B7, class B8, class B9, 
			class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4,B5,B6,B7,B8,B9) const, const C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6, A7 _a7, A8 _a8, A9 _a9)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_9<const C*, R, R (CC::*)(B1,B2,B3,B4,B5,B6,B7,B8,B9) const, 
													A1, A2, A3, A4, A5, A6, A7, A8, A9>(_pThis, _mfn, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4, class B5, class B6, class B7, class B8, 
			class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4,B5,B6,B7,B8) const, const C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6, A7 _a7, A8 _a8)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_8<const C*, R, R (CC::*)(B1,B2,B3,B4,B5,B6,B7,B8) const, 
													A1, A2, A3, A4, A5, A6, A7, A8>(_pThis, _mfn, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4, class B5, class B6, class B7,  
			class A1, class A2, class A3, class A4, class A5, class A6, class A7> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4,B5,B6,B7) const, const C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6, A7 _a7)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_7<const C*, R, R (CC::*)(B1,B2,B3,B4,B5,B6,B7) const, 
													A1, A2, A3, A4, A5, A6, A7>(_pThis, _mfn, _a1, _a2, _a3, _a4, _a5, _a6, _a7));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4, class B5, class B6,  
			class A1, class A2, class A3, class A4, class A5, class A6> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4,B5,B6) const, const C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5, A6 _a6)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_6<const C*, R, R (CC::*)(B1,B2,B3,B4,B5,B6) const, 
													A1, A2, A3, A4, A5, A6>(_pThis, _mfn, _a1, _a2, _a3, _a4, _a5, _a6));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4, class B5,  
			class A1, class A2, class A3, class A4, class A5> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4,B5) const, const C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4, A5 _a5)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_5<const C*, R, R (CC::*)(B1,B2,B3,B4,B5) const, 
													A1, A2, A3, A4, A5>(_pThis, _mfn, _a1, _a2, _a3, _a4, _a5));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3, class B4,   
			class A1, class A2, class A3, class A4> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3,B4) const, const C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3, A4 _a4)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_4<const C*, R, R (CC::*)(B1,B2,B3,B4) const, 
													A1, A2, A3, A4>(_pThis, _mfn, _a1, _a2, _a3, _a4));
	}

	template <class CC, class R, class C, 
			class B1, class B2, class B3,   
			class A1, class A2, class A3> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2,B3) const, const C* _pThis, 
								A1 _a1, A2 _a2, A3 _a3)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_3<const C*, R, R (CC::*)(B1,B2,B3) const, 
													A1, A2, A3>(_pThis, _mfn, _a1, _a2, _a3));
	}

	template <class CC, class R, class C, 
			class B1, class B2,   
			class A1, class A2> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1,B2) const, const C* _pThis, 
								A1 _a1, A2 _a2)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_2<const C*, R, R (CC::*)(B1,B2) const, 
													A1, A2>(_pThis, _mfn, _a1, _a2));
	}

	template <class CC, class R, class C, 
			class B1,   
			class A1> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)(B1) const, const C* _pThis, 
								A1 _a1)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_1<const C*, R, R (CC::*)(B1) const, 
													A1>(_pThis, _mfn, _a1));
	}

	template <class CC, class C, class R> inline 
	tiny_function0<R> tiny_bind(R (CC::*_mfn)() const, const C* _pThis)
	{
		return _tiny_bind_data_ptr<R>(new _tiny_bind_data_0<const C*, R, R (CC::*)() const>(_pThis, _mfn));
	}

} //namespace __tiny_bind_impl



//公开以下类/函数
using __tiny_bind_impl::tiny_function0;
using __tiny_bind_impl::tiny_bind;
