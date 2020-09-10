#pragma once

#include "AtlHelper.h"

#ifndef _NO_MEM_POOL
#	include "dwbase/MemPool.h"
#else
#	include <malloc.h>
#endif

extern "C"
{
	LONG __cdecl _InterlockedIncrement( LONG volatile * pn );
	LONG __cdecl _InterlockedDecrement( LONG volatile * pn );
};

#pragma intrinsic( _InterlockedIncrement )
#pragma intrinsic( _InterlockedDecrement )

namespace ATL_STR
{

#ifdef _M_IX86
#ifndef _M_CEE
#define _AtlInterlockedIncrement _InterlockedIncrement
#define _AtlInterlockedDecrement _InterlockedDecrement
#else
#define _AtlInterlockedIncrement InterlockedIncrement
#define _AtlInterlockedDecrement InterlockedDecrement
/* managed code must use the non-intrinsics */
#ifdef InterlockedIncrement
#undef InterlockedIncrement
#endif
#ifdef InterlockedDecrement
#undef InterlockedDecrement
#endif
#endif  // !_M_CEE
#else
#define _AtlInterlockedIncrement InterlockedIncrement
#define _AtlInterlockedDecrement InterlockedDecrement
#endif  // _M_IX86_

/* Modified by TaoRenyong 20110923
struct CStringData
{
	IAtlStringMgr* pStringMgr;  // String manager for this CStringData
	int nDataLength;			// Length of currently used data in XCHARs (not including terminating null)
	int nAllocLength;			// Length of allocated data in XCHARs (not including terminating null)
	long nRefs;					// Reference count: negative == locked
	// XCHAR data[nAllocLength+1]  // A CStringData is always followed in memory by the actual array of character data
}
*/
struct CStringData
{
	long nRefs;			// Reference count: negative == locked
	int nDataLength;	// Length of currently used data in XCHARs (not including terminating null)
	int nAllocLength;	// Length of allocated data in XCHARs (not including terminating null)
	// XCHAR data[nAllocLength+1]  // A CStringData is always followed in memory by the actual array of character data

	void* data() throw()
	{
		return (this + 1);
	}

	void addRef() throw()
	{
		ATLASSERT(nRefs > 0);
		_AtlInterlockedIncrement(&nRefs);
	}
	
	void release() throw()
	{
		ATLASSERT( nRefs != 0 );
		if( _AtlInterlockedDecrement( &nRefs ) <= 0 )
		{
#ifndef _NO_MEM_POOL
			mp_free( this );
#else
			free(this);
#endif
		}
	}

	long GetRef()
	{
		return nRefs;
	}

	bool IsLocked() const throw()
	{
		return nRefs < 0;
	}

	bool IsShared() const throw()
	{
		return( nRefs > 1 ); 
	}

	void Lock() throw()
	{
		ATLASSERT( nRefs <= 1 );
		nRefs = -1;
	}

