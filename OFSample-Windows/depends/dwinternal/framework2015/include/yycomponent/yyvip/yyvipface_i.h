#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

DWDEFINE_INTERFACE(IYYVipFace): public IUnk
{
	virtual int groupCount() = 0;
	virtual FACE_ATTR smiliesByKey(const std::wstring& smiliesKey) = 0;
	virtual bool isSmiliesKeyValid(const std::wstring& smiliesKey) = 0;

	/**
   * @brief .获取vip表情数据
   * @param .smiliesKey 表情key
   * @return 表情数据
   */
	virtual std::string getFileData(const std::wstring& smiliesKey) = 0;

	virtual bool groupIsInWhiteList(UINT32 nIndex) = 0;
	virtual bool isVipFaceDependVipGrade(const std::wstring& smiliesKey) = 0;

    /**
   * @brief .客户端获取vip信息的通用接口，避免版本兼容问题
   * @param .request 哪种信息
   * @return 需要的信息
   */
    virtual std::string commonGetVipInfo(const std::string& request) = 0;
};