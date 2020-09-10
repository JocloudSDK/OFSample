#pragma once

#ifdef _DWCOM_DISABLE_NO_VTABLE
#define DWCOM_NO_VTABLE
#else
#define DWCOM_NO_VTABLE	__declspec(novtable)
#endif	//_DWCOM_DISABLE_NO_VTABLE

#include <assert.h>
#include <map>

#include "dwuuid.h"
#include "dwsmartpointer.h"
#include "dwglobal.h"

template<typename Interface>
struct DWInterfaceUUIDBindTraits;

#define DWMAKE_INTERFACE_UUID_NAME(Interface)	(#Interface"_UUID")

#define DWBIND_INTERFACE(Interface, uuidname)	\
template<> struct DWInterfaceUUIDBindTraits<Interface>	\
{	\
	static DWUUID getUUID(void)	\
	{	\
		static DWUUID s_uuid = DWUUID::makeConstInstance(uuidname);	\
		return s_uuid;	\
	}	\
};

#define __dwuuidof(Interface)	(DWInterfaceUUIDBindTraits<Interface>::getUUID())
template<typename Interface> inline DWUUID __dwuuidof2(Interface *) {return __dwuuidof(Interface);}

#define DWDEFINE_INTERFACE_ONLY(Interface)	\
	struct Interface;	\
	typedef comptr<Interface> Interface##Ptr;	\
	typedef __comptr_cr<Interface> Interface##PtrCR;	\
	typedef weakPtr<Interface> Interface##WeakPtr;

#define DWDEFINE_INTERFACE(Interface)	\
	DWDEFINE_INTERFACE_ONLY(Interface)	\
	DWBIND_INTERFACE(Interface, DWMAKE_INTERFACE_UUID_NAME(Interface))	\
	struct DWCOM_NO_VTABLE Interface


struct DwUnkTraits;
DWDEFINE_INTERFACE(IUnk)
{
	virtual void addRef() = 0;
	virtual void release() = 0;
private:
	virtual bool queryInterface(REFDWUUID iid, void **ppv) = 0;
	friend struct DwUnkTraits;
public:
	virtual IWeakRef* weakPtr() = 0;

	template<typename Interface>
	bool queryInterface(comptr<Interface> &ppv) 
	{
		Interface *pInterface = NULL;
		bool b = queryInterface(__dwuuidof2(ppv.get()), (void**)&pInterface);
		if (!b)
		{
			return false;
		}

		ppv.attach(pInterface);

		return true;
	};

	template <typename Signal>
	bool querySignal(Signal * &ppv)
	{
		return queryInterface(__dwuuidof(Signal), (void**)&ppv);
	}

	template <typename Signal>
	Signal *getSignal()
	{
		Signal *s = NULL;
		if (!querySignal(s))
		{
			s = NULL;
		}
		return s;
	}
};

struct DwUnkTraits
{
	static bool proxyQueryInterface(IUnk *pUnk, REFDWUUID iid, void **ppv)
	{
		return pUnk->queryInterface(iid, ppv);
	}
};