	void Unlock() throw()
	{
		ATLASSERT( IsLocked() );

		if(IsLocked())
		{
			nRefs++;  // Locked buffers can't be shared, so no interlocked operation necessary
			if( nRefs == 0 )
			{
				nRefs = 1;
			}
		}
	}
};

/************************Added by TaoRenyong 2011.09.23 below******************************/
// Globals
// For an empty string, m_pchData will point here
// (note: avoids special case of checking for NULL m_pchData)
// empty string data (and locked)
_declspec(selectany) int rgInitData[] = { 2, 0, 0, 0 };
_declspec(selectany) CStringData* _atltmpDataNil = (CStringData*)&rgInitData;
/************************Added by TaoRenyong 2011.09.23 above******************************/

template< typename BaseType > 
class ChTraitsBase{};

template<>
class ChTraitsBase< char >
{
public:
	typedef char XCHAR;
	typedef LPSTR PXSTR;
	typedef LPCSTR PCXSTR;
	typedef wchar_t YCHAR;
	typedef LPWSTR PYSTR;
	typedef LPCWSTR PCYSTR;
};

template<>
class ChTraitsBase< wchar_t >
{
public:
	typedef wchar_t XCHAR;
	typedef LPWSTR PXSTR;
	typedef LPCWSTR PCXSTR;
	typedef char YCHAR;
	typedef LPSTR PYSTR;
	typedef LPCSTR PCYSTR;
};

template< typename BaseType >
class CSimpleStringT
{
public:
	typedef typename ChTraitsBase< BaseType >::XCHAR XCHAR;
	typedef typename ChTraitsBase< BaseType >::PXSTR PXSTR;
	typedef typename ChTraitsBase< BaseType >::PCXSTR PCXSTR;
	typedef typename ChTraitsBase< BaseType >::YCHAR YCHAR;
	typedef typename ChTraitsBase< BaseType >::PYSTR PYSTR;
	typedef typename ChTraitsBase< BaseType >::PCYSTR PCYSTR;

public:
	CSimpleStringT( )
	{
		//ATLENSURE( pStringMgr != NULL );
		//CStringData* pData = pStringMgr->GetNilString();
		//Attach( pData );
		_InitData( ); //Modifed by TaoRenyong 20110923
	}
	
	CSimpleStringT( _In_ const CSimpleStringT& strSrc )
	{
		CStringData* pSrcData = strSrc.GetData();
		CStringData* pNewData = CloneData( pSrcData );
		Attach( pNewData );
	}
	
	CSimpleStringT( _In_ PCXSTR pszSrc )
	{
		int nLength = StringLength( pszSrc );
		CStringData* pData = _Allocate( nLength, sizeof( XCHAR ) );
		if( pData == NULL )
		{
			ATL_MACRO::AtlThrowImpl(E_OUTOFMEMORY);
		}
		Attach( pData );
		SetLength( nLength );
		CopyChars( m_pszData, nLength, pszSrc, nLength );
	}

	CSimpleStringT( _In_ const XCHAR* pchSrc, _In_ int nLength )
	{
		if(pchSrc == NULL && nLength != 0)
			ATL_MACRO::AtlThrowImpl(E_INVALIDARG);

		CStringData* pData = _Allocate( nLength, sizeof( XCHAR ) );
		if( pData == NULL )
		{
			ATL_MACRO::AtlThrowImpl(E_OUTOFMEMORY);
		}
		Attach( pData );
		SetLength( nLength );
		CopyChars( m_pszData, nLength, pchSrc, nLength );
	}

	~CSimpleStringT() throw()
	{	
		GetData()->release();
	}
	
	operator CSimpleStringT<BaseType>&()
	{
		return *(CSimpleStringT<BaseType>*)this;
	}

	CSimpleStringT& operator=( _In_ const CSimpleStringT& strSrc )
	{
		CStringData* pSrcData = strSrc.GetData();
		CStringData* pOldData = GetData();
		if( pSrcData != pOldData)
		{
			/* Modified By TaoRenyong 20110923
			Deal with 'pSrcData->IsLocked()' in CloneData()
			Deal with 'NULL_STR == pOldData->GetRef()' in pOldData->release()
			Deal with 'NULL_STR == pSrcData->GetRef()' in CloneData() Invoke pSrcData->addRef()
			*/
			if( pOldData->IsLocked() )//Not NullString
			{
				SetString( strSrc.GetString(), strSrc.GetLength() );
			}
			else
			{
				CStringData* pNewData = CloneData( pSrcData );
				pOldData->release();
				Attach( pNewData );
			}
		}

		return( *this );
	}
	
	CSimpleStringT& operator=( _In_opt_ PCXSTR pszSrc )
	{
		SetString( pszSrc );

		return( *this );
	}

	CSimpleStringT& operator+=( _In_ const CSimpleStringT& strSrc )
	{
		Append( strSrc );

		return( *this );
	}
	
	CSimpleStringT& operator+=( _In_opt_z_ PCXSTR pszSrc )
	{
		Append( pszSrc );

		return( *this );
	}

