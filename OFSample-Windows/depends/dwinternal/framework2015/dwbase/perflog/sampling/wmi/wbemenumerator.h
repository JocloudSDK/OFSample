/*! @file wbemenumerator.h 
 *  @brief 包装IEnumWbemClassObject
 *
 *  CWbemEnumerator类包装IEnumWbemClassObject, 实现CWbemClassObject对象的枚举
 *
 *  @author wesleywu
 *  @version 1.0
 *  @data 2012-08-28
 */

#pragma once

struct IWbemServices;
struct IEnumWbemClassObject;
class CWbemClassObjectHandle;

namespace Perf
{
	/*! @brief 类CWbemEnumerator
	 *
	 *  包装IEnumWbemClassObject, 实现CWbemClassObject对象的枚举
	 */
	class CWbemEnumerator
	{
	public:			
		/*! @brief 尝试创建一个CWbemEnumerator对象
		 * 
		 * 根据所提供的IWbemServices以及WQL查询语句创建一个CWbemEnumerator对象
		 *
		 * @param [in] svr - 指向IWbemServices对象
		 * @param [in] query - 查询语句
		 * @param [out] ppEnumerator - 输出的CWbemEnumerator对象
		 * @return HRESULT 处理结果
		 * - S_OK: 创建成功, 对象创建于ppEnumerator
		 * - either: 创建失败
		 */
		static HRESULT CreateEnumerator(IWbemServices* svr, const _bstr_t& query, CWbemEnumerator** ppEnumerator);

		/*! @brief 重新枚举(重置m_enumerator)
		 * 
		 */
		void Restart();

		/*! @brief 获取下一个CWbemClassObjectHandle
		 *  return m_enumerator中的下一个WbemClassObject
		 */
		CWbemClassObjectHandle Next();

		/*! @brief 析构函数 
		 */
		~CWbemEnumerator(void);

	protected:
		/*! @brief 构造函数 
		 */
		CWbemEnumerator();

	private:
		IEnumWbemClassObject* m_enumerator;		/**< 包装的m_enumerator */
	};
}