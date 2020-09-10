#pragma once

#ifndef	DW_COMPONENT_ENV	//����������ᶨ�������
#include <string>
#include "dwbase/dwcomex.h"

class QString;
/**
   * @brief .
   */
DWDEFINE_INTERFACE(IYYTicketSimulatorInner):public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int emitTicketByComponentName(const QString& name, const QString &data) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int emitTicketByComponentName2(const std::wstring &name, const std::wstring &data) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int emitDataTicketByComponentName(const QString& name, const QString &data) = 0;

/**
   * @brief . ͨ��������ַ��ɻ�Ʊ����������ڣ������������ת��
   * @param .
   * @return .
   */
    virtual int emitTicketByComponentNameEx(const QString& name, const QString& data) = 0;

    /**
    * @brief . ���ָ������ɻ�Ʊ�Ƿ�ת����yy���̣���������Ƶ��ת�Ƶ��������̵������
    * @param . ��Ҫת�����������
    */
    virtual void addRountedComponent(const QString& name) = 0;

    /**
    * @brief . �Ƴ�addRountedComponent��ӵ�����
    * @param . ��Ҫת�����������
    */
    virtual void removeRountedComponent(const QString& name) = 0;
};

#endif	//����������ᶨ�������
