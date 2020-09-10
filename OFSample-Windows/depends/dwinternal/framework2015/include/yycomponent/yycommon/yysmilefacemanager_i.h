#pragma once
#include "dwbase/dwcomex.h"
#include "duifw/duiwidget.h"
#include "yycomponent/yycomclsid.h"

/* signals:
	void visibledChagned(bool);
	void smileSelected(const IYYSmileFaceManager::YYSmileData& data, const QString& keyID );
	void smileFocusout(Qt::FocusReason);

	// ��yy��Ҫ�������ѯ������Դʱ����
	// ���ͨ���ӿ�updateSmiles���ظ�yy��maskΪKUpdateResource
	void needSmilesResource(const QString& strTabText, const QVector<QString>& keys); 

	//nameΪҪ��ʾ��tabҳ������ (ϵͳ����ҳΪdefaultgroup)
	//type is IYYSmileFaceManager::SceneType (ImGroupScene or ChannelScene)
	void smileFaceClassTabSelectedChanged(const QString& name, quint32 type); 
    
    // ���ݺ���ͨģʽ�ı�
    void messageStyleChanged();
*/

/**
* @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE( IYYSmileFaceManager ) : public IDWQComponent
{
	enum SmileType
	{
		DefaultType,    // ϵͳĬ�ϱ���
		UserType,       // �Զ������.
		MagicType,      // ħ������?

		Reserved1 = 0x100,
	};

	/**
	* @brief YY��������.
	* @param smileKey ��ݼ�.
	* @param smileFile �ļ���.
	* @param description ����.
	* @param type �������ͣ������뿴SMILE_DATA_TYPE.
	* @param downloadUrl ���ϴ����������Ķ�Ӧ��ַ���粻Ϊ�գ���ʾ�Ѿ��ɹ��ϴ���������.
	* @param previewFile ����Ԥ����ͼƬ�ļ���.
	*/
	struct YYSmileData
	{
		SmileType type;
		QString	smileKey;				
		QString smileFile;				
		QString description;			
		QString downloadUrl;			
		QString previewFile;		
	};

	enum FontStyle
	{
		fontSys = 0,
		fontPersonal = 1,
	};

	/**
	* @brief . 
	* @param font ����.
	* @param color ������ɫ.
	* @param personalIndex �Ų���.
	* @param fontStyle ����/�Ų���.
	*/
	struct YYFontConfig
	{
		QFont       font;               
		QColor      color;              
		int         personalIndex;      
		FontStyle   fontStyle;          
	};

	enum SceneType
	{
		InvalidType = 0x0,
		ImScene = 0x1,
		GroupScene = 0x2,
		ImGroupScene = 0x3,
		ChannelScene = 0x4
	};

	/**
	* @brief ��ʾ�����.
	* @param pWidget �����ʾ�����İ�ť�������������ʱ���ָ���ť��ĩ���״̬.
	* @param keyID ��������ڵ�ҵ�����ͣ�����im��Ⱥ.
	*/
	virtual void showSmileFaceFrame(DuiWidget* pWidget, const QString& keyID) = 0;

	/**
	* @brief ��ʾ���ʹ�ù��ı����.
	* @param pWidget �����ʾ�����İ�ť�������������ʱ���ָ���ť��ĩ���״̬.
	* @param keyID ��������ڵ�ҵ�����ͣ�����im��Ⱥ.
	*/
	virtual void showRecentFaceFrame(DuiWidget* pWidget, const QString& strKeyId) = 0;

	/**
	* @brief ��ʾ�����Զ������Ի���.
	* @param myUid �Լ���uid.
	* @param parent ������.
	* @param strFile �Զ�������ļ�·��.
	* @param strGroupName �������ڵķ���.
	*/
	virtual void showAddSmileFaceDialog(quint32 myUid, QWidget* parent, const QString& strFile, const QString& strGroupName) = 0;

	/**
	* @brief �رձ���������ʹ�ù��ı����.
	* @return.
	*/
	virtual void clear() = 0;

	/**
	* @brief ͨ����ݼ����ұ�����Ϣ�����������п�ݼ�������֤���Գɹ���ѯ��һ������ʧ�ܣ�ȡ�������ݿⷱæ�̶ȣ�.
	* @param strShortcut ��ݼ�.
	* @return .
	*/
	virtual YYSmileData getByShortcut(const QString& strShortcut) = 0;

	/**
	* @brief ���б��������ʾ��������У�����ʾ����.
	* @param pWidget �����ʾ�����İ�ť�������������ʱ���ָ���ť��ĩ���״̬.
	* @param componentID ��������ڵ�ҵ�����ͣ�����im��Ⱥ.
	*/
	virtual void showAllSmileFrame(DuiWidget* pWidget, const QString& componentID) = 0;

	enum AddedWidgetType
	{
		AddToImGroup,
		AddToChannel,
	};

	/**
	* @brief . ����鴰�����һ��tab����tab�л�ʱ��ʾ��������w�����ݡ�
	Ϊ����YY�������������������YY�������tab���������¼����ӿڡ�
	addSmileWidget��yy���tab����ʾ���ݣ������widget��������ж��ƣ�yy�������ֻ�ṩһ����ʾ��������
	�������ʹ�ñ������ʾ�б���������в��ֱ��������Ϊ����ԭ�򣨵��ڵȣ������û�ʹ�ã�
	����ͨ��updateSmiles������(maskΪKRecentDisabled)��yy�յ�����б�ʱ����������б���������������������ʹ�ñ����б�
	���ԣ��������ʼ��ʱ�����б���ʹ��Ȩ��֮��ı仯��Ҫ֪ͨ��YY�����ͬʱ�������Ҫά��һ���Լ��ı�������ʹ���б�
	�б���������Ӧ��im/group/channel�е��������Ϊ��׼��
	* @param . w �л�tab����ʾ��widget����
	* @param . type  ��ʾ��ӵ��ĵط���0x1��ʾ��ӵ�im/group��0x2��ʾ��ӵ�Ƶ��.
	* @param . strTabText tab���֣��뱣֤Ψһ����������ͬ��tab
	* @return .
	********************************************************
	ע�⣺�˽ӿڽ�Ϊ��Ϸ��������ṩ��
	�����½ӿ�addSmileWidget2
	�Ѿ������widget�뱣��objectName��Ҫ�ı�!
	*********************************************************
	*/
	virtual void addSmileWidget(DuiWidget* w, AddedWidgetType type, const QString& strTabText);

	/**
	* @brief ������򿪵Ľ���������������.
	* @param strTabText ����yy����ʱ��tab���֣���addSmileWidget�еĲ���strTabTextһ��,��������ͬ��tab.
	* @param data Ŀǰ����typeΪReserved1�����Reserved1����ֵ��smileKey�ֶ���Ч�������ֶκ��ԡ�.
	*/
	virtual void insertSmile(const QString& strTabText, const YYSmileData& data);

	/**
	* @brief ��ʾ�б��ǽ����б��������б��еı�����yy���ʹ�ñ����в���ʾ.
	*/
	static const int KRecentDisabled = 0x1; 

	/**
	* @brief ��ʾ�б��Ǹ�����Դ���б��������֪ͨyy������Դ��yy���ʹ�ñ�����ʾʱ�õ�.
	* ��ʱ��YYSmileData�е�key��smileFile�ֶ���Ч.
	*/
	static const int KUpdateResource = 0x2;
 
	/**
	* @brief  ���±����б�.
	* @param  strTabText ����yy����ʱ��tab���֣���addSmileWidget�еĲ���strTabTextһ������������ͬ��tab.
	* @param  mask ��ʾ�б���;.
	* @param  smiles  ������Ϣ�б�.
	*/
	virtual void updateSmiles(const QString& strTabText, int mask, const std::vector<YYSmileData>& smiles);

	/**
	* @brief �õ�����ϵͳ������Ϣ�б�.
	* @param smiles  ������Ϣ�б�.
	* @return
	*/
	virtual void getSystemSmileInfo(QVector<YYSmileData>& smiles) = 0;

	/**
	* @brief �õ�����ϵͳ������Ϣ�б�.
	* @param imagePath  �����ļ�·��.
	* @return ����ϵͳ�������������.
	*/
	virtual QByteArray getSystemSmileImage(const QString imagePath) = 0;

    /**
    * @brief  ��ʾ/�����Ҳ�VIP�����Ƽ���.
    * @param  bVisible  false��ʾ����ʾ��true��ʾ��ԭĬ�ϵ���ʾ�����߼�.
    */
    virtual void setRightPartVisible(bool bVisible) = 0;
		
	/**
	* @brief ָ���ı������Ƿ����ϵͳ����.
	* @param text ָ�����ı�.
	* @return �Ƿ����ϵͳ����.
	*/
	virtual bool hasSystemEmotion(const QString& text) = 0;

	/**
	* @brief . ������addSmileWidgetһ�£����ṩ��iconPath��preferedPos������������tab�в���ͼ���Լ����ֲ�ͬ����������ҳ��λ��
	*		�����widget���������������,�������.
	* @param iconPath tab�е�ͼ��·�����ߴ���СΪ18X18.
	* @param preferedPos��ָ��λ��ֵ����ֵԽ��Լ����� ������������addSmileWidget.
	*/
	virtual void addSmileWidget2(DuiWidget* w, AddedWidgetType type, const QString& strTabText, const QString& iconPath, int preferedPos) = 0;

	 /**
    * @brief  ����widget�������.
    * @param  w  ����widget��ָ��.
	* @param  preferedPos  ����widget��λ��.
	* @param  type  ����򳡾�(ImGroupScene,ChannelScene).
    */
    virtual void insertWidgetToFrame(DuiWidget *w, int preferedPos, SceneType type) = 0;

	/**
    * @brief ��ʾ/���ز��뵽widget�ı����.
	* @param name  Ҫ��ʾ/���ز����widget��objectName.
    * @param isVisible  false��ʾ����ʾ��true��ʾ��ʾ.
	* @param type  ����򳡾�((ImGroupScene,ChannelScene)) ���typeΪƵ����û�м���Ƶ������ֱ�ӷ���.
    */
    virtual void setFrameWidgetVisible(const QString& name, bool isVisible, SceneType type) = 0;

	/**
    * @brief �л������ҳ�浽ָ��widget.
	* @param name  Ҫ�л�����widget���� (ϵͳ����ҳΪdefaultgroup).
	* @param type  ����򳡾�((ImGroupScene,ChannelScene)) ���typeΪƵ����û�м���Ƶ������ֱ�ӷ���.
    */
    virtual void setCurrentSmileWidget(const QString& name, SceneType type) = 0;

	/**
    * @brief  ͨ��ָ����frame��id�򿪶�Ӧ�ı����.
	* @param  id frame��id.
	* @return SceneType (ImScene, GroupScene, ChannelScene).
    */
	virtual SceneType showSmileFaceFrameById(quint32 id, const QString& componentID) = 0;


    /**
    * @brief  �ڱ����tab����ʾһ����㣬���֮�����Զ���ʧ.
	* @param  widgetName tab��Ӧwidget��objectName.
    */
    virtual void addRedPoint(const QString& widgetName) = 0;

	 /**
    * @brief  ���µ��鴰��tab��ͼ��.
	* @param  strGroupName tab�����Ӧ��group name.
	* @param  strIconPath tab�����Ӧgroup icon·��.
    */
    virtual void updateTabGroup(const QString& strGroupName,const QString & strIconPath) = 0;
};