	CSimpleStringT& operator+=( _In_ char ch )
	{
		AppendChar( XCHAR( ch ) );

		return( *this );
	}

	CSimpleStringT& operator+=( _In_ unsigned char ch )
	{
		AppendChar( XCHAR( ch ) );

		return( *this );
	}

	CSimpleStringT& operator+=( _In_ wchar_t ch )
	{
		AppendChar( XCHAR( ch ) );

		return( *this );
	}

	XCHAR operator[]( _In_ int iChar ) const
	{
		ATLASSERT( (iChar >= 0) && (iChar <= GetLength()) );  // Indexing the '\0' is OK
		
		if( (iChar < 0) || (iChar > GetLength()) )
			ATL_MACRO::AtlThrowImpl(E_INVALIDARG);
			
		return( m_pszData[iChar] );
	}

	operator PCXSTR() const throw()
	{
		return( m_pszData );
	}

	void Append( _In_opt_z_ PCXSTR pszSrc )
	{
		Append( pszSrc, StringLength( pszSrc ) );
	}

	void Append( _In_opt_z_count_(nLength) PCXSTR pszSrc, _In_ int nLength )
	{
		// See comment in SetString() about why we do this
		UINT_PTR nOffset = pszSrc-GetString();

		UINT nOldLength = GetLength();
		//Make sure the nLength is greater than zero
//		ATLENSURE_THROW(nLength >=0, E_INVALIDARG);
		ATLASSERT(nLength >=0); //Modified By TaoRenyong 20110923

		//Make sure we don't read pass end of the terminating NULL
		nLength = StringLengthN(pszSrc, nLength);

		//Make sure after the string doesn't exceed INT_MAX after appending
//		ATLENSURE_THROW(INT_MAX - nLength >= static_cast<int>(nOldLength), E_INVALIDARG);
		ATLASSERT(INT_MAX - nLength >= static_cast<int>(nOldLength)); //Modified By TaoRenyong 20110923

		int nNewLength = nOldLength+nLength;
		PXSTR pszBuffer = GetBuffer( nNewLength );
		if( nOffset <= nOldLength )
		{
			pszSrc = pszBuffer+nOffset;
			// No need to call CopyCharsOverlapped, since the destination is
			// beyond the end of the original buffer
		}
		CopyChars( pszBuffer+nOldLength, nLength, pszSrc, nLength );
		releaseBufferSetLength( nNewLength );
	}

	void AppendChar( _In_ XCHAR ch )
	{
		UINT nOldLength = GetLength();
		int nNewLength = nOldLength+1;
		PXSTR pszBuffer = GetBuffer( nNewLength );
		pszBuffer[nOldLength] = ch;
		releaseBufferSetLength( nNewLength );
	}

	void Append( _In_ const CSimpleStringT& strSrc )
	{
		Append( strSrc.GetString(), strSrc.GetLength() );
	}

	void Empty() throw()
	{
		CStringData* pOldData = GetData();
//		IAtlStringMgr* pStringMgr = pOldData->pStringMgr;
		if( pOldData->nDataLength == 0 )
		{
			return;
		}

		if( pOldData->IsLocked() )
		{
			// Don't reallocate a locked buffer that's shrinking
			SetLength( 0 );
		}
		else
		{
			pOldData->release();
//			CStringData* pNewData = pStringMgr->GetNilString();
//			Attach( pNewData );
			_InitData(); //Added by TaoRenyong 20110923
		}
	}

	void FreeExtra()
	{
		CStringData* pOldData = GetData();
		int nLength = pOldData->nDataLength;
//		IAtlStringMgr* pStringMgr = pOldData->pStringMgr;
		if( pOldData->nAllocLength == nLength )
		{
			return;
		}

		if( !pOldData->IsLocked() )  // Don't reallocate a locked buffer that's shrinking
		{
			//Modified by TaoRenyong 20110923
			CStringData* pNewData = _Allocate( nLength, sizeof( XCHAR ) );
			if( pNewData == NULL )
			{
				SetLength( nLength );
				return;
			}
			
			CopyChars( PXSTR( pNewData->data() ), nLength, 
				PCXSTR( pOldData->data() ), nLength );

			pOldData->release();
			Attach( pNewData );
			SetLength( nLength );
		}
	}