#define  DWBEGIN_COM_MAP(cls, mainInterface)	\
	static IUnk * WINAPI getUnKnown(cls *pThis) {return reinterpret_cast<IUnk*>(static_cast<mainInterface*>(pThis));}	\
	static bool WINAPI _innerQueryInterface(cls *pThis, REFDWUUID iid, void **ppv)	\
	{	\
		if (iid == __dwuuidof(IUnk))	\
		{	\
			*ppv = getUnKnown(pThis);	\
		}	\

#define DWCOM_INTERFACE_ENTRY(Interface)	\
		else if (iid == __dwuuidof(Interface))	\
		{	\
			DWSTATIC_CHECK((sizeof(Interface) == 4), binary_compatible_error)	\
			*ppv = static_cast<Interface*>(pThis);	\
		}

#define DWCOM_INTERFACE_ENTRY2(Interface, _1)	\
		else if (iid == __dwuuidof(Interface))	\
		{	\
			DWSTATIC_CHECK((sizeof(Interface) == 4), binary_compatible_error)	\
			*ppv = static_cast<Interface*>(static_cast<_1*>(pThis));	\
		}

#define DWCOM_INTERFACE_ENTRY3(Interface, _1, _2)	\
		else if (iid == __dwuuidof(Interface))	\
		{	\
			DWSTATIC_CHECK((sizeof(Interface) == 4), binary_compatible_error)	\
			*ppv = static_cast<Interface*>(static_cast<_1*>(static_cast<_2*>(pThis)));	\
		}

#define DWCOM_INTERFACE_ENTRY4(Interface, _1, _2, _3)	\
		else if (iid == __dwuuidof(Interface))	\
		{	\
			DWSTATIC_CHECK((sizeof(Interface) == 4), binary_compatible_error)	\
			*ppv = static_cast<Interface*>(static_cast<_1*>(static_cast<_2*>(static_cast<_3*>(pThis))));	\
		}

#define DWCOM_INTERFACE_ENTRY_AGGREGATE(Interface, aggreate)	\
		else if(iid == __dwuuidof(Interface))	\
		{	\
			DWSTATIC_CHECK((sizeof(Interface) == 4), binary_compatible_error)	\
			return pThis->aggreate != NULL ? (DwUnkTraits::proxyQueryInterface(pThis->aggreate,iid, ppv)) : false;	\
		}

#define DWEND_COM_MAP()	\
		else	\
		{	\
			*ppv = NULL;\
			return false;	\
		}	\
			\
		(getUnKnown(pThis))->addRef();\
		return true;	\
	}


namespace DWCOM
{

	//for weak ptr impliment
	class DWWeakRef: public IWeakRef
	{
	public:
		DWWeakRef(IUnk *pCom):m_weakRef(0), m_component(pCom) {}
		void reset() {m_component = NULL;_release();}

	public:
		virtual void addWeakCount(){::InterlockedIncrement((LONG*)&m_weakRef);};
		virtual void releaseWeakCount() 
		{
			::InterlockedDecrement((LONG*)&m_weakRef);
            _release();
		};
		virtual ULONG lock()
		{
			if (m_component != NULL)
			{
				m_component->addRef();
				return 1;
			}

			return 0;
		};

    private:
        void _release()
        {
            assert(m_weakRef >= 0);
            if (m_weakRef == 0 && m_component == NULL)
            {
                delete this;
            }
        }

	private:
		unsigned int m_weakRef;
		IUnk *m_component;
	};
	template<typename T, typename StorageModel>
	struct DWComponentCreator;

	struct DWGlobalStorageModel
	{
		long increment() {return 1;}
		long decrement() {return 1;}
		void setDeletedFlag() {}
	};

	struct DWHeapStorageModel
	{
		DWHeapStorageModel():m_dwRef(0L) {}
		long increment() {return ::InterlockedIncrement((LONG*)&m_dwRef);}
		long decrement() {return ::InterlockedDecrement((LONG*)&m_dwRef);}
		void setDeletedFlag() {m_dwRef = -(LONG_MAX/2);};

	private:
		long m_dwRef;
	};

	template<typename T>
	struct DWComponentCreator<T, DWGlobalStorageModel>
	{
		static T *createInstance(IUnk *pUnk) 
		{
			if (pUnk == NULL)
			{
				static T *s_ = new T(NULL);
				return s_;
			}

			return NULL;
		}
	};

	template<typename T>
	struct DWComponentCreator<T, DWHeapStorageModel>
	{
		static T *createInstance(IUnk *pUnk) 
		{
			return new T(pUnk);
		}
	};

	template<typename Base, typename StorageModel>
	class DwComImpl: public Base, public StorageModel
	{
	public:
		typedef StorageModel ComStorageModel;

	private:
		DwComImpl(const DwComImpl &);
		DwComImpl& operator = (const DwComImpl &);

