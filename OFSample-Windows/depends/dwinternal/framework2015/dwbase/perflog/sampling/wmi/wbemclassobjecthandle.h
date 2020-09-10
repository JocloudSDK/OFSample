/*! @file wbemclassobjectHandle.h
 *  @brief ��װWbem Class Object
 *
 *  CWbemClassObjectHandle���װIWbemClassObject, ʵ��Win32������Բ���, ���ü�������
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
	/*! @brief ��CWbemClassObjectHandle
	 *
	 *  ��װWbemClassObject, ʵ��Win32������Բ���, ���ü�������
	 */
	class CWbemClassObjectHandle
	{
	public:
		/*! @brief ���캯��
		 *  @param [in] obj - ��װ��IWbemClassObjectָ��, ȱʡΪ0
		 */
		explicit CWbemClassObjectHandle(IWbemClassObject *obj = 0);
		
		/*! @brief ��������
		 */
		~CWbemClassObjectHandle(void);

		/*! @brief ��ֵ���������
		 *  @param [in] rhs - const CWbemClassObjectHandle&
		 *  @return ����������
		 */
		CWbemClassObjectHandle& operator=(const CWbemClassObjectHandle& rhs);

		/*! @brief �������������
		 *  @return true if m_classObject is NOT NULL, false either
		 */
		operator bool() const;		
		
		/*! @brief ת�����������
		 *  @return m_classObject const
		 */
		operator IWbemClassObject* () const;

		/*! @brief RawObject
		 *  @return m_classObjectָ������, ����Ϊ��ֵ
		 */
		IWbemClassObject*& RawObject();

		/*! @brief ����CWbemClassObjectHandle���󸱱�
		 *  @return CWbemClassObjectHandle���󸱱�
		 */ 
		CWbemClassObjectHandle Clone();

		/*! @brief ��ȡIWbemClassObject���������·��
		 *  @return IWbemClassObject���������·��
		 *  - ��ĳWin32_Process�����·��\\YN-20120701YUNA\root\cimv2:Win32_Process.Handle="476"
		 */ 
		_bstr_t GetPath() const;

		/*! @brief ��ȡIWbemClassObject�����һ������ֵ
		 *  @param [in]  name - ��������
		 *  @param [out] var - ����ֵ
		 *  @return COM�������óɹ����(����var�Ƿ�Ϊ��)
		 *  - true ��ʾ���óɹ�(S_OK/S_FALSE)
		 *  - false ��ʾ����ʧ��(either)
		 */
		bool GetProperty(const wchar_t* name, CVariant& var) const;

	protected:  
		/*! @brief IWbemClassObject����·����������
		 *  @return ��������, ��L"__Path"
		 */
		virtual const wchar_t* path_property_name() const; 

	private:
		IWbemClassObject*	m_classObject;  /**< IWbemClassObject���� */
		CRefCount			m_refCount;		/**< ���ü����� */
	};
}
