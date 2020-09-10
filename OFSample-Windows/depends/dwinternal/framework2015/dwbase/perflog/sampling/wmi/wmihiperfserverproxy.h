/*! @file wmihiperfserverproxy.h 
 *  @brief Hi-Perf Wbem Server Proxy
 *
 *  用于查询Win32类型中不断发生变化的Perf数据
 *
 *  @author wesleywu
 *  @version 1.0
 *  @data 2012-08-28
 */

#pragma once

#include <map>

struct IWbemRefresher;
struct IWbemConfigureRefresher;

/*! @brief namespace Perf
 */	 
namespace Perf
{
	class CWbemClassObjectHandle;
	class CWbemLocator;

	/*! @brief Hi-Perf Wbem Server Proxy
	 *
	 *  使用IWbemRefresher实现Hi-Perf-Query
	 */
	class CWmiHiPerfServerProxy
	{
	public:	
		/*! @brief 尝试创建一个CWmiHiPerfServerProxy对象
		 * 
		 *  根据所提供的CWbemLocator创建一个CWmiHiPerfServerProxy对象
		 *
		 *  @param [in] loc - CWbemLocator对象
		 *  @param [out] ppWmiHiPerfServerProxy - 输出的CWmiHiPerfServerProxy对象
		 *  @return HRESULT 处理结果
		 *  - S_OK: 创建成功, 对象创建于ppWmiHiPerfServerProxy
		 *  - either: 创建失败
		 */
		static HRESULT CreateWmiHiPerfServerProxy(CWbemLocator loc, CWmiHiPerfServerProxy** ppWmiHiPerfServerProxy);

		/*! @brief 刷新Refresher
		 *  @param [in] lFlags 
		 *  @return HRESULT COM函数调用结果
		 */
		HRESULT Refresh(long lFlags = 0);

		/*! @brief 根据名称返回一个CWbemClassObjectHandle指针
		 *  @param [in] name - 对象名称 
		 *  @return CWbemClassObjectHandle*
		 *  - NULL if not exists in m_NamedObjectMap
		 */
	 	CWbemClassObjectHandle* NamedObject(std::wstring name);

		/*! @brief 向Refresher中加入一个CWbemClassObjectHandle对象, 并加入m_NamedObjectMap
		 * 
		 *  由className和nameFilter共同生成WbemClassObject对象路径
		 * 
		 *  @param [in] name - 对象名称 
		 *  @param [in] className - WIN32类型名称, 如Win32_Process
		 *  @param [in] nameFilter - 名称过滤, 如"YY.exe"
		 *  @return CWbemClassObjectHandle*
		 *  - NULL COM调用失败
		 */
		CWbemClassObjectHandle* AddNamedObject(std::wstring name, const wchar_t* className, const wchar_t* nameFilter);
		
		/*! @brief 获取上一次Refresh的时间
		 *  @return 上一次Refresh的时间
		 */
		LONGLONG	LastRefreshPerfCount();

		/*! @brief 析构函数
		 */
		~CWmiHiPerfServerProxy(void);

	protected:
		/*! @brief 构造函数
		 *  @param loc - CWbemLocator Object
		 */
		CWmiHiPerfServerProxy(CWbemLocator loc);

		/*! @brief 初始化
		 *	
		 *  创建m_pRefresher和m_pConfig对象
		 */
		HRESULT initialize();

	private:
		std::map<std::wstring, CWbemClassObjectHandle> m_NamedObjectMap;	/**< 名称-对象映射表 */

		IWbemRefresher*				m_pRefresher;			/**< Refresher */
		IWbemConfigureRefresher*	m_pConfig;				/**< Configure */

		LONGLONG					m_llRefreshPerfCount;	/**< 上一次Refresh时间 */

		CWbemLocator				m_locator;				/**< locator */
	};

	inline LONGLONG CWmiHiPerfServerProxy::LastRefreshPerfCount()
	{
		return m_llRefreshPerfCount;
	}
}