	public:
		DwComImpl(void * = NULL) 
		{
			__if_exists(Base::_getInnerWeakPtr)
			{
				m_weakPtr = new DWWeakRef(getUnKnown(this));
			}
		}
		virtual ~DwComImpl()
		{
			StorageModel::setDeletedFlag();
			__if_exists(Base::finalRelease)
			{
				finalRelease();
			}
			__if_exists(Base::_getInnerWeakPtr)
			{
				m_weakPtr->reset();
			}
		}

	public:
		HRESULT finalConstruct()
		{
			__if_exists(Base::finalConstruct)
			{
				return Base::finalConstruct();
			}

			return S_OK;
		}

	public:	//IUnk
		virtual void addRef(){StorageModel::increment();};

		virtual void release()
		{
			if (StorageModel::decrement() == 0)
			{
				delete this;
			}
		};

		virtual bool queryInterface(REFDWUUID iid, void **ppv)
		{
			return _innerQueryInterface(this, iid, ppv);
		};

		virtual IWeakRef* weakPtr() 
		{
			__if_exists(Base::_getInnerWeakPtr)
			{
				return m_weakPtr;
			}
			return NULL;
		};
	};

	//Contained必须继承自IUnk
	template<typename Contained>
	class DwComContainedImpl: public Contained
	{
	private:
		DwComContainedImpl(const DwComContainedImpl &);
		DwComContainedImpl& operator = (const DwComContainedImpl &);

	public:	
		DwComContainedImpl(IUnk *outter):m_outterUnKnown(outter) {}
		virtual ~DwComContainedImpl()
		{
			__if_exists(Contained::finalRelease)
			{
				Contained::finalRelease();
			}
		}

	public:
		HRESULT finalConstruct()
		{
			__if_exists(Contained::finalConstruct)
			{
				return Contained::finalConstruct();
			}

			return S_OK;
		}

	public:	//IUnk
		virtual void addRef() {m_outterUnKnown->addRef();}
		virtual void release() {m_outterUnKnown->release();}
		virtual bool queryInterface(REFDWUUID iid, void **ppv)
		{
			bool ret = DwUnkTraits::proxyQueryInterface(m_outterUnKnown, iid, ppv);
			if (ret == false)
			{
				__if_exists(Contained::_innerQueryInterface)
				{
					ret = Contained::_innerQueryInterface(this, iid, ppv);
				}
			}
			return ret;
		}
		virtual IWeakRef* weakPtr() {return NULL;};

	private:
		IUnk *m_outterUnKnown;
	};

	template<typename Base, typename StorageModel>
	class DwComAggImpl: public IUnk, public StorageModel
	{
	public:
		typedef StorageModel ComStorageModel;

	private:
		DwComAggImpl(const DwComAggImpl&);
		DwComAggImpl& operator = (const DwComAggImpl&);

	public:
		DwComAggImpl(IUnk *outter):m_contained(outter)
		{
			__if_exists(Base::_getInnerWeakPtr)
			{
				m_weakPtr = new DWWeakRef(getUnKnown(this));
			}
		};
		virtual ~DwComAggImpl()
		{
			StorageModel::setDeletedFlag();
			__if_exists(Base::_getInnerWeakPtr)
			{
				m_weakPtr->reset();
			}
		}

	public:	
		HRESULT finalConstruct()
		{
			return m_contained.finalConstruct();
		}

	public:	//IUnk
		virtual void addRef() {StorageModel::increment();};

		virtual void release() 
		{
			if (StorageModel::decrement() == 0)
			{
				delete this;
			}
		}

		virtual bool queryInterface(REFDWUUID iid, void **ppv)
		{
			if (ppv == NULL)
			{
				return false;
			}

			*ppv = NULL;
			bool hRet = true;
			if (iid == __dwuuidof(IUnk))
			{
				*ppv = static_cast<IUnk*>(this);
				addRef();
			}
			else
			{
				hRet = Base::_innerQueryInterface(&m_contained, iid, ppv);
			}

			return hRet;
		}
		virtual IWeakRef* weakPtr() 
		{
			__if_exists(Base::_getInnerWeakPtr)
			{
				return m_weakPtr;
			}
			return NULL;
		};

