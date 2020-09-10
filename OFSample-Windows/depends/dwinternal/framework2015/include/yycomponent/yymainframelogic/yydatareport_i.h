#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief 数据上报接口, 通过root->getServiceInterface<IYYDataReport>(KSERVICEMainframe)来获取.
  */
DWDEFINE_INTERFACE(IYYDataReport) : public IUnk
{
    enum BusinessType				
    {
        DefaultType = 0,			//默认值
        UserActionType = 0x01,		//用户行为类统计
        SecurityInfoType = 0x02,	//安全类信息上报
        PerformanceInfoType = 0x03		//性能类上报
    };
/**
   * @brief 点击上报
   * @param pBigT big T.
   */
	virtual void ClickReport(LPCSTR pBigT) = 0;
/**
   * @brief TVL上报.
   * @param pBigT big T.
   * @param cSubT sub type.
   * @param wValue value.
   */
	virtual void TLVReport(LPCSTR pBigT, BYTE cSubT, WORD wValue) = 0;
/**
   * @brief http实时上报.
   * @param eBusinessID business id.
   * @param pReportBuf report buffer.
   * @param dwBufLen buffer length.
   * @param pActType act type.
   * @param dwBufferCode buffer code.
   */
    virtual void HttpRealTimeReport(BusinessType eBusinessID, LPCSTR pReportBuf, DWORD dwBufLen, LPCTSTR pActType, DWORD dwBufferCode) = 0;

	virtual void TLVReportEx(LPCSTR pBigT, BYTE cSubT, LPCSTR pCustomBuffer1, LPCSTR pCustomBuffer2, WORD wValue) = 0;

	virtual void HttpRealTimeReportEx(BusinessType uModeID, LPCSTR pReportBuf, DWORD dwBufLen, LPCTSTR pActType, DWORD dwBufferCode) = 0;

	// 事件次数上报：
	// 客户端缓存，定时定量上报。
	// 返回值：是否成功开始本次事件次数上报。只当SDK启动后才能成功。
	// 参数pcstrEventid：事件ID（不能为空，否则失败）。请向海度申请有效的事件ID。长度超过255字节就被截断。
	virtual bool ReportEventTimes(const char* pcstrEventid) = 0;

	// 标准自定义数据上报（kv格式）：
	// 在SDK启动后，才能调用如下接口以kv格式进行标准自定义数据上报。
	// 数据上报是异步的。
	// 返回值：是否成功开始异步上报当前数据。
	// 参数pcstrType：数据类型（不能为空，否则失败）。非空的“数据类型”会成为act名、数据包名。必须向海度申请有效act名，建议使用小写字母和数字，不能已被使用。长度超过255字节就被截断。
	// 参数pcstrOrigData：数据（不能为空，否则失败）。“数据”要求是kv格式，形如“key1=value1&key2=value2&key3=value3”，遵循URL规范。必须向海度申请有效的字段名称和数据类型，请参考“海度发包申请”。
	// 参数nLength：数据的长度（单位字节，不能为零，否则失败）。长度超过4095字节就被截断。
	// 参数bPersist：是否长连接。上报频率高或者需要时才使用长连接，长连接需要客户端和服务端都支持才有效。
	virtual bool Report(const char* pcstrType,const char* pcstrOrigData,const unsigned int nLength,const bool bPersist) = 0;

	// 缓存模式的标准自定义数据上报（kv格式）：
	// 客户端缓存，定时定量上报。
	// 在SDK启动后，才能调用如下接口以kv格式进行标准自定义数据上报。
	// 数据上报是异步的。
	// 返回值：是否成功开始异步上报当前数据（缓存模式）。
	// 参数pcstrType：数据类型（不能为空，否则失败）。非空的“数据类型”会成为act名、数据包名。必须向海度申请有效act名，建议使用小写字母和数字，不能已被使用。长度超过255字节就被截断。
	// 参数pcstrOrigData：数据（不能为空，否则失败）。“数据”要求是kv格式，形如“key1=value1&key2=value2&key3=value3”，遵循URL规范。必须向海度申请有效的字段名称和数据类型，请参考“海度发包申请”。
	// 参数nLength：数据的长度（单位字节，不能为零，否则失败）。长度超过4095字节就被截断。
	// 参数bPersist：是否长连接。上报频率高或者需要时才使用长连接，长连接需要客户端和服务端都支持才有效。
	virtual bool ReportExt(const char* pcstrType,const char* pcstrOrigData,const unsigned int nLength,const bool bPersist) = 0;

	// 加密的标准自定义数据上报（kv格式）：
	// 数据被加密后才上报。
	// 在SDK启动后，才能调用如下接口以kv格式进行标准自定义数据上报。
	// 数据上报是异步的。
	// 返回值：是否成功开始异步上报当前数据。
	// 参数pcstrType：数据类型（不能为空，否则失败）。非空的“数据类型”会成为act名、数据包名。必须向海度申请有效act名，建议使用小写字母和数字，不能已被使用。长度超过255字节就被截断。
	// 参数pcstrOrigData：数据（不能为空，否则失败）。“数据”要求是kv格式，形如“key1=value1&key2=value2&key3=value3”，遵循URL规范。必须向海度申请有效的字段名称和数据类型，请参考“海度发包申请”。
	// 参数nLength：数据的长度（单位字节，不能为零，否则失败）。长度超过4095字节就被截断。
	// 参数bPersist：是否长连接。上报频率高或者需要时才使用长连接，长连接需要客户端和服务端都支持才有效。
	virtual bool ReportEncrypt(const char* pcstrType,const char* pcstrOrigData,const unsigned int nLength,const bool bPersist) = 0;


    //////////////////////////////////////////////////////////////////////////
    // 运营平台PC端通用协议（pcsdkprotocol）

    // 设置对应eventid的产品ID，默认为无。
    // 返回值：是否成功设置。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数pcstrProdid：产品ID。长度超过255字节就被截断。
    virtual bool SetProdidByEvent(const char* pcstrEventid, const char* pcstrProdid);

    // 设置对应eventid的外部渠道引流来源；如果没有设置过，则使用全局当前的渠道。
    // 返回值：是否成功设置。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数pcstrFrom：外部渠道引流来源。长度超过255字节就被截断。
    virtual bool SetFromByEvent(const char* pcstrEventid, const char* pcstrFrom);

    // 设置用户当前所在的频道ID，默认为无。
    // 返回值：是否成功设置。
    // 参数pcstrSid：频道ID。
    virtual bool SetSid(quint32 sid);

    // 设置对应eventid的频道ID；如果没有设置过，则使用用户当前所在的。
    // 返回值：是否成功设置。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数pcstrSid：频道ID。
    virtual bool SetSidByEvent(const char* pcstrEventid, quint32 sid);

    // 设置用户当前所在的主频道模板ID，默认为无。
    // 返回值：是否成功设置。
    // 参数pcstrTempid：主频道模板ID。
    virtual bool SetTempid(quint32 template_id);

    // 设置对应eventid的主频道模板ID；如果没有设置过，则使用用户当前所在的。
    // 返回值：是否成功设置。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数pcstrTempid：主频道模板ID。八字节整数的十进制字符串，或者空字符串表示清空；如果不符合规范就返回失败。
    virtual bool SetTempidByEvent(const char* pcstrEventid, quint32 template_id);

    // 设置用户当前所在的子频道ID，默认为无。
    // 返回值：是否成功设置。
    // 参数pcstrSubsid：子频道ID。
    virtual bool SetSubsid(quint32 ssid);

    // 设置对应eventid的子频道ID；如果没有设置过，则使用用户当前所在的。
    // 返回值：是否成功设置。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数pcstrSubsid：子频道ID。
    virtual bool SetSubsidByEvent(const char* pcstrEventid, quint32 ssid);

    // 设置用户当前所在的子频道模板ID，默认为无。
    // 返回值：是否成功设置。
    // 参数pcstrSubtempid：子频道模板ID。
    virtual bool SetSubtempid(quint32 template_id);

    // 设置对应eventid的子频道模板ID；如果没有设置过，则使用用户当前所在的。
    // 返回值：是否成功设置。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数pcstrSubtempid：子频道模板ID。八字节整数的十进制字符串，或者空字符串表示清空；如果不符合规范就返回失败。
    virtual bool SetSubtempidByEvent(const char* pcstrEventid, quint32 template_id);

    // 设置对应eventid的主播UID，默认为无。
    // 返回值：是否成功设置。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数pcstrHostid：主播UID。
    virtual bool SetHostid(const char* pcstrEventid, quint32 usrid);

    // 设置对应eventid的事件值统计对象，默认为无。
    // 返回值：是否成功设置。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数pcstrStatisobj：事件值统计对象。长度超过255字节就被截断。
    virtual bool SetStatisobj(const char* pcstrEventid, const char* pcstrStatisobj);

    // 设置对应eventid的内部入口ID，默认为无。
    // 返回值：是否成功设置。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数pcstrEntranid：内部入口ID。长度超过255字节就被截断。
    virtual bool SetEntranid(const char* pcstrEventid, const char* pcstrEntranid);

    // 设置对应eventid的moreinfo，默认为无，由业务保证使用JSON格式字符串。
    // 返回值：是否成功设置。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数pcstrMoreinfo：附加信息。由业务保证使用JSON格式字符串。长度超过4095字节就被截断。注意：因为可能包含影响海度上报的字符，所以SDK自动进行urlencode。
    virtual bool SetMoreinfo(const char* pcstrEventid, const char* pcstrMoreinfo);

    // 设置对应eventid的region，默认为1。
    // 返回值：是否成功设置。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数iRegion：分组标志。
    virtual bool SetRegion(const char* pcstrEventid, const int iRegion);

    // 心跳包上报：
    // （重新）启动定时器上报心跳，定时器对应事件ID；首先上报0值，然后定时上报间隔秒数，调用QuitReportSDK时上报重置秒数；间隔为0表示停止定时器。
    // 返回值：是否成功开始异步上报和启动心跳。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数nSecondInterval：心跳间隔时间，单位为秒。间隔为0表示停止定时器。
    virtual bool ReportHeart(const char* pcstrEventid, const unsigned int nSecondInterval);

    // 事件值上报：
    // 返回值：是否成功开始异步上报。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数value：事件数值。自动四舍五入保留最多六位小数。
    virtual bool ReportAmount(const char* pcstrEventid, const double value);

    // 是否上报：
    // 值恒为1。
    // 返回值：是否成功开始异步上报。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    virtual bool ReportJudge(const char* pcstrEventid);

    // 路径上报：
    // 返回值：是否成功开始异步上报。
    // 参数pcstrEventid：事件ID（不能为空，否则失败）。长度超过255字节就被截断。
    // 参数value：路径数值。自动四舍五入保留最多六位小数。
    virtual bool ReportProcess(const char* pcstrEventid, const double value);

};

class QVariant;
DWDEFINE_INTERFACE(IYYStatistics): public IUnk
{
	virtual void report(const std::vector<QVariant> &data) = 0;
};
