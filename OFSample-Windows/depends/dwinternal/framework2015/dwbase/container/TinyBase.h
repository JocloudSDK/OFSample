/** 
@file
@brief "Tiny"容器内部通用实现
@version 2011-12-20 bobdeng
*/

#pragma once

#include <assert.h>
#include <vector>

#ifndef _NO_MEM_POOL
	#define _MEMORY_MALLOC	mp_alloc
	#define _MEMORY_FREE	mp_free
#else
	#define _MEMORY_MALLOC	malloc
	#define _MEMORY_FREE	free
#endif

template<class Node, UINT BUCKET_SIZE>
class DWTinyBase
{
protected:
	DWTinyBase()
	{
		//校验BUCKET_SIZE
#ifdef _DEBUG
		UINT nBucket = BUCKET_SIZE;
		do
		{
			assert((nBucket & 1) != 0);
			nBucket >>= 1;
		} while (nBucket);
#endif

		m_dwSize = 0;
	}

	explicit DWTinyBase(const DWTinyBase<Node, BUCKET_SIZE> & src)
	{
		copy_from(src);
	}

	~DWTinyBase()
	{
		clear();
	}

	//return说明：
	//	first	如果指定row已经存在，返回值的地址，否则返回新插入的值的地址
	//	second	是否真的插入了
	std::pair<Node *, BOOL> insert(const Node &node)
	{
		std::pair<Node *, BOOL> ret(NULL, FALSE);

		DWORD dwIndex = ((DWORD)node.key) & BUCKET_SIZE;
		
		if (m_vecData.empty())
			m_vecData.resize(BUCKET_SIZE + 1, NULL);

		NOTE_SET *pSet = m_vecData[dwIndex];
		if (!pSet)
		{
			 pSet = new NOTE_SET;
			 pSet->pNodes		= NULL;
			 pSet->dwCount		= 0;
			 pSet->dwReserve	= 0;

			 m_vecData[dwIndex] = pSet;
		}

		UINT nPos = lower_bound(pSet, node.key);
		if (nPos == -1)
		{
			if (!pSet->pNodes || pSet->dwReserve == pSet->dwCount)
			{
				DWORD dwReserve = calc_expand(pSet->dwReserve + 1);
				
				Node *pNewNodes = (Node *)_MEMORY_MALLOC(sizeof(Node) * dwReserve);

				//复制旧的数组
				if (pSet->pNodes)
				{
					memcpy(pNewNodes, pSet->pNodes, sizeof(Node) * pSet->dwCount);
					_MEMORY_FREE(pSet->pNodes);
				}

				pSet->dwReserve = dwReserve;
				pSet->pNodes = pNewNodes;
			}

			//插入新的节点
			pSet->pNodes[pSet->dwCount] = node;
			
			ret.first = &pSet->pNodes[pSet->dwCount];
			ret.second = TRUE;

			pSet->dwCount++;
		}
		else if (pSet->pNodes[nPos].key != node.key)
		{
			assert(pSet->dwCount > nPos);
			
			if (pSet->dwCount == pSet->dwReserve)//空间不够
			{
				DWORD dwReserve = calc_expand(pSet->dwReserve + 1);
				assert(dwReserve > pSet->dwReserve);
				
				Node *pNewNodes = (Node *)_MEMORY_MALLOC(sizeof(Node) * dwReserve);

				//复制旧的前半部分节点到新的数组
				if (nPos > 0)
					memcpy(pNewNodes, pSet->pNodes, sizeof(Node) * nPos);

				//插入新节点
				pNewNodes[nPos] = node;

				//复制旧的后半部分数据到新的数组
				if (nPos < pSet->dwCount)
					memcpy(&pNewNodes[nPos + 1], &pSet->pNodes[nPos], sizeof(Node) * (pSet->dwCount - nPos));

				//释放旧的数组
				_MEMORY_FREE(pSet->pNodes);

				pSet->dwReserve = dwReserve;
				pSet->pNodes = pNewNodes;
				pSet->dwCount++;
				
				ret.first = &pSet->pNodes[nPos];
				ret.second = TRUE;
			}
			else
			{
				assert(pSet->dwCount < pSet->dwReserve);

				//移动数组
				memmove(&pSet->pNodes[nPos + 1], &pSet->pNodes[nPos], sizeof(Node) * (pSet->dwCount - nPos));
				
				//插入新节点
				pSet->pNodes[nPos] = node;
				pSet->dwCount++;

				ret.first = &pSet->pNodes[nPos];
				ret.second = TRUE;
			}
		}
		else
		{
			assert(pSet->pNodes[nPos].key == node.key);

			ret.first = &pSet->pNodes[nPos];
			ret.second = FALSE;
		}

		if (ret.second)
			m_dwSize++;

		return ret;
	}

