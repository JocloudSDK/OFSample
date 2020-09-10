#pragma once

/**
   * @brief ˵�����ó����ṹ.
   */
struct SpeakSettingConst
{
	enum SpeakType
	{
		Mouse		= 1,
		Keyboard	= 2,
		Free		= 3
	};
	enum SettingType
	{
		Channel = 0,
		CommandTalk = 1
	};
};

/**
   * @brief ˵�����ýӿ�.
   */
DWDEFINE_INTERFACE(IYYSpeakSetting): IUnk, public SpeakSettingConst
{
/**
   * @brief ���ð���˵��.
   * @param virtualKey virtual key.
   * @param scanCode scan code.
   */
	virtual void	setSpeakKey(quint32 virtualKey, quint32 scanCode) = 0;
/**
   * @brief get speak key.
   * @param [output] virtualKey virtual key.
   * @param [output] scanCode scan code.
   */
	virtual void	speakKey(quint32 &virtualKey, quint32 &scanCode) const = 0;

/**
   * @brief ������갴��˵��.
   * @param button ����.
   */
	virtual void setSpeakMouseButton( quint32 button ) = 0;
/**
   * @brief ��ȡ˵������갴��.
   * @return ����.
   */
	virtual quint32 speakMouseButton() const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setSettingType(SettingType type) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual SettingType settingType() const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setSpeakType( SpeakType type ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual SpeakType speakType() const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual SpeakType speakKeyType() const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void load() = 0;
};