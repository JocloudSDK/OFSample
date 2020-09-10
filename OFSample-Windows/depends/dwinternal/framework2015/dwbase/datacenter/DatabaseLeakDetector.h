#pragma once
#include <map>
#include <sstream>
#include <string>

#include "CriticalSection.h"

namespace Data
{
	enum DETECTOR_E_TYPE
	{
		DETECTOR_E_TYPE_ROW,
		DETECTOR_E_TYPE_ROWEDIT,
		DETECTOR_E_TYPE_TABLEEDIT
	};

	class CDatabaseLeakDetector : public IUnk
	{
	public:
		static CDatabaseLeakDetector * createInstance();

		//IUnk
		virtual void		addRef();
		virtual void		release();
		virtual	IWeakRef*	weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);

		void				InsertPtr(PVOID ptr, DETECTOR_E_TYPE eObjType);
		void				RemovePtr(PVOID ptr);
		void				StartDetector();
	
	private:
		CDatabaseLeakDetector();
		~CDatabaseLeakDetector(void);
		
		static DWORD WINAPI	DetectorThreadProc(void* thiz);
		void				OnDetector() const;

	private:
		DWORD					m_dwRef;

		mutable CCriticalLock	m_csLock;
		DWORD64					m_dw64InsertCount;

		typedef std::map<DWORD, DETECTOR_E_TYPE> LeakDetectorMap;
		LeakDetectorMap		m_mapLeak;
	};


#ifdef _DEBUG
	#define DBLEAK_DETECTOR_CREATE()				m_pDetector = CDatabaseLeakDetector::createInstance()
	#define DBLEAK_DATABASE_DETECTOR(pCtx)			(pCtx)->StartDetector()
	#define DBLEAK_INSERT_ROW(pCtx,  ptr)			(pCtx)->InsertDetectorPtr((ptr), DETECTOR_E_TYPE_ROW)
	#define DBLEAK_INSERT_ROWEDIT(pCtx, ptr) 		(pCtx)->InsertDetectorPtr((ptr), DETECTOR_E_TYPE_ROWEDIT)
	#define DBLEAK_INSERT_TABLEEDIT(pCtx, ptr)		(pCtx)->InsertDetectorPtr((ptr), DETECTOR_E_TYPE_TABLEEDIT)
	#define DBLEAK_REMOVE_PTR(pCtx, ptr)			(pCtx)->RemoveDetectorPtr((ptr))
#else
	#define DBLEAK_DETECTOR_CREATE()		m_pDetector = NULL
	#define DBLEAK_DATABASE_DETECTOR(pCtx)	DW_UNUSED(pCtx)
	#define DBLEAK_INSERT_ROW(pCtx,  ptr)
	#define DBLEAK_INSERT_ROWEDIT(pCtx, ptr)
	#define DBLEAK_INSERT_TABLEEDIT(pCtx, ptr)
	#define DBLEAK_REMOVE_PTR(pCtx, ptr)
#endif



}