	private:
		DwComContainedImpl<Base> m_contained;
	};

	template<typename T>
	struct DWComCreator
	{
		static HRESULT WINAPI createInstance(IUnk *pUnkOuter, IUnk **ppv)
		{
			if (ppv == NULL)
			{
				return E_POINTER;
			}

			*ppv = NULL;
			HRESULT hRes = S_OK;
			T * p = NULL;
			p = DWComponentCreator<T, T::ComStorageModel>::createInstance(pUnkOuter);
			if (p == NULL)
			{
				return E_OUTOFMEMORY;
			}

			p->addRef();	//防止在finalConstruct被删除
			hRes = p->finalConstruct();

			if (hRes == S_OK)
			{
				if (!p->queryInterface(__dwuuidof(IUnk), (void**)ppv))
				{
					hRes = E_FAIL;
				}
				p->release();	//恢复先前为了防止finalConstruct被删除而增加的引用计数，此时引用计数应该为1
			}

			if (hRes != S_OK)
			{
				*ppv = NULL;
				delete p;
			}

			return hRes;
		}
	};

	template<HRESULT hRes>
	struct DWComFailCreator
	{
		static HRESULT WINAPI createInstance(IUnk *pUnkOuter, IUnk **ppv)
		{
			(pUnkOuter);
			(ppv);
			return hRes;
		}
	};

	template<typename T1, typename T2>
	struct DWComCreator2
	{
		static HRESULT WINAPI createInstance(IUnk *pUnkOuter, IUnk **ppv)
		{
			return (pUnkOuter == NULL) ? T1::createInstance(NULL, ppv) : T2::createInstance(pUnkOuter, ppv);
		}
	};
}

template<bool b, typename T, typename U>
struct DWSelector
{
	typedef T Result;
};

template<typename T, typename U>
struct DWSelector<false, T, U>
{
	typedef U Result;
};

#define DWDECLARE_AGGREGATABLE_EX(cls, heapOrGloabl)	\
	public:	\
	typedef DWCOM::DWComCreator2<DWCOM::DWComCreator<DWCOM::DwComImpl<cls, DWSelector<heapOrGloabl, DWCOM::DWHeapStorageModel, DWCOM::DWGlobalStorageModel>::Result>>,	\
							DWCOM::DWComCreator<DWCOM::DwComAggImpl<cls, DWSelector<heapOrGloabl, DWCOM::DWHeapStorageModel, DWCOM::DWGlobalStorageModel>::Result>>>	\
				_CreatorClass;

#define DWDECLARE_NOT_AGGREGATABLE_EX(cls, heapOrGloabl)	\
	public:	\
		typedef DWCOM::DWComCreator2<DWCOM::DWComCreator<DWCOM::DwComImpl<cls, DWSelector<heapOrGloabl, DWCOM::DWHeapStorageModel, DWCOM::DWGlobalStorageModel>::Result>>,	\
						DWCOM::DWComFailCreator<CLASS_E_NOAGGREGATION>>	\
				_CreatorClass;

#define DWDECLARE_ONLY_AGGREGATABLE_EX(cls, heapOrGloabl)	\
	public:	\
		typedef DWCOM::DWComCreator2<DWCOM::DWComFailCreator<E_FAIL>,	\
						DWCOM::DWComCreator<DWCOM::DwComAggImpl<cls, DWSelector<heapOrGloabl, DWCOM::DWHeapStorageModel, DWCOM::DWGlobalStorageModel>::Result>>>	\
			_CreatorClass;

#define DWDECLARE_AGGREGATABLE(cls)	DWDECLARE_AGGREGATABLE_EX(cls, true)

