#pragma once

/**
   * @brief 说话设置常量结构.
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
   * @brief 说话设置接口.
   */
DWDEFINE_INTERFACE(IYYSpeakSetting): IUnk, public SpeakSettingConst
{
/**
   * @brief 设置按键说话.
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
   * @brief 设置鼠标按键说话.
   * @param button 鼠标键.
   */
	virtual void setSpeakMouseButton( quint32 button ) = 0;
/**
   * @brief 获取说话的鼠标按键.
   * @return 鼠标键.
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