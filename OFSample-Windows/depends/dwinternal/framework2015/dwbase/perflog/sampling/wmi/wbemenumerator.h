/*! @file wbemenumerator.h 
 *  @brief ��װIEnumWbemClassObject
 *
 *  CWbemEnumerator���װIEnumWbemClassObject, ʵ��CWbemClassObject�����ö��
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
	/*! @brief ��CWbemEnumerator
	 *
	 *  ��װIEnumWbemClassObject, ʵ��CWbemClassObject�����ö��
	 */
	class CWbemEnumerator
	{
	public:			
		/*! @brief ���Դ���һ��CWbemEnumerator����
		 * 
		 * �������ṩ��IWbemServices�Լ�WQL��ѯ��䴴��һ��CWbemEnumerator����
		 *
		 * @param [in] svr - ָ��IWbemServices����
		 * @param [in] query - ��ѯ���
		 * @param [out] ppEnumerator - �����CWbemEnumerator����
		 * @return HRESULT ������
		 * - S_OK: �����ɹ�, ���󴴽���ppEnumerator
		 * - either: ����ʧ��
		 */
		static HRESULT CreateEnumerator(IWbemServices* svr, const _bstr_t& query, CWbemEnumerator** ppEnumerator);

		/*! @brief ����ö��(����m_enumerator)
		 * 
		 */
		void Restart();

		/*! @brief ��ȡ��һ��CWbemClassObjectHandle
		 *  return m_enumerator�е���һ��WbemClassObject
		 */
		CWbemClassObjectHandle Next();

		/*! @brief �������� 
		 */
		~CWbemEnumerator(void);

	protected:
		/*! @brief ���캯�� 
		 */
		CWbemEnumerator();

	private:
		IEnumWbemClassObject* m_enumerator;		/**< ��װ��m_enumerator */
	};
}