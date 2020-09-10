/*! @file wmihiperfserverproxy.h 
 *  @brief Hi-Perf Wbem Server Proxy
 *
 *  ���ڲ�ѯWin32�����в��Ϸ����仯��Perf����
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
	 *  ʹ��IWbemRefresherʵ��Hi-Perf-Query
	 */
	class CWmiHiPerfServerProxy
	{
	public:	
		/*! @brief ���Դ���һ��CWmiHiPerfServerProxy����
		 * 
		 *  �������ṩ��CWbemLocator����һ��CWmiHiPerfServerProxy����
		 *
		 *  @param [in] loc - CWbemLocator����
		 *  @param [out] ppWmiHiPerfServerProxy - �����CWmiHiPerfServerProxy����
		 *  @return HRESULT ������
		 *  - S_OK: �����ɹ�, ���󴴽���ppWmiHiPerfServerProxy
		 *  - either: ����ʧ��
		 */
		static HRESULT CreateWmiHiPerfServerProxy(CWbemLocator loc, CWmiHiPerfServerProxy** ppWmiHiPerfServerProxy);

		/*! @brief ˢ��Refresher
		 *  @param [in] lFlags 
		 *  @return HRESULT COM�������ý��
		 */
		HRESULT Refresh(long lFlags = 0);

		/*! @brief �������Ʒ���һ��CWbemClassObjectHandleָ��
		 *  @param [in] name - �������� 
		 *  @return CWbemClassObjectHandle*
		 *  - NULL if not exists in m_NamedObjectMap
		 */
	 	CWbemClassObjectHandle* NamedObject(std::wstring name);

		/*! @brief ��Refresher�м���һ��CWbemClassObjectHandle����, ������m_NamedObjectMap
		 * 
		 *  ��className��nameFilter��ͬ����WbemClassObject����·��
		 * 
		 *  @param [in] name - �������� 
		 *  @param [in] className - WIN32��������, ��Win32_Process
		 *  @param [in] nameFilter - ���ƹ���, ��"YY.exe"
		 *  @return CWbemClassObjectHandle*
		 *  - NULL COM����ʧ��
		 */
		CWbemClassObjectHandle* AddNamedObject(std::wstring name, const wchar_t* className, const wchar_t* nameFilter);
		
		/*! @brief ��ȡ��һ��Refresh��ʱ��
		 *  @return ��һ��Refresh��ʱ��
		 */
		LONGLONG	LastRefreshPerfCount();

		/*! @brief ��������
		 */
		~CWmiHiPerfServerProxy(void);

	protected:
		/*! @brief ���캯��
		 *  @param loc - CWbemLocator Object
		 */
		CWmiHiPerfServerProxy(CWbemLocator loc);

		/*! @brief ��ʼ��
		 *	
		 *  ����m_pRefresher��m_pConfig����
		 */
		HRESULT initialize();

	private:
		std::map<std::wstring, CWbemClassObjectHandle> m_NamedObjectMap;	/**< ����-����ӳ��� */

		IWbemRefresher*				m_pRefresher;			/**< Refresher */
		IWbemConfigureRefresher*	m_pConfig;				/**< Configure */

		LONGLONG					m_llRefreshPerfCount;	/**< ��һ��Refreshʱ�� */

		CWbemLocator				m_locator;				/**< locator */
	};

	inline LONGLONG CWmiHiPerfServerProxy::LastRefreshPerfCount()
	{
		return m_llRefreshPerfCount;
	}
}