	template <class Key>
	std::pair<Node, BOOL> erase(Key key)
	{
		std::pair<Node, BOOL> ret;
		ret.second = FALSE;

		DWORD dwIndex = ((DWORD)key) & BUCKET_SIZE;

		NOTE_SET *pSet = NULL;
		if (!m_vecData.empty())
			pSet = m_vecData[dwIndex];
		if (!pSet)
			return ret;

		UINT nPos = lower_bound(pSet, key);
		if (nPos != -1 && pSet->pNodes[nPos].key == key)
		{
			ret.first = pSet->pNodes[nPos];
			ret.second = TRUE;

			//是否数组空闲区域需要收缩
			if (calc_expand(pSet->dwCount - 1) < pSet->dwReserve)
			{				
				Node *pNewNodes = (Node *)_MEMORY_MALLOC(sizeof(Node) * (pSet->dwCount - 1));

				//复制旧的前半部分节点到新的数组
				if (nPos > 0)
					memcpy(pNewNodes, pSet->pNodes, sizeof(Node) * nPos);

				//复制旧的后半部分数据到新的数组
				if (nPos + 1 < pSet->dwCount)
					memcpy(&pNewNodes[nPos], &pSet->pNodes[nPos + 1], sizeof(Node) * (pSet->dwCount - nPos - 1));

				//释放旧的数组
				_MEMORY_FREE(pSet->pNodes);

				pSet->pNodes = pNewNodes;
				pSet->dwCount = pSet->dwReserve = pSet->dwCount - 1;
			}
			else
			{
				if (nPos + 1 < pSet->dwCount)
				{
					memmove(&pSet->pNodes[nPos], &pSet->pNodes[nPos + 1], sizeof(Node) * (pSet->dwCount - nPos - 1));
					//for (UINT i = nPos; i < pSet->dwCount - 1; i++)
					//	*((__int64 *)&pSet->pNodes[i]) = *((__int64 *)&pSet->pNodes[i+1]);
				}

				assert(pSet->dwCount > 0);

				pSet->pNodes[pSet->dwCount - 1] = Node();//清零
				pSet->dwCount--;
			}

			if (pSet->dwCount == 0)
			{
				_MEMORY_FREE(pSet->pNodes);
				pSet->pNodes = NULL;
				pSet->dwReserve = 0;
				pSet = NULL;

				delete m_vecData[dwIndex];
				m_vecData[dwIndex] = NULL;
			}
		}

		if (ret.second)
		{
			m_dwSize--;
			assert(m_dwSize < 0x7FFFFFFF);
		}

		return ret;
	}

	template <class Key>
	std::pair<Node *, BOOL> find(Key key) const
	{
		std::pair<Node *, BOOL> ret(NULL, FALSE);

		DWORD dwIndex = ((DWORD)key) & BUCKET_SIZE;

		NOTE_SET *pSet = NULL;
		if (!m_vecData.empty())
			pSet = m_vecData[dwIndex];
		if (!pSet)
			return ret;

		UINT nPos = lower_bound(pSet, key);
		if (nPos != -1 && pSet->pNodes[nPos].key == key)
		{
			ret.first = &pSet->pNodes[nPos];
			ret.second = TRUE;
		}

		return ret;
	}