#define DWDECLARE_NOT_AGGREGATABLE(cls)	DWDECLARE_NOT_AGGREGATABLE_EX(cls, true)
#define DWDECLARE_NOT_AGGREGATABLE_GLOBAL(cls)	DWDECLARE_NOT_AGGREGATABLE_EX(cls, false)

//被聚合的组件不能单件
#define DWDECLARE_ONLY_AGGREGATABLE(cls) DWDECLARE_ONLY_AGGREGATABLE_EX(cls, true)

#define DWSUPPORT_WEAKPTR	\
	protected:	\
		DWCOM::DWWeakRef *m_weakPtr;	\
		DWCOM::DWWeakRef *_getInnerWeakPtr() const {return m_weakPtr;};

namespace DWCOM
{
	struct _DW_CLASSMAP_ENTRY;
}


#ifndef CODE_COVERAGE

#if defined(_M_IA64) || defined(_M_IX86) || defined (_M_AMD64)

/*
链接器的一个强制命名规则会保证数据段在PE文件中按照字母顺序排序；如果数据段名字中有$字母，
链接器会做如下特殊处理：
	1、先将数据段按照字母排序。
	2、删除$字母，以及段名中$以后的所有字符。
	3、合并名字相同的数据段。
这样，就会保证在实际链接后的PE文件中只有一个单独的DWCOM数据段，
且按照DWCOM$__a，DWCOM$__m，DWCOM$__z的顺序排列
*/
#pragma section("DWCOM$__a", read, shared)
#pragma section("DWCOM$__z", read, shared)
#pragma section("DWCOM$__m", read, shared)
extern "C"
{
	__declspec(selectany) __declspec(allocate("DWCOM$__a")) DWCOM::_DW_CLASSMAP_ENTRY* g_classMapEntryFirst = NULL;
	__declspec(selectany) __declspec(allocate("DWCOM$__z")) DWCOM::_DW_CLASSMAP_ENTRY* g_classMapEntryLast = NULL;
}

/*
使得DWCOM开头的几个数据段强制与.rdata节合并
*/
#if !defined(_M_IA64)
#pragma comment(linker, "/merge:DWCOM=.rdata")
#endif

#else

extern "C"
{
	__declspec(selectany) DWCOM::_DW_CLASSMAP_ENTRY* g_classMapEntryFirst = NULL;
	__declspec(selectany) DWCOM::_DW_CLASSMAP_ENTRY* g_classMapEntryLast = NULL;
}

#endif  // defined(_M_IA64) || defined(_M_IX86)

#ifndef DWCLASS_ENTRY_PRAGMA	//此宏的意义是：强制链接器包含一个指向_DW_CLASSMAP_ENTRY的符号

#if defined(_M_IX86)
#define DWCLASS_ENTRY_PRAGMA(class) __pragma(comment(linker, "/include:_g_pClassMap_" #class));
#elif defined(_M_IA64)
#define DWCLASS_ENTRY_PRAGMA(class) __pragma(comment(linker, "/include:g_pClassMap_" #class));
#elif defined(_M_AMD64)
#define DWCLASS_ENTRY_PRAGMA(class) __pragma(comment(linker, "/include:g_pClassMap_" #class));
#else
#error Unknown Platform. define DWCLASS_ENTRY_PRAGMA
#endif

#endif	//DWCLASS_ENTRY_PRAGMA

namespace DWCOM
{
	typedef HRESULT (WINAPI *_DWCREATEINSTANCE)(IUnk *pUnkOuter, IUnk **ppv);

	struct _DW_CLASSMAP_ENTRY
	{
		const char *clsid;
		_DWCREATEINSTANCE creator;
	};
}

#define DWCLASS_MAP_ENTRY(clsid, cls)	\
	__declspec(selectany) DWCOM::_DW_CLASSMAP_ENTRY	\
	g_classMap_##cls	=	\
	{clsid, &cls::_CreatorClass::createInstance	};	\
	extern "C"	__declspec(allocate("DWCOM$__m"))	__declspec(selectany)	DWCOM::_DW_CLASSMAP_ENTRY * const g_pClassMap_##cls = &g_classMap_##cls;\
	DWCLASS_ENTRY_PRAGMA(cls)