	int GetAllocLength() const throw()
	{
		return( GetData()->nAllocLength );
	}

	XCHAR GetAt( _In_ int iChar ) const
	{
		ATLASSERT( (iChar >= 0) && (iChar <= GetLength()) );  // Indexing the '\0' is OK
		if( (iChar < 0) || (iChar > GetLength()) )
			ATL_MACRO::AtlThrowImpl(E_INVALIDARG);		
			
		return( m_pszData[iChar] );
	}

	PXSTR GetBuffer()
	{
		CStringData* pData = GetData();
		if( pData->IsShared() )
		{
			Fork( pData->nDataLength );
		}

		return( m_pszData );
	}

	_Ret_cap_(nMinBufferLength + 1) PXSTR GetBuffer( _In_ int nMinBufferLength )
	{
		return( PrepareWrite( nMinBufferLength ) );
	}

	_Ret_cap_(nLength + 1) PXSTR GetBufferSetLength( _In_ int nLength )
	{
		PXSTR pszBuffer = GetBuffer( nLength );
		SetLength( nLength );

		return( pszBuffer );
	}

	int GetLength() const throw()
	{
		return( GetData()->nDataLength );
	}

	_Ret_count_x_(m_nLength) PCXSTR GetString() const throw()
	{
		return( m_pszData );
	}

	bool IsEmpty() const throw()
	{
		return( GetLength() == 0 );
	}

	PXSTR LockBuffer()
	{
		CStringData* pData = GetData();
		if( pData->IsShared() )
		{
			Fork( pData->nDataLength );
			pData = GetData();  // Do it again, because the fork might have changed it
		}
		pData->Lock();

		return( m_pszData );
	}

	void UnlockBuffer() throw()
	{
		CStringData* pData = GetData();
		pData->Unlock();
	}

	void Preallocate( _In_ int nLength )
	{
		PrepareWrite( nLength );
	}

	void releaseBuffer( _In_ int nNewLength = -1 )
	{
		if( nNewLength == -1 )
		{
			int nAlloc = GetData()->nAllocLength;
			nNewLength = StringLengthN( m_pszData, nAlloc);
		}
		SetLength( nNewLength );
	}

	void releaseBufferSetLength( _In_ int nNewLength )
	{
		ATLASSERT( nNewLength >= 0 );
		SetLength( nNewLength );
	}

	void Truncate( _In_ int nNewLength )
	{
		ATLASSERT( nNewLength <= GetLength() );
		GetBuffer( nNewLength );
		releaseBufferSetLength( nNewLength );
	}

	void SetAt( _In_ int iChar, _In_ XCHAR ch )
	{
		ATLASSERT( (iChar >= 0) && (iChar < GetLength()) );

		if( (iChar < 0) || (iChar >= GetLength()) )
			ATL_MACRO::AtlThrowImpl(E_INVALIDARG);		
			
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer();
		pszBuffer[iChar] = ch;
		releaseBufferSetLength( nLength );
			
	}

	void SetString( _In_opt_ PCXSTR pszSrc )
	{
		SetString( pszSrc, StringLength( pszSrc ) );
	}

