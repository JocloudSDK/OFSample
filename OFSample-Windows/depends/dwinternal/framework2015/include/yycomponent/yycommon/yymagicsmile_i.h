#pragma once

#include "dwbase/dwcomex.h"
#include "duifw/duiwidget.h"
#include "yycomponent/yycomclsid.h"

enum SMILE_DATA_TYPE
{
	ENUM_SMILE_TYPE_DEFAULT, // ϵͳĬ�ϱ���
	ENUM_SMILE_TYPE_USER,	 // �Զ������.
	ENUM_SMILE_TYPE_MAGIC // ħ������?
};

/**
* @brief YY��������.
* @param smileKey ��ݼ�.
* @param smileFile �ļ���.
* @param description ����.
* @param type �������ͣ������뿴SMILE_DATA_TYPE.
* @param downloadUrl ���ϴ����������Ķ�Ӧ��ַ���粻Ϊ�գ���ʾ�Ѿ��ɹ��ϴ���������.
* @param prevFile ����Ԥ����ͼƬ�ļ���.
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
	* @brief �жϱ��������Ƿ���Ч.
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
* @brief ħ������
********************************************
ע��:�˽ӿ��Ѿ�����
********************************************
*/
DWDEFINE_INTERFACE(IYYMagicSmile): public IDWQComponent
{

	/**
	* @brief ��ʼ������.
	* @return �����Ƿ�ɹ�.
	*/
	virtual bool init() = 0;

	/**
	* @brief �ѷ�����������ʹ��.
	* @param .
	* @return.
	*/
	virtual void show(DuiPushButton *emotionButton, bool vip) = 0;
	/**
	* @brief ͨ����ݼ����Ҷ�Ӧ����������Ϣ.
	* @param key ��ݼ�.
	* @return ����SmileData.
	*/
	virtual SmileData smiliesData(const QString &key) = 0;

	/**
	* @brief ͨ����ݼ��õ���Ӧ��������Ϣ.
	* @param strShortcut ��ݼ�.
	* @return ����FACE_ATTR.
	*/
	virtual FACE_ATTR getByShortcut(const QString& strShortcut) = 0;

	/**
	* @brief ���ð����������Կ�����ʾ��Щ����.
	* @param groupIdList �������id�б�.
	* @return.
	*/
	virtual void setWhiteList(const std::vector<UINT32>& groupIdList) = 0;

	/**
	* @brief ��ħ�������widgetǶ�뵽ָ����widget��ȥ.
	* @param pParentWidget ��Ƕ��ĸ�widget.
	* @param strWidgetName ħ������widget������.
	* @return ���ش�����ħ������widget.
	*/
	virtual DuiWidget* addToWidget(DuiWidget* pParentWidget, const QString& strWidgetName) = 0;

	/**
	* @brief ͨ�������ļ�����·�����ҵ��ļ�����������.
	* @param strFilename ħ�������ļ�·��.
	* @return ����ħ�������ļ�����������.
	*/
	virtual QByteArray getFileData(const QString& strFilename) = 0;

	/**
	* @brief ħ��������Ƿ�������.
	* @return. 
	*/
	virtual bool isYYMagicSmileLoadComplete() = 0;

	/**
	* @brief ͨ�������ݼ��������ڷ���.
	* @param smiliesKey �����ݼ�.
	* @return �������ڷ���id.
	*/
	virtual int getSmiliesGroup(const QString& smiliesKey) = 0;

	virtual DuiWidget* getWidgetByName(const QString &strWidgetName) = 0;
/*
signals:
    void smiliesSelected(const SmileData&);
    void smilesComplete();
*/
};