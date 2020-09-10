/*! @file wmiserverproxy.h .h 
 *  @brief Wmi Server Proxy
 *
 *  使用WQL进行Win32对象的查询
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
	 *  使用WQL进行查询
	 */
	class CWmiServerProxy
	{
		typedef std::map<std::wstring, std::wstring> filter_t;		
	public:
		/*! @brief 构造函数
		 *  @param [in] loc - CWbemLocator Object
		 *  @param [in] serviceClass - e.g. L"Win32_Process"
		 *  @param [in] category - e.g. L"Win32_Process"
		 */
		CWmiServerProxy(CWbemLocator loc, const wchar_t* serviceClass = NULL, const wchar_t* category = NULL);

		/*! @brief 构造函数
		 */
		virtual ~CWmiServerProxy();

		/*! @brief 执行WQL查询
		 *  @return S_OK if call successfully, else either.
		 */
		HRESULT Start();

		/*! @brief 获取下一个CWbemClassObjectHandle
		 *  return m_enumerator中的下一个WbemClassObject
		 */
		CWbemClassObjectHandle Next();
		
		/*! @brief 设置Equal Filter, 即WQL中的Where xxx = "yyy"
		 */
		void SetEqualFilter(const wchar_t* k, const wchar_t* v);

		/*! @brief 设置Match Filter, 即WQL中的Where xxx like "yyy"
		 */
		void SetMatchFilter(const wchar_t* k, const wchar_t* v);

	protected:
		/*! @brief 根据类名和Condition构造WQL语句
		 *  @return 完整的WQL查询语句
		 */
		virtual std::wstring	getQuery() const;

		/*! @brief 根据Filters构造Condition
		 *  @return 不包含WHERE关键词的所有条件语句, e.g. xxx = "yyy" AND zzz like "zzzz"
		 */
		virtual std::wstring	getCondition() const;

		/*! @brief 获取ServiceClass类名
		 *  @return ServiceClass类名
		 */
		inline _bstr_t			getServiceClass() const
		{
			return _bstr_t(m_szServiceClass);
		}

		/*! @brief 获取category名
		 *  @return category名
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