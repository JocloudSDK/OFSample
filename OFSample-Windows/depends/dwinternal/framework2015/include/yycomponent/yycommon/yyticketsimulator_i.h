#pragma once

#include <string>
#include "dwbase/dwcomex.h"

class QString;
/**
   * @brief .
   */
DWDEFINE_INTERFACE(IYYTicketSimulator):public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int emitTicket(const QString& alias, const QString &data) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int emitTicket(const std::wstring &alias, const std::wstring &data) = 0;

	/**
	* @brief ��������û�������������Ỻ��data��Ϣ����������ɺ��Զ��ٴη���.
	* @param .
	* @return -1:���͵�����ǲ����ڵģ�����ʧ�ܣ�0�����ͳɹ���1�����͵��������������.
	*/
	virtual int emitTicketEx(const QString& alias, const QString &data) = 0;
	/**
	* @brief ��������û�������������Ỻ��data��Ϣ����������ɺ��Զ��ٴη���.
	* @param .
	* @return -1:���͵�����ǲ����ڵģ�����ʧ�ܣ�0�����ͳɹ���1�����͵��������������.
	*/
	virtual int emitTicketEx2(const std::wstring &alias, const std::wstring &data) = 0;

	/**
	* @brief ���ָ������ɻ�Ʊ�Ƿ�ת����yy���̣���������Ƶ��ת�Ƶ��������̵������
	* @param .��Ҫת���ķɻ�Ʊ����
	*/
    virtual void addRountedTicket(const QString& alias) = 0;

	/**
	* @brief �Ƴ�ͨ��addRountedTicket��ӵı���
	* @param .��Ҫת���ķɻ�Ʊ����
	*/
    virtual void removeRountedTicket(const QString& alias) = 0;    
};