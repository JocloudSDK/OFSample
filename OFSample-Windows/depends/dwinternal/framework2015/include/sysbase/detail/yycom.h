
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwcom.h"
#include "yystd.h"

interface NOVTABLE INothing : IUnk
{
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//CComObjPtr：com对象创建器

using __yycomimp::new_inst;

template <bool INTERLOCK_REF, class MALLOC>
class SharedCount : public IWeakRef
{
	STD_REF<INTERLOCK_REF> use_count;
	STD_REF<INTERLOCK_REF> weak_count;
public:
	SharedCount() : use_count(1), weak_count(1)
	{
	}
	~SharedCount()
	{
	}
	virtual void addWeakCount() {
		weak_count.Inc();
	}
	virtual void releaseWeakCount() {
		if(weak_count.Dec() == 0) {
			this->~SharedCount();
			MALLOC::free(reinterpret_cast<void*>(this));
		}
	}

	ULONG addRef() {
		return use_count.Inc();
	}

	ULONG release() {
		ULONG tmp = use_count.Dec();
		if(tmp == 0) {
			this->releaseWeakCount();
		}
		return tmp;
	}

	virtual ULONG lock() {
		return use_count.Lock();
	}
};

template <class T,
		  bool INTERLOCK_REF = _DEF_COMOBJ_INTERLOCK_REF,					//是否使用线程安全计数？（默认线程安全）
		  class MALLOC = _DEF_COMOBJ_MALLOC<T> >							//指定内存分配器
class CComObjPtr : public comptr<T>
{
	using comptr<T>::_p;

public:
	//construct: 第一个参数必须为new_inst，其后参数与T的构造函数参数表一致
	explicit
	CComObjPtr(__yycomimp::_enum_new_inst) {_p = new (MALLOC::malloc(sizeof(_CComObj))) _CComObj();}
	template <class A1>
	CComObjPtr(__yycomimp::_enum_new_inst, const A1& a1) {_p = new (MALLOC::malloc(sizeof(_CComObj))) _CComObj(a1);}
	template <class A1, class A2>
	CComObjPtr(__yycomimp::_enum_new_inst, const A1& a1, const A2& a2) {_p = new (MALLOC::malloc(sizeof(_CComObj))) _CComObj(a1, a2);}
	template <class A1, class A2, class A3>
	CComObjPtr(__yycomimp::_enum_new_inst, const A1& a1, const A2& a2, const A3& a3) {_p = new (MALLOC::malloc(sizeof(_CComObj))) _CComObj(a1, a2, a3);}
	template <class A1, class A2, class A3, class A4>
	CComObjPtr(__yycomimp::_enum_new_inst, const A1& a1, const A2& a2, const A3& a3, const A4& a4) {_p = new (MALLOC::malloc(sizeof(_CComObj))) _CComObj(a1, a2, a3, a4);}
	template <class A1, class A2, class A3, class A4, class A5>
	CComObjPtr(__yycomimp::_enum_new_inst, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5) {_p = new (MALLOC::malloc(sizeof(_CComObj))) _CComObj(a1, a2, a3, a4, a5);}
	template <class A1, class A2, class A3, class A4, class A5, class A6>
	CComObjPtr(__yycomimp::_enum_new_inst, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6) {_p = new (MALLOC::malloc(sizeof(_CComObj))) _CComObj(a1, a2, a3, a4, a5, a6);}
	template <class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	CComObjPtr(__yycomimp::_enum_new_inst, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7) {_p = new (MALLOC::malloc(sizeof(_CComObj))) _CComObj(a1, a2, a3, a4, a5, a6, a7);}
	template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	CComObjPtr(__yycomimp::_enum_new_inst, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8) {_p = new (MALLOC::malloc(sizeof(_CComObj))) _CComObj(a1, a2, a3, a4, a5, a6, a7, a8);}

	void releaseObject()
		{*(comptr<T>*)(this) = NULL;}

private:
	CComObjPtr(const CComObjPtr&); //d
	void operator =(const CComObjPtr&); //d
	class _CComObj : public T
	{
		typedef SharedCount<INTERLOCK_REF, MALLOC> shared_count_type;
		shared_count_type *_com_base_data;
		_CComObj(const _CComObj&); //d
		void operator =(const _CComObj&); //d

