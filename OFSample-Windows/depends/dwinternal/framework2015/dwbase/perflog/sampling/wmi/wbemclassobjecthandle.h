/*! @file wbemclassobjectHandle.h
 *  @brief 包装Wbem Class Object
 *
 *  CWbemClassObjectHandle类包装IWbemClassObject, 实现Win32类别属性查找, 引用计数管理
 *
 *  @author wesleywu
 *  @version 1.0
 *  @data 2012-08-28
 */

#pragma once

#include "refcount.h"

struct IWbemClassObject;
class CVariant;
class _bstr_t;

/*! @brief namespace Perf
 */	 
namespace Perf
{	
	/*! @brief 类CWbemClassObjectHandle
	 *
	 *  包装WbemClassObject, 实现Win32类别属性查找, 引用计数管理
	 */
	class CWbemClassObjectHandle
	{
	public:
		/*! @brief 构造函数
		 *  @param [in] obj - 包装的IWbemClassObject指针, 缺省为0
		 */
		explicit CWbemClassObjectHandle(IWbemClassObject *obj = 0);
		
		/*! @brief 析构函数
		 */
		~CWbemClassObjectHandle(void);

		/*! @brief 赋值运算符重载
		 *  @param [in] rhs - const CWbemClassObjectHandle&
		 *  @return 本对象引用
		 */
		CWbemClassObjectHandle& operator=(const CWbemClassObjectHandle& rhs);

		/*! @brief 布尔运算符重载
		 *  @return true if m_classObject is NOT NULL, false either
		 */
		operator bool() const;		
		
		/*! @brief 转型运算符重载
		 *  @return m_classObject const
		 */
		operator IWbemClassObject* () const;

		/*! @brief RawObject
		 *  @return m_classObject指针引用, 可作为左值
		 */
		IWbemClassObject*& RawObject();

		/*! @brief 创建CWbemClassObjectHandle对象副本
		 *  @return CWbemClassObjectHandle对象副本
		 */ 
		CWbemClassObjectHandle Clone();

		/*! @brief 获取IWbemClassObject对象的完整路径
		 *  @return IWbemClassObject对象的完整路径
		 *  - 如某Win32_Process对象的路径\\YN-20120701YUNA\root\cimv2:Win32_Process.Handle="476"
		 */ 
		_bstr_t GetPath() const;

		/*! @brief 获取IWbemClassObject对象的一项属性值
		 *  @param [in]  name - 属性名称
		 *  @param [out] var - 属性值
		 *  @return COM函数调用成功与否(不论var是否为空)
		 *  - true 表示调用成功(S_OK/S_FALSE)
		 *  - false 表示调用失败(either)
		 */
		bool GetProperty(const wchar_t* name, CVariant& var) const;

	protected:  
		/*! @brief IWbemClassObject对象路径的属性名
		 *  @return 属性名称, 即L"__Path"
		 */
		virtual const wchar_t* path_property_name() const; 

	private:
		IWbemClassObject*	m_classObject;  /**< IWbemClassObject对象 */
		CRefCount			m_refCount;		/**< 引用计数器 */
	};
}