	void copy_from(const DWTinyBase<Node, BUCKET_SIZE> & src)
	{
		if (src.m_vecData.empty())
		{
			m_vecData.clear();
			m_dwSize = 0;
		}
		else
		{
			if (m_vecData.empty())
				m_vecData.resize(BUCKET_SIZE + 1, NULL);

			for (UINT i = 0; i < src.m_vecData.size(); i++)
			{
				assert(m_vecData[i] == NULL);

				const NOTE_SET *pSetSrc = src.m_vecData[i];
				if (pSetSrc)
				{
					NOTE_SET *pSetDst = new NOTE_SET;
					pSetDst->dwCount = pSetSrc->dwCount;
					pSetDst->dwReserve = pSetSrc->dwReserve;
					pSetDst->pNodes = (Node *) _MEMORY_MALLOC(sizeof(Node) * pSetSrc->dwReserve);
					memcpy(pSetDst->pNodes, pSetSrc->pNodes, sizeof(Node) * pSetSrc->dwReserve); 

					m_vecData[i] = pSetDst;
				}
			}

			m_dwSize = src.m_dwSize;
		}
	}

	void swap(DWTinyBase<Node, BUCKET_SIZE> & right)
	{
		m_vecData.swap(right.m_vecData);
		
		DWORD dwTemp = m_dwSize;
		m_dwSize = right.m_dwSize;
		right.m_dwSize = dwTemp;
	}

	void clear()
	{
		UINT c_nCount = 0;

		for (UINT i = 0; i < m_vecData.size(); i++)
		{
			NOTE_SET *pSet = m_vecData[i];
			if (pSet)
			{
				c_nCount += pSet->dwCount;

				_MEMORY_FREE(pSet->pNodes);
				pSet->pNodes = NULL;

				delete pSet;
				m_vecData[i] = NULL;
			}
		}

		assert(c_nCount == m_dwSize);
		m_dwSize = 0;
	}

	UINT size() const 
	{
		return m_dwSize;
	}

	struct NOTE_SET
	{
		Node *	pNodes;
		DWORD	dwCount;
		DWORD	dwReserve;
	};

	template<class T>	BOOL check_type_at_compile(T){ /*TinySet/TimyMap只允许基本类型，故意不返回，制造编译错误*/ }
	template<class T>	BOOL check_type_at_compile(T*){ return TRUE; }
	template<>			BOOL check_type_at_compile(__int64){ return TRUE; }
	template<>			BOOL check_type_at_compile(unsigned __int64){ return TRUE; }
	template<>			BOOL check_type_at_compile(int){ return TRUE; }
	template<>			BOOL check_type_at_compile(unsigned int){ return TRUE; }
	template<>			BOOL check_type_at_compile(long){ return TRUE; }
	template<>			BOOL check_type_at_compile(unsigned long){ return TRUE; }
	template<>			BOOL check_type_at_compile(short){ return TRUE; }
	template<>			BOOL check_type_at_compile(unsigned short){ return TRUE; }
	template<>			BOOL check_type_at_compile(char){ return TRUE; }
	template<>			BOOL check_type_at_compile(unsigned char){ return TRUE; }
	template<>			BOOL check_type_at_compile(bool){ return TRUE; }

private:
	//return -1 表示不存在
	template <class Key>
	UINT lower_bound(const NOTE_SET *pSet, Key key) const
	{
		UINT nIndex = -1;

		if (pSet->pNodes && pSet->dwCount > 0)
		{
			if (pSet->dwCount < 64)
			{
				Node *p = pSet->pNodes;
				for (UINT i = 0; i < pSet->dwCount; i++)
				{
					if (p[i].key >= key)
					{
						nIndex = i;
						break;
					}
				}
			}
			else
			{
				UINT i = 0, j = pSet->dwCount - 1;
				while (i + 1 < j)
				{
					UINT pos = (i + j) / 2;
					if (pSet->pNodes[pos].key < key)
						i = pos;
					else
						j = pos;
				}

				if (i == j)
				{
					if (pSet->pNodes[i].key >= key)
						nIndex = i;
				}
				else if (i == j - 1)
				{
					if (pSet->pNodes[i].key >= key)
						nIndex = i;
					else if (pSet->pNodes[j].key >= key)
						nIndex = j;		
				}
				else
				{
					assert(0);
				}
			}
		}

		return nIndex;
	}
	
	inline DWORD calc_expand(DWORD dwNodeCount) const
	{
		UINT dwExpand;

		if (dwNodeCount < 2)
			dwExpand = 2;
		else
			dwExpand = dwNodeCount * 3 / 2;

		return dwExpand;
	}

protected:
	std::vector<NOTE_SET *> m_vecData;
	DWORD					m_dwSize;						
};
