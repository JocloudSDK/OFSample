#pragma once

namespace Data 
{
	class CConditionExpr;
	struct __declspec(novtable) CFilter
	{
		virtual const CConditionExpr * _private() const = 0;
	};
};