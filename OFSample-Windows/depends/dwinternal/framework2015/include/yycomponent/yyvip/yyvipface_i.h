#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

DWDEFINE_INTERFACE(IYYVipFace): public IUnk
{
	virtual int groupCount() = 0;
	virtual FACE_ATTR smiliesByKey(const std::wstring& smiliesKey) = 0;
	virtual bool isSmiliesKeyValid(const std::wstring& smiliesKey) = 0;

	/**
   * @brief .��ȡvip��������
   * @param .smiliesKey ����key
   * @return ��������
   */
	virtual std::string getFileData(const std::wstring& smiliesKey) = 0;

	virtual bool groupIsInWhiteList(UINT32 nIndex) = 0;
	virtual bool isVipFaceDependVipGrade(const std::wstring& smiliesKey) = 0;

    /**
   * @brief .�ͻ��˻�ȡvip��Ϣ��ͨ�ýӿڣ�����汾��������
   * @param .request ������Ϣ
   * @return ��Ҫ����Ϣ
   */
    virtual std::string commonGetVipInfo(const std::string& request) = 0;
};