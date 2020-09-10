#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief �����ϱ��ӿ�, ͨ��root->getServiceInterface<IYYDataReport>(KSERVICEMainframe)����ȡ.
  */
DWDEFINE_INTERFACE(IYYDataReport) : public IUnk
{
    enum BusinessType				
    {
        DefaultType = 0,			//Ĭ��ֵ
        UserActionType = 0x01,		//�û���Ϊ��ͳ��
        SecurityInfoType = 0x02,	//��ȫ����Ϣ�ϱ�
        PerformanceInfoType = 0x03		//�������ϱ�
    };
/**
   * @brief ����ϱ�
   * @param pBigT big T.
   */
	virtual void ClickReport(LPCSTR pBigT) = 0;
/**
   * @brief TVL�ϱ�.
   * @param pBigT big T.
   * @param cSubT sub type.
   * @param wValue value.
   */
	virtual void TLVReport(LPCSTR pBigT, BYTE cSubT, WORD wValue) = 0;
/**
   * @brief httpʵʱ�ϱ�.
   * @param eBusinessID business id.
   * @param pReportBuf report buffer.
   * @param dwBufLen buffer length.
   * @param pActType act type.
   * @param dwBufferCode buffer code.
   */
    virtual void HttpRealTimeReport(BusinessType eBusinessID, LPCSTR pReportBuf, DWORD dwBufLen, LPCTSTR pActType, DWORD dwBufferCode) = 0;

	virtual void TLVReportEx(LPCSTR pBigT, BYTE cSubT, LPCSTR pCustomBuffer1, LPCSTR pCustomBuffer2, WORD wValue) = 0;

	virtual void HttpRealTimeReportEx(BusinessType uModeID, LPCSTR pReportBuf, DWORD dwBufLen, LPCTSTR pActType, DWORD dwBufferCode) = 0;

	// �¼������ϱ���
	// �ͻ��˻��棬��ʱ�����ϱ���
	// ����ֵ���Ƿ�ɹ���ʼ�����¼������ϱ���ֻ��SDK��������ܳɹ���
	// ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������򺣶�������Ч���¼�ID�����ȳ���255�ֽھͱ��ضϡ�
	virtual bool ReportEventTimes(const char* pcstrEventid) = 0;

	// ��׼�Զ��������ϱ���kv��ʽ����
	// ��SDK�����󣬲��ܵ������½ӿ���kv��ʽ���б�׼�Զ��������ϱ���
	// �����ϱ����첽�ġ�
	// ����ֵ���Ƿ�ɹ���ʼ�첽�ϱ���ǰ���ݡ�
	// ����pcstrType���������ͣ�����Ϊ�գ�����ʧ�ܣ����ǿյġ��������͡����Ϊact�������ݰ����������򺣶�������Чact��������ʹ��Сд��ĸ�����֣������ѱ�ʹ�á����ȳ���255�ֽھͱ��ضϡ�
	// ����pcstrOrigData�����ݣ�����Ϊ�գ�����ʧ�ܣ��������ݡ�Ҫ����kv��ʽ�����硰key1=value1&key2=value2&key3=value3������ѭURL�淶�������򺣶�������Ч���ֶ����ƺ��������ͣ���ο������ȷ������롱��
	// ����nLength�����ݵĳ��ȣ���λ�ֽڣ�����Ϊ�㣬����ʧ�ܣ������ȳ���4095�ֽھͱ��ضϡ�
	// ����bPersist���Ƿ����ӡ��ϱ�Ƶ�ʸ߻�����Ҫʱ��ʹ�ó����ӣ���������Ҫ�ͻ��˺ͷ���˶�֧�ֲ���Ч��
	virtual bool Report(const char* pcstrType,const char* pcstrOrigData,const unsigned int nLength,const bool bPersist) = 0;

	// ����ģʽ�ı�׼�Զ��������ϱ���kv��ʽ����
	// �ͻ��˻��棬��ʱ�����ϱ���
	// ��SDK�����󣬲��ܵ������½ӿ���kv��ʽ���б�׼�Զ��������ϱ���
	// �����ϱ����첽�ġ�
	// ����ֵ���Ƿ�ɹ���ʼ�첽�ϱ���ǰ���ݣ�����ģʽ����
	// ����pcstrType���������ͣ�����Ϊ�գ�����ʧ�ܣ����ǿյġ��������͡����Ϊact�������ݰ����������򺣶�������Чact��������ʹ��Сд��ĸ�����֣������ѱ�ʹ�á����ȳ���255�ֽھͱ��ضϡ�
	// ����pcstrOrigData�����ݣ�����Ϊ�գ�����ʧ�ܣ��������ݡ�Ҫ����kv��ʽ�����硰key1=value1&key2=value2&key3=value3������ѭURL�淶�������򺣶�������Ч���ֶ����ƺ��������ͣ���ο������ȷ������롱��
	// ����nLength�����ݵĳ��ȣ���λ�ֽڣ�����Ϊ�㣬����ʧ�ܣ������ȳ���4095�ֽھͱ��ضϡ�
	// ����bPersist���Ƿ����ӡ��ϱ�Ƶ�ʸ߻�����Ҫʱ��ʹ�ó����ӣ���������Ҫ�ͻ��˺ͷ���˶�֧�ֲ���Ч��
	virtual bool ReportExt(const char* pcstrType,const char* pcstrOrigData,const unsigned int nLength,const bool bPersist) = 0;

	// ���ܵı�׼�Զ��������ϱ���kv��ʽ����
	// ���ݱ����ܺ���ϱ���
	// ��SDK�����󣬲��ܵ������½ӿ���kv��ʽ���б�׼�Զ��������ϱ���
	// �����ϱ����첽�ġ�
	// ����ֵ���Ƿ�ɹ���ʼ�첽�ϱ���ǰ���ݡ�
	// ����pcstrType���������ͣ�����Ϊ�գ�����ʧ�ܣ����ǿյġ��������͡����Ϊact�������ݰ����������򺣶�������Чact��������ʹ��Сд��ĸ�����֣������ѱ�ʹ�á����ȳ���255�ֽھͱ��ضϡ�
	// ����pcstrOrigData�����ݣ�����Ϊ�գ�����ʧ�ܣ��������ݡ�Ҫ����kv��ʽ�����硰key1=value1&key2=value2&key3=value3������ѭURL�淶�������򺣶�������Ч���ֶ����ƺ��������ͣ���ο������ȷ������롱��
	// ����nLength�����ݵĳ��ȣ���λ�ֽڣ�����Ϊ�㣬����ʧ�ܣ������ȳ���4095�ֽھͱ��ضϡ�
	// ����bPersist���Ƿ����ӡ��ϱ�Ƶ�ʸ߻�����Ҫʱ��ʹ�ó����ӣ���������Ҫ�ͻ��˺ͷ���˶�֧�ֲ���Ч��
	virtual bool ReportEncrypt(const char* pcstrType,const char* pcstrOrigData,const unsigned int nLength,const bool bPersist) = 0;


    //////////////////////////////////////////////////////////////////////////
    // ��Ӫƽ̨PC��ͨ��Э�飨pcsdkprotocol��

    // ���ö�Ӧeventid�Ĳ�ƷID��Ĭ��Ϊ�ޡ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����pcstrProdid����ƷID�����ȳ���255�ֽھͱ��ضϡ�
    virtual bool SetProdidByEvent(const char* pcstrEventid, const char* pcstrProdid);

    // ���ö�Ӧeventid���ⲿ����������Դ�����û�����ù�����ʹ��ȫ�ֵ�ǰ��������
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����pcstrFrom���ⲿ����������Դ�����ȳ���255�ֽھͱ��ضϡ�
    virtual bool SetFromByEvent(const char* pcstrEventid, const char* pcstrFrom);

    // �����û���ǰ���ڵ�Ƶ��ID��Ĭ��Ϊ�ޡ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrSid��Ƶ��ID��
    virtual bool SetSid(quint32 sid);

    // ���ö�Ӧeventid��Ƶ��ID�����û�����ù�����ʹ���û���ǰ���ڵġ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����pcstrSid��Ƶ��ID��
    virtual bool SetSidByEvent(const char* pcstrEventid, quint32 sid);

    // �����û���ǰ���ڵ���Ƶ��ģ��ID��Ĭ��Ϊ�ޡ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrTempid����Ƶ��ģ��ID��
    virtual bool SetTempid(quint32 template_id);

    // ���ö�Ӧeventid����Ƶ��ģ��ID�����û�����ù�����ʹ���û���ǰ���ڵġ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����pcstrTempid����Ƶ��ģ��ID�����ֽ�������ʮ�����ַ��������߿��ַ�����ʾ��գ���������Ϲ淶�ͷ���ʧ�ܡ�
    virtual bool SetTempidByEvent(const char* pcstrEventid, quint32 template_id);

    // �����û���ǰ���ڵ���Ƶ��ID��Ĭ��Ϊ�ޡ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrSubsid����Ƶ��ID��
    virtual bool SetSubsid(quint32 ssid);

    // ���ö�Ӧeventid����Ƶ��ID�����û�����ù�����ʹ���û���ǰ���ڵġ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����pcstrSubsid����Ƶ��ID��
    virtual bool SetSubsidByEvent(const char* pcstrEventid, quint32 ssid);

    // �����û���ǰ���ڵ���Ƶ��ģ��ID��Ĭ��Ϊ�ޡ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrSubtempid����Ƶ��ģ��ID��
    virtual bool SetSubtempid(quint32 template_id);

    // ���ö�Ӧeventid����Ƶ��ģ��ID�����û�����ù�����ʹ���û���ǰ���ڵġ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����pcstrSubtempid����Ƶ��ģ��ID�����ֽ�������ʮ�����ַ��������߿��ַ�����ʾ��գ���������Ϲ淶�ͷ���ʧ�ܡ�
    virtual bool SetSubtempidByEvent(const char* pcstrEventid, quint32 template_id);

    // ���ö�Ӧeventid������UID��Ĭ��Ϊ�ޡ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����pcstrHostid������UID��
    virtual bool SetHostid(const char* pcstrEventid, quint32 usrid);

    // ���ö�Ӧeventid���¼�ֵͳ�ƶ���Ĭ��Ϊ�ޡ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����pcstrStatisobj���¼�ֵͳ�ƶ��󡣳��ȳ���255�ֽھͱ��ضϡ�
    virtual bool SetStatisobj(const char* pcstrEventid, const char* pcstrStatisobj);

    // ���ö�Ӧeventid���ڲ����ID��Ĭ��Ϊ�ޡ�
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����pcstrEntranid���ڲ����ID�����ȳ���255�ֽھͱ��ضϡ�
    virtual bool SetEntranid(const char* pcstrEventid, const char* pcstrEntranid);

    // ���ö�Ӧeventid��moreinfo��Ĭ��Ϊ�ޣ���ҵ��֤ʹ��JSON��ʽ�ַ�����
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����pcstrMoreinfo��������Ϣ����ҵ��֤ʹ��JSON��ʽ�ַ��������ȳ���4095�ֽھͱ��ضϡ�ע�⣺��Ϊ���ܰ���Ӱ�캣���ϱ����ַ�������SDK�Զ�����urlencode��
    virtual bool SetMoreinfo(const char* pcstrEventid, const char* pcstrMoreinfo);

    // ���ö�Ӧeventid��region��Ĭ��Ϊ1��
    // ����ֵ���Ƿ�ɹ����á�
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����iRegion�������־��
    virtual bool SetRegion(const char* pcstrEventid, const int iRegion);

    // �������ϱ���
    // �����£�������ʱ���ϱ���������ʱ����Ӧ�¼�ID�������ϱ�0ֵ��Ȼ��ʱ�ϱ��������������QuitReportSDKʱ�ϱ��������������Ϊ0��ʾֹͣ��ʱ����
    // ����ֵ���Ƿ�ɹ���ʼ�첽�ϱ�������������
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����nSecondInterval���������ʱ�䣬��λΪ�롣���Ϊ0��ʾֹͣ��ʱ����
    virtual bool ReportHeart(const char* pcstrEventid, const unsigned int nSecondInterval);

    // �¼�ֵ�ϱ���
    // ����ֵ���Ƿ�ɹ���ʼ�첽�ϱ���
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����value���¼���ֵ���Զ��������뱣�������λС����
    virtual bool ReportAmount(const char* pcstrEventid, const double value);

    // �Ƿ��ϱ���
    // ֵ��Ϊ1��
    // ����ֵ���Ƿ�ɹ���ʼ�첽�ϱ���
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    virtual bool ReportJudge(const char* pcstrEventid);

    // ·���ϱ���
    // ����ֵ���Ƿ�ɹ���ʼ�첽�ϱ���
    // ����pcstrEventid���¼�ID������Ϊ�գ�����ʧ�ܣ������ȳ���255�ֽھͱ��ضϡ�
    // ����value��·����ֵ���Զ��������뱣�������λС����
    virtual bool ReportProcess(const char* pcstrEventid, const double value);

};

class QVariant;
DWDEFINE_INTERFACE(IYYStatistics): public IUnk
{
	virtual void report(const std::vector<QVariant> &data) = 0;
};
