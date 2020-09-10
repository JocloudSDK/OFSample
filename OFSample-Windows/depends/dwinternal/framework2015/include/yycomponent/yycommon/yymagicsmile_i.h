#pragma once

#include "dwbase/dwcomex.h"
#include "duifw/duiwidget.h"
#include "yycomponent/yycomclsid.h"

enum SMILE_DATA_TYPE
{
	ENUM_SMILE_TYPE_DEFAULT, // 系统默认表情
	ENUM_SMILE_TYPE_USER,	 // 自定义表情.
	ENUM_SMILE_TYPE_MAGIC // 魔法表情?
};

/**
* @brief YY表情描述.
* @param smileKey 快捷键.
* @param smileFile 文件名.
* @param description 描述.
* @param type 表情类型，具体请看SMILE_DATA_TYPE.
* @param downloadUrl 已上传到服务器的对应地址，如不为空，表示已经成功上传到服务器.
* @param prevFile 用于预览的图片文件名.
*/
struct SmileData
{
	SmileData()
		: type(ENUM_SMILE_TYPE_DEFAULT)
	{}

	QString	smileKey;  
	QString smileFile;     
	QString description;    
	SMILE_DATA_TYPE type;   
	QString downloadUrl;          
	QString prevFile;            

	SmileData& operator = (const SmileData& object)
	{
		if (this == &object)
		{
			return *this;
		}
		smileKey	= object.smileKey;
		smileFile	= object.smileFile;
		description = object.description;
		type		= object.type;
		downloadUrl = object.downloadUrl;
		prevFile	= object.prevFile;

		return *this;
	}

	bool operator ==(const SmileData& object) const
	{ 
		return smileFile == object.smileFile; 
	}

	/**
	* @brief 判断表情描述是否有效.
	* @return. 
	*/
	bool isNull() const
	{
		return smileKey.isEmpty();
	}
};


class DuiPushButton;
struct FACE_ATTR;

/**
* @brief 魔法表情
********************************************
注意:此接口已经废弃
********************************************
*/
DWDEFINE_INTERFACE(IYYMagicSmile): public IDWQComponent
{

	/**
	* @brief 初始化操作.
	* @return 返回是否成功.
	*/
	virtual bool init() = 0;

	/**
	* @brief 已废弃，不建议使用.
	* @param .
	* @return.
	*/
	virtual void show(DuiPushButton *emotionButton, bool vip) = 0;
	/**
	* @brief 通过快捷键查找对应表情描述信息.
	* @param key 快捷键.
	* @return 返回SmileData.
	*/
	virtual SmileData smiliesData(const QString &key) = 0;

	/**
	* @brief 通过快捷键得到对应的描述信息.
	* @param strShortcut 快捷键.
	* @return 返回FACE_ATTR.
	*/
	virtual FACE_ATTR getByShortcut(const QString& strShortcut) = 0;

	/**
	* @brief 设置白名单，可以控制显示哪些分组.
	* @param groupIdList 表情分组id列表.
	* @return.
	*/
	virtual void setWhiteList(const std::vector<UINT32>& groupIdList) = 0;

	/**
	* @brief 把魔法表情的widget嵌入到指定的widget中去.
	* @param pParentWidget 被嵌入的父widget.
	* @param strWidgetName 魔法表情widget的名字.
	* @return 返回创建的魔法表情widget.
	*/
	virtual DuiWidget* addToWidget(DuiWidget* pParentWidget, const QString& strWidgetName) = 0;

	/**
	* @brief 通过表情文件所在路径查找到文件二进制数据.
	* @param strFilename 魔法表情文件路径.
	* @return 返回魔法表情文件二进制数据.
	*/
	virtual QByteArray getFileData(const QString& strFilename) = 0;

	/**
	* @brief 魔法表情包是否加载完成.
	* @return. 
	*/
	virtual bool isYYMagicSmileLoadComplete() = 0;

	/**
	* @brief 通过表情快捷键查找所在分组.
	* @param smiliesKey 表情快捷键.
	* @return 返回所在分组id.
	*/
	virtual int getSmiliesGroup(const QString& smiliesKey) = 0;

	virtual DuiWidget* getWidgetByName(const QString &strWidgetName) = 0;
/*
signals:
    void smiliesSelected(const SmileData&);
    void smilesComplete();
*/
};