	void SetString( _In_opt_count_(nLength) PCXSTR pszSrc, _In_ int nLength )
	{
		if( nLength == 0 )
		{
			Empty();
		}
		else
		{
			// It is possible that pszSrc points to a location inside of our 
			// buffer.  GetBuffer() might change m_pszData if (1) the buffer 
			// is shared or (2) the buffer is too small to hold the new 
			// string.  We detect this aliasing, and modify pszSrc to point
			// into the newly allocated buffer instead.
			
			if(pszSrc == NULL)
				ATL_MACRO::AtlThrowImpl(E_INVALIDARG);			
				
			UINT nOldLength = GetLength();
			UINT_PTR nOffset = pszSrc-GetString();
			// If 0 <= nOffset <= nOldLength, then pszSrc points into our 
			// buffer

			PXSTR pszBuffer = GetBuffer( nLength );
			if( nOffset <= nOldLength )
			{
				CopyCharsOverlapped( pszBuffer, GetAllocLength(), 
					pszBuffer+nOffset, nLength );
			}
			else
			{
				CopyChars( pszBuffer, GetAllocLength(), pszSrc, nLength );
			}
			releaseBufferSetLength( nLength );
		}
	}

public:
	friend CSimpleStringT operator+(
		_In_ const CSimpleStringT& str1,
		_In_ const CSimpleStringT& str2 )
	{
//		CSimpleStringT s( str1.GetManager() );
		CSimpleStringT s;//Modified by TaoRenyong 20110923

		Concatenate( s, str1, str1.GetLength(), str2, str2.GetLength() );

		return( s );
	}

	friend CSimpleStringT operator+(
		_In_ const CSimpleStringT& str1,
		_In_ PCXSTR psz2 )
	{
//		CSimpleStringT s( str1.GetManager() );
		CSimpleStringT s;//Modified by TaoRenyong 20110923

		Concatenate( s, str1, str1.GetLength(), psz2, StringLength( psz2 ) );

		return( s );
	}

	friend CSimpleStringT operator+(
		_In_ PCXSTR psz1,
		_In_ const CSimpleStringT& str2 )
	{
//		CSimpleStringT s( str1.GetManager() );
		CSimpleStringT s;//Modified by TaoRenyong 20110923

		Concatenate( s, psz1, StringLength( psz1 ), str2, str2.GetLength() );

		return( s );
	}

//	//_ATL_INSECURE_DEPRECATE("CSimpleStringT::CopyChars must be passed a buffer size")
	static void __cdecl CopyChars
		( _Out_cap_(nChars) XCHAR* pchDest, _In_opt_count_(nChars) const XCHAR* pchSrc, _In_ int nChars ) throw()
	{
		#pragma warning (push)
		#pragma warning(disable : 4996)
		memcpy( pchDest, pchSrc, nChars*sizeof( XCHAR ) );
		#pragma warning (pop)
	}

	static void __cdecl CopyChars
		( _Out_cap_post_count_(nDestLen,nChars) XCHAR* pchDest, _In_ size_t nDestLen, _In_opt_count_(nChars) const XCHAR* pchSrc, _In_ int nChars ) throw()
	{
		memcpy_s( pchDest, nDestLen*sizeof( XCHAR ), 
			pchSrc, nChars*sizeof( XCHAR ) );
	}

	//_ATL_INSECURE_DEPRECATE("CSimpleStringT::CopyCharsOverlapped must be passed a buffer size")
	static void __cdecl CopyCharsOverlapped
		( _Out_cap_(nChars) XCHAR* pchDest, _In_count_(nChars) const XCHAR* pchSrc, _In_ int nChars ) throw()
	{
		#pragma warning (push)
		#pragma warning(disable : 4996)
		memmove( pchDest, pchSrc, nChars*sizeof( XCHAR ) );
		#pragma warning (pop)
	}
	static void __cdecl CopyCharsOverlapped
		( _Out_cap_post_count_(nDestLen, nDestLen) XCHAR* pchDest, _In_ size_t nDestLen, _In_count_(nChars) const XCHAR* pchSrc, _In_ int nChars ) throw()
	{
		memmove_s( pchDest, nDestLen*sizeof( XCHAR ), 
			pchSrc, nChars*sizeof( XCHAR ) );
	}