		virtual void addRef() {_com_base_data->addRef();}
		virtual void release() {
			if(_com_base_data->release() == 0) {
				this->~_CComObj();
				MALLOC::free(reinterpret_cast<void*>(this));
			}
		}
		virtual bool queryInterface(REFDWUUID iid, void **ppv) {(iid);(ppv); return false;};
		virtual IWeakRef* weakPtr() {
			return _com_base_data;
		};
		virtual void __disable_new_inst__() {}
#define INIT_SHARED_COUNT() _com_base_data(new (MALLOC::malloc(sizeof(shared_count_type))) shared_count_type())
		_CComObj()
			: T(), INIT_SHARED_COUNT() {}
		_CComObj(const T& r)
			: T(r), INIT_SHARED_COUNT() {}
		template <class A1>
		_CComObj(const A1& a1)
			: T(a1), INIT_SHARED_COUNT() {}
		template <class A1, class A2>
		_CComObj(const A1& a1, const A2& a2)
			: T(a1, a2), INIT_SHARED_COUNT() {}
		template <class A1, class A2, class A3>
		_CComObj(const A1& a1, const A2& a2, const A3& a3)
			: T(a1, a2, a3), INIT_SHARED_COUNT() {}
		template <class A1, class A2, class A3, class A4>
		_CComObj(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
			: T(a1, a2, a3, a4), INIT_SHARED_COUNT() {}
		template <class A1, class A2, class A3, class A4, class A5>
		_CComObj(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
			: T(a1, a2, a3, a4, a5), INIT_SHARED_COUNT() {}
		template <class A1, class A2, class A3, class A4, class A5, class A6>
		_CComObj(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
			: T(a1, a2, a3, a4, a5, a6), INIT_SHARED_COUNT() {}
		template <class A1, class A2, class A3, class A4, class A5, class A6, class A7>
		_CComObj(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7)
			: T(a1, a2, a3, a4, a5, a6, a7), INIT_SHARED_COUNT() {}
		template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
		_CComObj(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8)
			: T(a1, a2, a3, a4, a5, a6, a7, a8), INIT_SHARED_COUNT() {}
#undef INIT_SHARED_COUNT
		friend class CComObjPtr;
	};
};


//类实现多个接口时，使用此宏指定一个主要接口
#define MAIN_INTERFACE(I)	\
		public: I* getThisUnk() {return static_cast<I*>(this);}				\
		public: virtual void addRef() {static_cast<I*>(this)->addRef();}	\
		public: virtual void release() {static_cast<I*>(this)->release();}	\
		public: virtual bool queryInterface(REFDWUUID iid, void **ppv) {(iid);(ppv); return false;};	\
		private: virtual void __disable_new_inst__() = 0;

//当类实现一个接口，又不使用addRef/release进行生命期维护，则可使用此宏
#define NO_addRef_release(I)	\
		public: I* getThisUnk() {return static_cast<I*>(this);}		\
		public: virtual void addRef() {}							\
		public: virtual void release() {}							\
		public: virtual IWeakRef* weakPtr() {return NULL;}						\
		public: virtual bool queryInterface(REFDWUUID iid, void **ppv) {(iid);(ppv); return false;};	\
		private: virtual int __disable_new_inst__() {return 0;}





//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//接口定义宏

#define define_interface(InterfaceName)		\
	interface NOVTABLE InterfaceName;								\
	typedef comptr<InterfaceName>			InterfaceName##Ptr;		\
	typedef /*const InterfaceName##Ptr&*/ __comptr_cr<InterfaceName>	InterfaceName##PtrCR;	\
	interface NOVTABLE InterfaceName

#define typedef_interface(ExistingInterfaceName, NewInterfaceName)	\
	typedef ExistingInterfaceName			NewInterfaceName;		\
	typedef ExistingInterfaceName##Ptr		NewInterfaceName##Ptr;	\
	typedef ExistingInterfaceName##PtrCR	NewInterfaceName##PtrCR

#define nameof_interface(InterfaceName) (#InterfaceName)

define_interface(INothing);

