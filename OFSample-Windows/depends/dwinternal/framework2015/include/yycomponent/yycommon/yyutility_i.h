#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYUtility): public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void openUrlWithPostData( const std::wstring &url, const std::wstring &postData ) = 0;

/**
   * @brief .
   * @return .
   */
	virtual bool isYYAutoRun() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setYYAutoRun(bool bAutoRun) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual unsigned int getConstellationIndex(unsigned int nMath, unsigned int nDay) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int levelFromPoints(int points) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int pointsFromLevel(int level) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int hoursToUpgrade(int points) = 0;
};

/**
  * @brief use root->getServiceInterface<ICheckUrlStatus>(KSERVICECommon).
*/
DWDEFINE_INTERFACE(IYYCheckUrlStatus): public IDWQComponent
{
	/**
	 * @brief 异步查询url安全状态.
	 */
	virtual void asyncCheckUrl(const QString& url) = 0;

	/*
	 * @brief status的值分别是：0代表安全、1代表unknown、2代表危险.
	 */
	typedef _def_boost_signals2_signal_type<void (const QString&, int)>::type _SIG_URL_CHECKED;
	virtual _SIG_URL_CHECKED* getUrlCheckedSignal() = 0;
};

DWDEFINE_INTERFACE(IYYCommitGameInfo): public IDWQComponent
{
	virtual void startCheckGame() = 0;
	virtual void clearUp() = 0;
};