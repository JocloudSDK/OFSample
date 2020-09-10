#pragma once

#include <Windows.h>
#include <string.h>

struct DWUUID
{
	DWUUID():m_csz("") {}
	DWUUID(const char *csz):m_csz(csz) {}
	DWUUID(const DWUUID &r):m_csz(r.m_csz) {}
	DWUUID& operator = (const DWUUID &r)
	{
		m_csz = r.m_csz; return *this;
	}

	bool operator == (const DWUUID &r) const
	{
		if (m_csz == r.m_csz)
		{
			return true;
		}

		return strcmp(m_csz, r.m_csz) == 0;
	}

	bool operator != (const DWUUID &r) const
	{
		return !(this->operator ==(r));
	}

	bool operator < (const DWUUID &r) const
	{
		if (m_csz == r.m_csz)
		{
			return false;
		}

		return strcmp(m_csz, r.m_csz) < 0;
	}

	bool operator > (const DWUUID &r) const
	{
		return r.operator < (*this);
	}

	bool operator <= (const DWUUID &r) const
	{
		return !(this->operator >(r));
	}

	bool operator >= (const DWUUID &r) const
	{
		return !(this->operator <(r));
	}

	std::string toString() const
	{
		return std::string(m_csz);
	}

	static DWUUID makeConstInstance(const char *csz)
	{
		DWUUID uuid;
		uuid.m_csz = csz;

		return uuid;
	}

private:
	const char *m_csz;
};

typedef DWUUID	DWCLSID;
typedef const DWUUID&	REFDWUUID;
typedef const DWCLSID&	REFDWCLSID;

const DWUUID DWUUIND_NULL	= DWUUID();