	static int __cdecl StringLength( _In_opt_z_ const char* psz ) throw()
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( strlen( psz ) ) );
	}

	static int __cdecl StringLength( _In_opt_z_ const wchar_t* psz ) throw()
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( wcslen( psz ) ) );
	}

	static int __cdecl StringLengthN( _In_opt_z_ const char* psz, _In_ size_t sizeInXChar ) throw()
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( strnlen( psz, sizeInXChar ) ) );
	}

	static int __cdecl StringLengthN( _In_opt_z_ const wchar_t* psz, _In_ size_t sizeInXChar ) throw()
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( wcsnlen( psz, sizeInXChar ) ) );
	}

protected:
	static void __cdecl Concatenate( _Out_ CSimpleStringT& strResult, _In_count_(nLength1) PCXSTR psz1, _In_ int nLength1, _In_count_(nLength2) PCXSTR psz2, _In_ int nLength2 )
	{
		int nNewLength = nLength1+nLength2;
		PXSTR pszBuffer = strResult.GetBuffer( nNewLength );
		CopyChars( pszBuffer, nLength1, psz1, nLength1 );
		CopyChars( pszBuffer+nLength1, nLength2, psz2, nLength2 );
		strResult.releaseBufferSetLength( nNewLength );
	}

// Implementation
private:
	/**********************Added By TaoRenyong 2011.09.23 below***************/
	void _InitData()
	{
		Attach(_GetNilString());
	}

	static CStringData* _GetNilString()
	{
		_atltmpDataNil->addRef();
		return _atltmpDataNil;
	}

	static CStringData* _Allocate( int nChars, int nCharSize ) throw()
	{
		size_t nTotalSize;
		CStringData* pData;
		size_t nDataBytes;

		ATLASSERT(nCharSize > 0);

		if(nChars < 0)
		{
			ATLASSERT(FALSE);
			return NULL;
		}

		nDataBytes = (nChars+1)*nCharSize;
		nTotalSize = sizeof( CStringData )+nDataBytes;

#ifndef _NO_MEM_POOL
		pData = (CStringData*)mp_alloc( nTotalSize );
#else
		pData = (CStringData*)malloc( nTotalSize );
#endif	

		if (pData == NULL)
			return NULL;

		pData->nRefs = 1;
		pData->nAllocLength = nChars;
		pData->nDataLength = 0;

		return pData;
	}

	static void _Free( CStringData* pData ) throw()
	{
		pData->release();
	}

	static CStringData* _Reallocate( CStringData* pData, int nChars, int nCharSize ) throw()
	{
		CStringData* pNewData;
		size_t nTotalSize;
		size_t nDataBytes;

		ATLASSERT(nCharSize > 0);

		if(nChars < 0)
		{
			ATLASSERT(FALSE);
			return NULL;
		}

		nDataBytes = (nChars+1)*nCharSize;
		nTotalSize = sizeof( CStringData )+nDataBytes;

#ifdef _NO_MEM_POOL
		pNewData = (CStringData*)realloc( pData, nTotalSize );
#else
		pNewData = (CStringData*)mp_realloc( pData, nTotalSize );
#endif	

		if( pNewData == NULL )
		{
			return NULL;
		}
		pNewData->nAllocLength = nChars;

		return pNewData;
	}
	/**********************Added By TaoRenyong 2011.09.23 above***************/

	void Attach( _In_ CStringData* pData ) throw()
	{
		m_pszData = static_cast< PXSTR >( pData->data() );
	}

	ATL_NOINLINE void Fork( _In_ int nLength )
	{
		CStringData* pOldData = GetData();
		int nOldLength = pOldData->nDataLength;
//		CStringData* pNewData = pOldData->pStringMgr->Clone()->Allocate( nLength, sizeof( XCHAR ) );
		CStringData* pNewData = _Allocate( nLength, sizeof( XCHAR ) ); //Added by TaoRenyong 20110923
		if( pNewData == NULL )
		{
			ATL_MACRO::AtlThrowImpl(E_OUTOFMEMORY);
		}
		int nCharsToCopy = ((nOldLength < nLength) ? nOldLength : nLength)+1;  // Copy '\0'
		CopyChars( PXSTR( pNewData->data() ), nCharsToCopy, 
			PCXSTR( pOldData->data() ), nCharsToCopy );
		pNewData->nDataLength = nOldLength;
		pOldData->release();
		Attach( pNewData );
	}

	CStringData* GetData() const throw()
	{
		return( reinterpret_cast< CStringData* >( m_pszData )-1 );
	}

	PXSTR PrepareWrite( _In_ int nLength )
	{
		CStringData* pOldData = GetData();
		int nShared = 1-pOldData->nRefs;  // nShared < 0 means true, >= 0 means false
		int nTooShort = pOldData->nAllocLength-nLength;  // nTooShort < 0 means true, >= 0 means false
		if( (nShared|nTooShort) < 0 )  // If either sign bit is set (i.e. either is less than zero), we need to copy data
		{
			PrepareWrite2( nLength );
		}

		return( m_pszData );
	}

	ATL_NOINLINE void PrepareWrite2( _In_ int nLength )
	{
		CStringData* pOldData = GetData();
		if( pOldData->nDataLength > nLength )
		{
			nLength = pOldData->nDataLength;
		}
		if( pOldData->IsShared() )
		{
			Fork( nLength );
		}
		else if( pOldData->nAllocLength < nLength )
		{
			// Grow exponentially, until we hit 1K.
			int nNewLength = pOldData->nAllocLength;
			if( nNewLength > 1024 )
			{
				nNewLength += 1024;
			}
			else
			{
				nNewLength *= 2;
			}
			if( nNewLength < nLength )
			{
				nNewLength = nLength;
			}
			Reallocate( nNewLength );
		}
	}

	ATL_NOINLINE void Reallocate( _In_ int nLength )
	{
		CStringData* pOldData = GetData();
		ATLASSERT( pOldData->nAllocLength < nLength );
//		IAtlStringMgr* pStringMgr = pOldData->pStringMgr;
		if ( pOldData->nAllocLength >= nLength || nLength <= 0)
		{
			ATL_MACRO::AtlThrowImpl(E_OUTOFMEMORY);
			return;
		}

//		CStringData* pNewData = pStringMgr->Reallocate( pOldData, nLength, sizeof( XCHAR ) );
		CStringData* pNewData = _Reallocate( pOldData, nLength, sizeof( XCHAR ) );//Modified by TaoRenyong 20110923
		if( pNewData == NULL )
		{
			ATL_MACRO::AtlThrowImpl(E_OUTOFMEMORY);
		}
		Attach( pNewData );
	}

	void SetLength( _In_ int nLength )
	{
		ATLASSERT( nLength >= 0 );
		ATLASSERT( nLength <= GetData()->nAllocLength );

		if( nLength < 0 || nLength > GetData()->nAllocLength)
			ATL_MACRO::AtlThrowImpl(E_INVALIDARG);
			
		GetData()->nDataLength = nLength;
		m_pszData[nLength] = 0;
	}

	static CStringData* __cdecl CloneData( _In_ CStringData* pData )
	{
		CStringData* pNewData = NULL;

		if( pData->IsLocked() )//Modified by TaoRenyong 20110923
		{
			pNewData = _Allocate( pData->nDataLength, sizeof( XCHAR ) );
			if( pNewData == NULL )
			{
				ATL_MACRO::AtlThrowImpl(E_OUTOFMEMORY);
			}
			pNewData->nDataLength = pData->nDataLength;
			CopyChars( PXSTR( pNewData->data() ), pData->nDataLength+1,
				PCXSTR( pData->data() ), pData->nDataLength+1 );  // Copy '\0'
		}
		else 
		{
			pNewData = pData;
			pNewData->addRef();//Deal with pData is NULL_STR In addRef
		}

		return( pNewData );
	}

private:
	PXSTR m_pszData;
	
	friend class CSimpleStringT<BaseType>;
};

};  // namespace ATL_STR

