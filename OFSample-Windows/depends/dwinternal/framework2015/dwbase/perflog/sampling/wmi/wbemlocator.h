/*! @file wbemlocator.h 
 *  @brief Wbem Locator&Server Wrapper, ����Wbem����
 *  @author wesleywu
 *  @version 1.0
 *  @data 2012-08-28
 */

#pragma once

#include "refcount.h"
#include <comutil.h>

#include <assert.h>

struct IWbemServices;
struct IWbemLocator;

/*! @brief namespace Perf
 */	 
namespace Perf
{
	/*! @brief ��CWbemLocator
	 *
	 *  Wbem Locator&Server Wrapper, ����Wbem����
	 */
	class CWbemLocator
	{
		/*! @brief IWbemServices Wrapper
		 *
		 *  ��װIWbemServices��ʵ�����ü���
		 */
		class wbem_server
		{
			CRefCount		_ref;
			IWbemServices*	_svr;
		public:
			wbem_server(IWbemServices* svr = 0);
			~wbem_server();
			wbem_server& operator = (const wbem_server& rhs);
			inline operator IWbemServices* () const;
		};

		/*! @brief IWbemLocator Wrapper
		 *
		 *  ��װIWbemLocator��ʵ������Wbem����
		 */
		struct the_locator
		{
			IWbemLocator* _locator;

			the_locator();
			~the_locator();
			wbem_server connect_to(_bstr_t ns, _bstr_t user, _bstr_t password, _bstr_t locale);
		};

	public:
		/*! @brief ���캯��
		 */
		CWbemLocator(const wchar_t* host, 
			const wchar_t* user,
			const wchar_t* psw,
			const wchar_t* locale, 
			const wchar_t* path);

		/*! @brief ��ȡIWbemServices����
		 *	@return IWbemServices����
		 */
		IWbemServices* GetServer() const;

		/*! @brief ��ʼ��
		 *
		 *  call CoSetProxyBlanket
		 */
		HRESULT Initialize();
	private:
		const wchar_t* m_szHost;			/**< �������� */
		const wchar_t* m_szUser;			/**< �û��� */
		const wchar_t* m_szPassword;		/**< ���� */
		const wchar_t* m_szLocale;			/**< Locale, 0 */
		const wchar_t* m_szPath;			/**< ·�� */
		wbem_server m_server;				/**< wbem_server Object */
	};

	inline CWbemLocator local_host(const wchar_t* path = L"\\root\\cimv2")  
	{  
		assert(path);  
		return CWbemLocator(L".", 0, 0, 0, path);  
	}  
	inline CWbemLocator local_host_with_credential(const wchar_t* user, const wchar_t* pwd, const wchar_t* path = L"\\root\\cimv2")  
	{  
		assert(user && pwd && path);  
		return CWbemLocator(L".", user, pwd, 0, path);  
	}  
	inline CWbemLocator remote_host(const wchar_t* host, const wchar_t* path = L"\\root\\cimv2")  
	{  
		assert(host && path);  
		return CWbemLocator(host, 0, 0, 0, path);  
	}  
	inline CWbemLocator remote_host_with_credential(const wchar_t* host, const wchar_t* user, const wchar_t* pwd, const wchar_t* path = L"\\root\\cimv2")  
	{  
		assert(host && user && pwd && path);  
		return CWbemLocator(host, user, pwd, 0, path);  
	}  
}
