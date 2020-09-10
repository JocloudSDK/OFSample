#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"

/**
  * @brief 新手指引接口,通过Root->getServiceInterface<IYYBeginnerGuide>(KSERVICEMainframe)获取.
  */
DWDEFINE_INTERFACE(IYYBeginnerGuide): public IUnk
{
/**
   * @brief set application guide.
   * @param index app index.
   * @param enable is enable.
   */
	virtual void setAppGuide(int index,bool enable) = 0;
/**
   * @brief get application guide.
   * @param index app index.
   * @return true, means success, otherwise, fail.
   */
	virtual bool getAppGuide(int index) = 0;
/**
   * @brief check whether the guide is finished.
   * @return true, means success, otherwise, fail.
   */
	virtual bool getIsGuideFinished() = 0;
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_GUIDE_FINISHED;

/**
  * @brief get guide finished signal.
  */
	virtual _SIG_GUIDE_FINISHED& getGuideFinishedSignal() = 0;	
};