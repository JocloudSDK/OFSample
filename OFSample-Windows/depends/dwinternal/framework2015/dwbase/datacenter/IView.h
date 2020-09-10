#pragma once

#include "ITable.h"

namespace Data
{
	//视图接口
	interface NOVTABLE IView : IUnk
	{
		//可以多次改变Filter，每次修改Filter会发出Reset事件
		//请不要滥用Filter，可能会导致其他使用view用户输出不被期望的结果
		//如果仅仅用做查询，请使用ITable的select方法。
		virtual void				setFilter(const CFilter *pFilter) = 0;

		//可以指定的监控一个或者多个列修改（默认监控全部的列）
		virtual void				setChangedEventFilter(const NAME_ID *pColNames, UINT nColCount) = 0;

		virtual IRowPtr				row(const KEY &key) = 0;
		virtual BOOL				hasRow(const KEY &key) = 0;
		
		//如果结果为空，返回包含空内容的Array（不是NULL）
		virtual IRowArrayPtr		all() = 0;
						
		virtual const NAME_ID		getViewName() = 0;
		virtual ITablePtr			getOwner() = 0;

		//事件
		//!!禁止UI连接下面一组信号
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigAddList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigDelList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigChangedList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigReset() = 0;
		
		virtual xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> & getSigChangedListEx() = 0;

		//用于排除某些属性列
		virtual void				setFilterExcept(const NAME_IDS &pColNames) = 0;
	};
	typedef comptr<IView> IViewPtr;
};
