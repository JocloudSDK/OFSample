#pragma once

#include <vector>
#include <algorithm>

#define INVALID_NAME_ID	0xFFFF //无效的name id值

extern "C" DWBASE_EXPORTS WORD query_name_id(LPCTSTR pszSrc);

extern "C" DWBASE_EXPORTS WORD insert_name_id(LPCTSTR pszSrc);

extern "C" DWBASE_EXPORTS LPCTSTR query_name(WORD nNameId);

#ifdef _DEBUG
	DWBASE_EXPORTS void	free_string_table(); //测试内存用
#endif

#define DEFINE_NAMEID(x) __declspec(selectany) extern const NAME_ID x = _T(#x)

struct NAME_ID
{
public:
	NAME_ID()
	{
		m_nId = INVALID_NAME_ID;
	}

	NAME_ID(WORD wId)
	{
		m_nId = wId;
	}

	NAME_ID(LPCTSTR pszName)
	{
		m_nId = insert_name_id(pszName);
	}

	NAME_ID(const NAME_ID& oVal)
	{
		m_nId = oVal.m_nId;
	}

	LPCTSTR GetName() const
	{
		return query_name(m_nId);
	}	

	BOOL IsEmpty() const
	{ 
		return (m_nId == INVALID_NAME_ID);
	}

	bool operator == (const NAME_ID& oVal) const
	{
		return (m_nId == oVal.m_nId);
	}

	bool operator != (const NAME_ID& oVal) const
	{
		return (m_nId != oVal.m_nId);
	}

	bool operator > (const NAME_ID &oVal) const
	{
		return (m_nId > oVal.m_nId);
	}

	bool operator >= (const NAME_ID &oVal) const
	{
		return (m_nId >= oVal.m_nId);
	}

	bool operator < (const NAME_ID &oVal) const
	{
		return (m_nId < oVal.m_nId);
	}

	bool operator <= (const NAME_ID &oVal) const
	{
		return (m_nId <= oVal.m_nId);
	}

public:
	WORD m_nId;
};

struct NAME_IDS
{
	void clear()
	{
		m_ids.clear();
	}
	size_t count() const
	{
		return m_ids.size();
	}
	const NAME_ID& item(size_t idx) const
	{
		return m_ids[idx];
	}
	bool contains(const NAME_ID& val) const
	{
		return std::find(m_ids.begin(), m_ids.end(), val)
			!= m_ids.end();
	}
	NAME_IDS& operator << (const NAME_ID& val)
	{
		if (!contains(val))
		{
			m_ids.push_back(val);
			std::sort(m_ids.begin(), m_ids.end());
		}
		return *this;
	}
	bool operator==(const NAME_IDS& rhs) const
	{
		return m_ids == rhs.m_ids;
	}
	bool operator<(const NAME_IDS& rhs) const
	{
		if (m_ids.size() == rhs.m_ids.size())
		{
			for (size_t i = 0; i < m_ids.size(); ++i)
			{
				if (m_ids[i] < rhs.m_ids[i])
					return true;
			}
			return false;
		}
		else
		{
			return m_ids.size() < rhs.m_ids.size();
		}
	}
private:
	std::vector<NAME_ID> m_ids;
};