#define DWCOM_IMPLEMENT_EX(modifier, functionName)	\
	extern "C" modifier HRESULT __cdecl functionName(const char *clsid, IUnk *pUnkOuter, IUnk **ppv)	\
	{	\
		DWCOM::_DW_CLASSMAP_ENTRY **ppEntry = &g_classMapEntryFirst + 1;	\
		for (; ppEntry < &g_classMapEntryLast; ++ppEntry)	\
		{	\
			DWCOM::_DW_CLASSMAP_ENTRY *pEntry = *ppEntry;	\
			if (pEntry != NULL && DWUUID(pEntry->clsid) == DWUUID(clsid))	\
			{\
				return pEntry->creator(pUnkOuter, ppv);	\
			}\
		}	\
		\
		return E_FAIL;	\
	}

#define DWCOM_IMPLEMENT()	DWCOM_IMPLEMENT_EX(__declspec(dllexport), createComInstance)



#else // CODE_COVERAGE

#ifndef DWCLASS_ENTRY_PRAGMA	//此宏的意义是：强制链接器包含一个指向_DW_CLASSMAP_ENTRY的符号

#if defined(_M_IX86)
#define DWCLASS_ENTRY_PRAGMA(class) __pragma(comment(linker, "/include:_doRegist_" #class));
#elif defined(_M_IA64)
#define DWCLASS_ENTRY_PRAGMA(class) __pragma(comment(linker, "/include:doRegist_" #class));
#elif defined(_M_AMD64)
#define DWCLASS_ENTRY_PRAGMA(class) __pragma(comment(linker, "/include:doRegist_" #class));
#else
#error Unknown Platform. define DWCLASS_ENTRY_PRAGMA
#endif

#endif	//DWCLASS_ENTRY_PRAGMA

namespace DWCOM
{
	typedef HRESULT (WINAPI *_DWCREATEINSTANCE)(IUnk *pUnkOuter, IUnk **ppv);
}

__declspec(selectany) std::map<DWUUID, DWCOM::_DWCREATEINSTANCE> g_sDWComEntryMap;

#define DWCLASS_MAP_ENTRY(clsid, cls) \
	class _Do_Regist_##clsid \
	{ \
	public: \
		_Do_Regist_##clsid() \
		{ \
			g_sDWComEntryMap[clsid] = &cls::_CreatorClass::createInstance; \
		} \
	} ;\
	\
	__declspec(selectany) _Do_Regist_##clsid _Temp_Do_Regist_##clsid; \
	extern "C" __declspec(selectany) _Do_Regist_##clsid* doRegist_##clsid = &_Temp_Do_Regist_##clsid; \
	DWCLASS_ENTRY_PRAGMA(clsid);

#define DWCOM_IMPLEMENT_EX(modifier, functionName)	\
	extern "C" modifier HRESULT __cdecl functionName(const char *clsid, IUnk *pUnkOuter, IUnk **ppv)	\
	{	\
		std::map<DWUUID, DWCOM::_DWCREATEINSTANCE>::iterator it = g_sDWComEntryMap.find(DWUUID(clsid)); \
		if (it != g_sDWComEntryMap.end()) \
		{ \
			return it->second(pUnkOuter, ppv);	\
		} \
		\
		return E_FAIL;	\
	}

#define DWCOM_IMPLEMENT()	DWCOM_IMPLEMENT_EX(__declspec(dllexport), createComInstance)

#endif // CODE_COVERAGE

#define DWCOM_IMPLEMENT2(componentid)	\
	DWCOM_IMPLEMENT_EX(__declspec(dllexport), createComInstance)	\
	const char *KComponentNameUsedInternal__ = componentid;
extern const char*KComponentNameUsedInternal__;