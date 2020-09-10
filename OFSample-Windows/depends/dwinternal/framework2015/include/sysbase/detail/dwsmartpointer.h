#pragma once

#include "leak_detect.h"
#include "yystd.h"

interface NOVTABLE IWeakRef
{
	virtual void addWeakCount() = 0;
	virtual void releaseWeakCount() = 0;
	virtual ULONG lock() = 0;

#ifdef __LEAKCOUNT_DETECTION
	IWeakRef()
	{
		ADD_LEAK_COUNTER_DETAIL(this);
	}

	virtual ~IWeakRef()
	{
		REMOVE_LEAK_COUNTER_DETAIL(this);
	}
#endif
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//comptr：智能指针模板类
template <class I> class comptr;

template <class I> class __comptr_cr
{
protected:
	I* _p;
private:
	class _noaddRef_I : public I
	{
		virtual void addRef();
		virtual void release();
	};

public:
	//construct/destruct
	__comptr_cr(I* p) : _p(p) {/* if (_p) _p->addRef(); */}
	template <class I2>
	__comptr_cr(const __comptr_cr<I2>& r) : _p(r.get()) { /* if (_p) _p->addRef(); */}
	template <class I2>
	__comptr_cr(const comptr<I2>& r) : _p(r.get()) {/* if (_p) _p->addRef(); */}
	~__comptr_cr() {/* if (_p) _p->release(); */}

	//operator I* (as I*)
	operator I*() const {return _p;}
	//operator -> (as I*)
	_noaddRef_I* operator ->() const {assert(_p); return reinterpret_cast<_noaddRef_I*>(_p);}

	//explicit extract _p (as I*, equivalent to operator I*)
	I* get() const
	{ return _p; }

	////valid
	//bool isNull() const
	//	{ return _p == NULL; }

	//compare operators
	bool operator ==(I* p) const
	{ return _p == p; }
	bool operator !=(I* p) const
	{ return _p != p; }
	bool operator <(I* p) const
	{ return _p < p; }
	bool operator >(I* p) const
	{ return _p > p; }
	bool operator <=(I* p) const
	{ return _p <= p; }
	bool operator >=(I* p) const
	{ return _p >= p; }
};

template <class I> class weakPtr;

template <class I> class comptr
{
protected:
	I* _p;
private:
	class _noaddRef_I : public I
	{
		virtual void addRef();
		virtual void release();
	};

	//only accessible by weakPtr.Lock, p is already addRefed by Lock
	explicit comptr(I *p, IWeakRef* /* lockptr_tag_ */) : _p(p) {}
	friend class weakPtr<I>;

	//#ifdef YY_LINUX
	class _address
	{
		comptr* m_pThis;
		_address(); //disabled
		explicit _address(comptr* pThis) : m_pThis(pThis) {}
		friend class comptr;
	public:
		_address(const _address& r) : m_pThis(r.m_pThis) {}
		_address& operator =(const _address& r) {m_pThis = r.m_pThis; return *this;}
		operator comptr*() const {return m_pThis;}
		operator void*() const {return m_pThis;}
		operator I**() const {ASSERT(! m_pThis->_p); return &m_pThis->_p;}
		operator void**() const {ASSERT(! m_pThis->_p); return (void**)&m_pThis->_p;}
		comptr* _pointer() const {return m_pThis;}
	};
	//#endif

public:
	//construct/destruct
	comptr() : _p(NULL) {}
	comptr(I* p) : _p(p) {if (_p) _p->addRef();}
	comptr(const comptr& r) : _p(r._p) {if (_p) _p->addRef();}
	template <class I2>
	comptr(const comptr<I2>& r) : _p(r.get()) {if (_p) _p->addRef();}
	template <class I2>
	comptr(const __comptr_cr<I2>& r) : _p(r.get()) {if (_p) _p->addRef();}
	~comptr() {if (_p) _p->release();}

	//operator =
	comptr& operator =(I* p) {if (_p != p) {if (_p) _p->release(); _p = p; if (_p) _p->addRef();} return *this;}
	comptr& operator =(const comptr& r) {return operator =(r._p);}
	template <class I2>
	comptr& operator =(const comptr<I2>& r) {return operator =(r.get());}
	template <class I2>
	comptr& operator =(const __comptr_cr<I2>& r) {return operator =(r.get());}

	//attach/detach
	void attach(I* p) {if (_p) _p->release(); _p = p;}
	I* detach() {I* t = _p; _p = NULL; return t;}
	void swap(comptr& r) {I* t = _p; _p = r._p; r._p = t;}

	//operator I* (as I*)
	operator I*() const {return _p;}
	//operator -> (as I*)
	_noaddRef_I* operator ->() const {assert(_p); return reinterpret_cast<_noaddRef_I*>(_p);}

	//operator & (as I**)
	//#ifndef YY_LINUX
	//	I** operator &() {ASSERT(!_p); return &_p;}
	//#else
	_address operator &() {return _address(this);}
	//#endif

	//explicit extract _p (as I*, equivalent to operator I*)
	I* get() const
	{ return _p; }
	//explicit extract _p's address (as I**, equivalent to operator &)
	I** _ppv()
	{ ASSERT(!_p); return &_p; }
	//explicit extract comptr's address (as comptr*)
	comptr* _this()
	{return this;}
	const comptr* _this() const
	{return this;}

	////valid
	//bool isNull() const
	//	{ return _p == NULL; }

	//compare operators
	bool operator ==(I* p) const
	{ return _p == p; }
	bool operator !=(I* p) const
	{ return _p != p; }
	bool operator <(I* p) const
	{ return _p < p; }
	bool operator >(I* p) const
	{ return _p > p; }
	bool operator <=(I* p) const
	{ return _p <= p; }
	bool operator >=(I* p) const
	{ return _p >= p; }
};

template <class I> class weakPtr
{
protected:
	mutable I* _p;
	IWeakRef *_pn;
private:
	void assign(I *p, IWeakRef *pn) {
		if (_p != p) {
			if (_pn)
				_pn->releaseWeakCount();
			_p = p;
			_pn = pn;
			if (_pn) {
				_pn->addWeakCount();
			}
		}
	}

	void assign(I *p) {
		IWeakRef *pn = p ? p->weakPtr() : NULL;
		this->assign(p, pn);
	}

public:
	//construct/destruct
	weakPtr() : _p(NULL), _pn(NULL) {}
	weakPtr(I* p) : _p(p), _pn(p?p->weakPtr():NULL) {if (_pn) _pn->addWeakCount();}
	weakPtr(const weakPtr& r) : _p(r._p), _pn(r._pn) {if (_pn) _pn->addWeakCount();}
	template <class I2>
	weakPtr(const weakPtr<I2>& r) : _p(r._p), _pn(r._pn) {if (_pn) _pn->addWeakCount();}
	template <class I2>
	weakPtr(const comptr<I2>& r) : _p(r._p), _pn(r._p?r._p->weakPtr():NULL) {if (_pn) _pn->addWeakCount();}
	~weakPtr() {if (_pn) _pn->releaseWeakCount();}

	comptr<I> lock() const {
		if(_pn && _pn->lock()) {
			return comptr<I>(_p, _pn);
		} else {
			return comptr<I>(NULL);
		}
	}

	//operator =
	template <class I2>
	weakPtr& operator =(const comptr<I2>& r) {
		this->assign(r.get());
		return *this;
	}
	template <class I2>
	weakPtr& operator =(const __comptr_cr<I2>& r) {
		this->assign(r.get());
		return *this;
	}

	weakPtr& operator =(const weakPtr& r) {
		this->assign(r._p, r._pn);
		return *this;
	}
	template <class I2>
	weakPtr& operator =(const weakPtr<I2>& r) {
		this->assign(r._p, r._pn);
		return *this;
	}

	//compare operators
	bool operator ==(I* p) const
	{ return _p == p; }
	bool operator !=(I* p) const
	{ return _p != p; }
	bool operator <(I* p) const
	{ return _p < p; }
	bool operator >(I* p) const
	{ return _p > p; }
	bool operator <=(I* p) const
	{ return _p <= p; }
	bool operator >=(I* p) const
	{ return _p >= p; }
};

namespace std
{
	template <class I> inline
		void swap(comptr<I>& sp1, comptr<I>& sp2)
	{sp1.swap(sp2);}
}
