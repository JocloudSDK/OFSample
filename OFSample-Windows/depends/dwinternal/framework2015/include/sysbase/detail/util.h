#pragma once


template<typename From, typename To>
struct Convertable
{
	struct Big{ char buf[10];};
	typedef char Small;
	static From makeFrom();

	static Small function(To);
	static Big function(...);

	enum {exist = (sizeof(function(makeFrom())) == sizeof(Small))};	
	enum {exist2Way = (exist && Convertable<To,From>::exist)};
	enum {sameType = false};
};

template<typename T>
struct Convertable<T,T>
{
	enum {exist = true, exist2Way = true, sameType = true};
};

template<typename To>
struct Convertable<void, To>
{
	enum {exist = false, exist2Way = false, sameType = false};
};

template<>
struct Convertable<void,void>
{
	enum {exist = true, exist2Way = true, sameType = true};
};


#define BaseClassDerivedClass(baseCls, derivedCls)	\
	(Convertable<const derivedCls*, const baseCls*>::exist &&	\
	!Convertable<const derivedCls*, const void*>::sameType)

#define BaseClassDerivedClass_Strict(baseCls, derivedCls)	\
	(BaseClassDerivedClass(baseCls, derivedCls)	&&	\
	!Convertable<const baseCls, const derivedCls>::sameType)