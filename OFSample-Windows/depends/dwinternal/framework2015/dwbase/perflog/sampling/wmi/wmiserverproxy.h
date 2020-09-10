/*! @file wmiserverproxy.h .h 
 *  @brief Wmi Server Proxy
 *
 *  ʹ��WQL����Win32����Ĳ�ѯ
 *
 *  @author wesleywu
 *  @version 1.0
 *  @data 2012-08-28
 */

#pragma once

#include <map>

/*! @brief namespace Perf
 */	 
namespace Perf
{
	class CWbemEnumerator;
	class CWbemLocator;
	class CWbemClassObjectHandle;

	/*! @brief Wmi Server Proxy
	 * 
	 *  ʹ��WQL���в�ѯ
	 */
	class CWmiServerProxy
	{
		typedef std::map<std::wstring, std::wstring> filter_t;		
	public:
		/*! @brief ���캯��
		 *  @param [in] loc - CWbemLocator Object
		 *  @param [in] serviceClass - e.g. L"Win32_Process"
		 *  @param [in] category - e.g. L"Win32_Process"
		 */
		CWmiServerProxy(CWbemLocator loc, const wchar_t* serviceClass = NULL, const wchar_t* category = NULL);

		/*! @brief ���캯��
		 */
		virtual ~CWmiServerProxy();

		/*! @brief ִ��WQL��ѯ
		 *  @return S_OK if call successfully, else either.
		 */
		HRESULT Start();

		/*! @brief ��ȡ��һ��CWbemClassObjectHandle
		 *  return m_enumerator�е���һ��WbemClassObject
		 */
		CWbemClassObjectHandle Next();
		
		/*! @brief ����Equal Filter, ��WQL�е�Where xxx = "yyy"
		 */
		void SetEqualFilter(const wchar_t* k, const wchar_t* v);

		/*! @brief ����Match Filter, ��WQL�е�Where xxx like "yyy"
		 */
		void SetMatchFilter(const wchar_t* k, const wchar_t* v);

	protected:
		/*! @brief ����������Condition����WQL���
		 *  @return ������WQL��ѯ���
		 */
		virtual std::wstring	getQuery() const;

		/*! @brief ����Filters����Condition
		 *  @return ������WHERE�ؼ��ʵ������������, e.g. xxx = "yyy" AND zzz like "zzzz"
		 */
		virtual std::wstring	getCondition() const;

		/*! @brief ��ȡServiceClass����
		 *  @return ServiceClass����
		 */
		inline _bstr_t			getServiceClass() const
		{
			return _bstr_t(m_szServiceClass);
		}

		/*! @brief ��ȡcategory��
		 *  @return category��
		 */
		inline const wchar_t*	categoryName() const
		{
			return m_szCategoryName ? m_szCategoryName : L"<unspecified>";
		}

	private:
		CWbemEnumerator* m_enumerator;
		filter_t m_filterEqual;
		filter_t m_filterMatch;
		CWbemLocator m_locator;

		const wchar_t* m_szServiceClass;
		const wchar_t* m_szCategoryName;
	};
}