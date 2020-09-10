#pragma once

#include "StringTable.h"
#include "../variant/Variant.h"
#include "DataCenterPub.h"
#include "TableDataImpl.h"

namespace Data 
{
	class CConditionExpr
	{
	public:
		struct RAW_MATCH_CASE
		{
			E_MATCH_CASE eCase;
			NAME_ID		name;
			DWVariant	val;
		};

		struct RAW_MATCH_NODE
		{	
			//仅叶子节点有效
			std::vector<RAW_MATCH_CASE> vecCase;
			
			//非叶子节点有效
			E_COMBINE_MODE				eMode;					
			std::vector<RAW_MATCH_NODE> vecChild;
		};

		CConditionExpr();
		CConditionExpr(E_COMBINE_MODE eMode);
		~CConditionExpr();	

		void			equal(const NAME_ID & name, const DWVariant & val);
		void			notEqual(const NAME_ID & name, const DWVariant & val);
		void			less(const NAME_ID & name, const DWVariant & val);
		void			greater(const NAME_ID & name, const DWVariant & val);
		void			null(const NAME_ID & name);
		void			notNull(const NAME_ID & name);
		void			modified(const NAME_ID & name);

		void			combine(const CConditionExpr *pExpr);

		BOOL			isEmpty() const { return m_header.eMode == COMBINE_E_NULL; }

		MATCH_NODE*	getMatchTree(const CTableDataImpl::REAL_COLS *pCols) const;
		MATCH_NODE *	convertToMatchTree(const CTableDataImpl::REAL_COLS *pCols) const;
		static void		destroyMatchTree(MATCH_NODE *pHeader);
		
		BOOL			isEqual(const CConditionExpr * pExpr) const;

	private:
		static BOOL		isEqualHelper(const RAW_MATCH_NODE *pLeft, const RAW_MATCH_NODE *pRight);
		static DWORD	calcMatchTreeAllocSize(const RAW_MATCH_NODE *pNode);
		static BOOL		convertToMatchTreeHelper(const CTableDataImpl::REAL_COLS *pCols, 
												 const RAW_MATCH_NODE *pSrcNode, 
												 MATCH_NODE *pDstNode, 
												 BYTE * pBuf,
												 /*[in][out]*/DWORD *pLenUsed);

		void			addCase(const NAME_ID & name, const DWVariant &val, E_MATCH_CASE eCase);
		void			clearCachedMatchTree();


		RAW_MATCH_NODE m_header;
		mutable std::map<void*, MATCH_NODE*> m_pCachedMatchTree;
	};
};