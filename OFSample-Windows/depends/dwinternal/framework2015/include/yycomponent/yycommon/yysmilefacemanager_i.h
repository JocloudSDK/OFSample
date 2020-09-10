#pragma once
#include "dwbase/dwcomex.h"
#include "duifw/duiwidget.h"
#include "yycomponent/yycomclsid.h"

/* signals:
	void visibledChagned(bool);
	void smileSelected(const IYYSmileFaceManager::YYSmileData& data, const QString& keyID );
	void smileFocusout(Qt::FocusReason);

	// 当yy需要向组件查询表情资源时引发
	// 组件通过接口updateSmiles返回给yy，mask为KUpdateResource
	void needSmilesResource(const QString& strTabText, const QVector<QString>& keys); 

	//name为要显示的tab页的名字 (系统表情页为defaultgroup)
	//type is IYYSmileFaceManager::SceneType (ImGroupScene or ChannelScene)
	void smileFaceClassTabSelectedChanged(const QString& name, quint32 type); 
    
    // 气泡和普通模式改变
    void messageStyleChanged();
*/

/**
* @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE( IYYSmileFaceManager ) : public IDWQComponent
{
	enum SmileType
	{
		DefaultType,    // 系统默认表情
		UserType,       // 自定义表情.
		MagicType,      // 魔法表情?

		Reserved1 = 0x100,
	};

	/**
	* @brief YY表情描述.
	* @param smileKey 快捷键.
	* @param smileFile 文件名.
	* @param description 描述.
	* @param type 表情类型，具体请看SMILE_DATA_TYPE.
	* @param downloadUrl 已上传到服务器的对应地址，如不为空，表示已经成功上传到服务器.
	* @param previewFile 用于预览的图片文件名.
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
	* @param font 字体.
	* @param color 字体颜色.
	* @param personalIndex 炫彩字.
	* @param fontStyle 字体/炫彩字.
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
	* @brief 显示表情框.
	* @param pWidget 点击显示表情框的按钮，当表情框隐藏时，恢复按钮的末点击状态.
	* @param keyID 表情框所在的业务类型，例如im、群.
	*/
	virtual void showSmileFaceFrame(DuiWidget* pWidget, const QString& keyID) = 0;

	/**
	* @brief 显示最近使用过的表情框.
	* @param pWidget 点击显示表情框的按钮，当表情框隐藏时，恢复按钮的末点击状态.
	* @param keyID 表情框所在的业务类型，例如im、群.
	*/
	virtual void showRecentFaceFrame(DuiWidget* pWidget, const QString& strKeyId) = 0;

	/**
	* @brief 显示增加自定义表情对话框.
	* @param myUid 自己的uid.
	* @param parent 父窗口.
	* @param strFile 自定义表情文件路径.
	* @param strGroupName 表情所在的分组.
	*/
	virtual void showAddSmileFaceDialog(quint32 myUid, QWidget* parent, const QString& strFile, const QString& strGroupName) = 0;

	/**
	* @brief 关闭表情框与最近使用过的表情框.
	* @return.
	*/
	virtual void clear() = 0;

	/**
	* @brief 通过快捷键查找表情信息，适用于所有快捷键，不保证绝对成功查询，一定机率失败（取决于数据库繁忙程度）.
	* @param strShortcut 快捷键.
	* @return .
	*/
	virtual YYSmileData getByShortcut(const QString& strShortcut) = 0;

	/**
	* @brief 所有表情分组显示到表情框中，并显示出来.
	* @param pWidget 点击显示表情框的按钮，当表情框隐藏时，恢复按钮的末点击状态.
	* @param componentID 表情框所在的业务类型，例如im、群.
	*/
	virtual void showAllSmileFrame(DuiWidget* pWidget, const QString& componentID) = 0;

	enum AddedWidgetType
	{
		AddToImGroup,
		AddToChannel,
	};

	/**
	* @brief . 向表情窗口添加一个tab，该tab切换时显示的内容是w的内容。
	为了让YY不依赖组件，而又能向YY插入表情tab，加入以下几个接口。
	addSmileWidget向yy添加tab及显示内容，传入的widget由组件自行定制，yy表情界面只提供一个显示的容器；
	对于最近使用表情的显示列表，由于组件有部分表情可能因为各种原因（到期等）不让用户使用，
	可以通过updateSmiles来禁用(mask为KRecentDisabled)，yy收到这份列表时，会以这份列表来过滤最近表情界面的最近使用表情列表。
	所以，当组件初始化时，或有表情使用权限之类的变化需要通知到YY，因此同时，组件需要维护一份自己的表情的最近使用列表，
	列表数量上限应以im/group/channel中的最大上限为基准。
	* @param . w 切换tab后，显示的widget内容
	* @param . type  表示添加到的地方，0x1表示添加到im/group，0x2表示添加到频道.
	* @param . strTabText tab名字，请保证唯一。用于区别不同的tab
	* @return .
	********************************************************
	注意：此接口仅为游戏表情组件提供。
	请用新接口addSmileWidget2
	已经插入的widget请保持objectName不要改变!
	*********************************************************
	*/
	virtual void addSmileWidget(DuiWidget* w, AddedWidgetType type, const QString& strTabText);

	/**
	* @brief 向最近打开的界面的输入框插入表情.
	* @param strTabText 插入yy表情时的tab名字，与addSmileWidget中的参数strTabText一样,用于区别不同的tab.
	* @param data 目前其中type为Reserved1或大于Reserved1的数值，smileKey字段有效，其他字段忽略。.
	*/
	virtual void insertSmile(const QString& strTabText, const YYSmileData& data);

	/**
	* @brief 表示列表是禁用列表，用于令列表中的表情在yy最近使用表情中不显示.
	*/
	static const int KRecentDisabled = 0x1; 

	/**
	* @brief 表示列表是更新资源的列表，用于组件通知yy表情资源，yy最近使用表情显示时用到.
	* 这时，YYSmileData中的key，smileFile字段有效.
	*/
	static const int KUpdateResource = 0x2;
 
	/**
	* @brief  更新表情列表.
	* @param  strTabText 插入yy表情时的tab名字，与addSmileWidget中的参数strTabText一样。用于区别不同的tab.
	* @param  mask 表示列表用途.
	* @param  smiles  表情信息列表.
	*/
	virtual void updateSmiles(const QString& strTabText, int mask, const std::vector<YYSmileData>& smiles);

	/**
	* @brief 得到所有系统表情信息列表.
	* @param smiles  表情信息列表.
	* @return
	*/
	virtual void getSystemSmileInfo(QVector<YYSmileData>& smiles) = 0;

	/**
	* @brief 得到所有系统表情信息列表.
	* @param imagePath  表情文件路径.
	* @return 返回系统表情二进制数据.
	*/
	virtual QByteArray getSystemSmileImage(const QString imagePath) = 0;

    /**
    * @brief  显示/隐藏右侧VIP表情推荐栏.
    * @param  bVisible  false表示不显示，true表示还原默认的显示控制逻辑.
    */
    virtual void setRightPartVisible(bool bVisible) = 0;
		
	/**
	* @brief 指定文本里面是否存在系统表情.
	* @param text 指定的文本.
	* @return 是否存在系统表情.
	*/
	virtual bool hasSystemEmotion(const QString& text) = 0;

	/**
	* @brief . 功能与addSmileWidget一致，但提供了iconPath和preferedPos参数，用来在tab中插入图标以及区分不同组件插入表情页的位置
	*		插入的widget请以组件别名命名,方便查找.
	* @param iconPath tab中的图标路径，尺寸最小为18X18.
	* @param preferedPos是指定位置值，该值越大约往后插 。其他参数见addSmileWidget.
	*/
	virtual void addSmileWidget2(DuiWidget* w, AddedWidgetType type, const QString& strTabText, const QString& iconPath, int preferedPos) = 0;

	 /**
    * @brief  插入widget到表情框.
    * @param  w  插入widget的指针.
	* @param  preferedPos  插入widget的位置.
	* @param  type  表情框场景(ImGroupScene,ChannelScene).
    */
    virtual void insertWidgetToFrame(DuiWidget *w, int preferedPos, SceneType type) = 0;

	/**
    * @brief 显示/隐藏插入到widget的表情框.
	* @param name  要显示/隐藏插入的widget的objectName.
    * @param isVisible  false表示不显示，true表示显示.
	* @param type  表情框场景((ImGroupScene,ChannelScene)) 如果type为频道且没有加入频道，则直接返回.
    */
    virtual void setFrameWidgetVisible(const QString& name, bool isVisible, SceneType type) = 0;

	/**
    * @brief 切换表情框页面到指定widget.
	* @param name  要切换到的widget名字 (系统表情页为defaultgroup).
	* @param type  表情框场景((ImGroupScene,ChannelScene)) 如果type为频道且没有加入频道，则直接返回.
    */
    virtual void setCurrentSmileWidget(const QString& name, SceneType type) = 0;

	/**
    * @brief  通过指定的frame的id打开对应的表情框.
	* @param  id frame的id.
	* @return SceneType (ImScene, GroupScene, ChannelScene).
    */
	virtual SceneType showSmileFaceFrameById(quint32 id, const QString& componentID) = 0;


    /**
    * @brief  在表情的tab上显示一个红点，点击之后红点自动消失.
	* @param  widgetName tab对应widget的objectName.
    */
    virtual void addRedPoint(const QString& widgetName) = 0;

	 /**
    * @brief  更新调情窗口tab项图标.
	* @param  strGroupName tab上面对应的group name.
	* @param  strIconPath tab上面对应group icon路径.
    */
    virtual void updateTabGroup(const QString& strGroupName,const QString & strIconPath) = 0;
};