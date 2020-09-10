
#pragma once
#include "dwbase/dwcomex.h"
#include "yyproperty.h"
#include "yyaudiovolumecompare.h"

DWDEFINE_INTERFACE(IChannelAudioLogic):public IUnk
{
public:
	enum AudioState
	{ 
		Off // READ:关闭的. WRITE:关闭.
		,On // READ:打开的. WRITE:开启.
	}; 
	enum NewDeviceState
	{ 
		Detected // READONLY: 检测到新设备.
		,Connect // WRITEONLY: 使用新设备.
		,Enabled // READONLY: 已经启用.
		,Disconnected // READONLY: 已经断开.
	}; 
	enum SpeakStyle
	{
		Mouse = 1 // READ/WRITE: 用鼠标键说话.
		,Keyboard = 2 // READ/WRITE: 用键盘键说话.
		,Free = 3 // READ/WRITE: 自由说话.
		,Switch = 4 // WRITEONLY: 在自由说话和按键(鼠标键或键盘键)说话间切换
	}; 
	typedef double Volume; // Volume: 描述声音大小. 合法值区间 [0.0, 1.0] .

	// ErrorCode: 错误代码.详见本类定义后面的注释. Accompany_Select_Player 已经废弃
	enum ErrorCode{ Accompany_Permission, Accompany_Select_Player, Player_Not_Exist, Record_Create_Path_Failed
		, Record_Finished, Rename_Illegal_Name, Rename_File_Exists, Rename_File_Not_Found
		, Rename_File_Cant_Write, Rename_File_Ivalid_Name, Rename_Unknown_Error, Speak_Permission
		, Speak_Banned,Speak_Visitor_Banned, Key_Press_Invalid,Warning_Voice_Change, Warning_Speaker_Muted
		, Warning_Sys_Speaker_Muted	,Warning_Min_Volume, ErrorCodeMax,GetKeyEventFailed
	};

	// ErrorType: 错误类型.
	enum ErrorType
	{
		None // 无类型.
		,Information // 消息
		,Prompt // 提示
		,Warning // 警告
		,Warn_Notice // 警告-注意
		,Warn_Question // 警告-选择
	};
	
	// Error: 详见本类定义后面的注释.
	struct Error
	{
		ErrorCode code; // 错误代码.
		ErrorType type; // 错误类型.
		QString description; // 错误描述.
		QVariantList args; // 额外参数.
	};	

	enum AudioSourceType
	{
		MIC = 0,
		OUTER_PCM,
		MIX
	};

	// 警告: 除特殊说明外,不要给任何 property 添加任何 rule.

	typedef yyproperty::property<AudioState> SwitchPro; // { On, Off }
	typedef yyproperty::property<Volume,VolumeCmpare> VolumePro; // [0.0,1.0]
	typedef yyproperty::property<std::wstring> StringPro; // "*"
	typedef yyproperty::property<int> StylePro; // 1,2,3 ... ,通用类型
	typedef yyproperty::property<NewDeviceState> NewDevicePro; // 仅 pro_newDevice 使用
	typedef yyproperty::property<SpeakStyle> SpeakStylePro; // Mouse,Keyboard,Free
	typedef yyproperty::property<double> ScalePro;// for pro_enhenceMicScale only
	typedef yyproperty::property<AudioSourceType> AudioSourcePro;// for pro_enhenceMicScale only

	typedef _def_boost_signals2_signal_type<void (const Error&)>::type _SIG_ERROR_;
	/**
   * @brief Error signal.
   */
	virtual _SIG_ERROR_& sigError() = 0;

	/**
   * @brief 音频总开关, READ/WRITE. 
   * @return AudioState.
   */
	virtual SwitchPro& pro_masterSwitch() = 0;
	
	/**
   * @brief 伴奏开关, READ/WRITE.
   * @return AudioState.
   */
	virtual SwitchPro& pro_accompany() = 0;
	
	/**
   * @brief 录音开关, READ/WRITE.
   * @return AudioState
   */
	virtual SwitchPro& pro_record() = 0;
		
	/**
   * @brief 扬声器开关, READ/WRITE.
   * @return AudioState.
   */
	virtual SwitchPro& pro_speaker() = 0;

	/**
   * @brief 麦克峰开关, READ/WRITE.
   * @return AudioState.
   */
	virtual SwitchPro& pro_micphone() = 0;

	/**
   * @brief 混响开关, READ/WRITE.
   * @return AudioState.
   */
	virtual SwitchPro& pro_reverb() = 0;

	/**
   * @brief 扬声器音量大小(设置), READ/WRITE.
   * @return Volume.
   */
	virtual VolumePro& pro_speakerVolume() = 0;
	
	/**
   * @brief 麦克峰音量大小(设置), READ/WRITE.
   * @return Volume.
   */
	virtual VolumePro& pro_micphoneVolume() = 0;
	
	/**
   * @brief 扬声器电平大小, READONLY.
   * @return Volume.
   */
	virtual const VolumePro& pro_speakerLevel() = 0;
	
	/**
   * @brief 麦克峰电平大小, READONLY
   * @return Volume.
   */
	virtual const VolumePro& pro_micphoneLevel() = 0;

	/**
   * @brief 说话状态, READONLY
   * @return AudioState, AudioState::On : 正在说话,AudioState::Off : 没在说话.
   */
	virtual const SwitchPro& pro_talking() = 0;
	
	/**
   * @brief 说话热键, READONLY.
   * @return std::wtring 键名. 当该键被按下时,会加上前缀 "Pressing-"
   */
	virtual const StringPro& pro_speakKey() = 0;

	/**
   * @brief 说话方式, READ/WRITE.
   * @return SpeakStyle.
   */
	virtual SpeakStylePro& pro_speakStyle() = 0;

	/**
   * @brief 录音文件名/重命名录音文件. READ/WRITE
   * @return std::wtring 文件名.
   */
	virtual StringPro& pro_recordName() = 0;

	/**
   * @brief 新设备状态.  READ/WRITE
   * @return NewDeviceState.
   */
	virtual NewDevicePro& pro_newDevice() = 0;	
	
	/**
   * @brief 系统扬声器开关. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_systemSpeaker() = 0;
	
	/**
   * @brief 软件混音. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_softAccompany() = 0;
	
	/**
   * @brief 混响强度. READ/WRITE
   * @return Volume.
   */
	virtual VolumePro& pro_reverbVolume() = 0;
	
	/**
   * @brief 混响输入. READ/WRITE
   * @return Volume.
   */
	virtual VolumePro& pro_reverbInputVolume() = 0;
	
	/**
   * @brief 混响模式. READ/WRITE
   * @return Style.
   */
	virtual StylePro& pro_reverbPreset() = 0;

	/**
   * @brief 麦克峰增强. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_enhanceMic() = 0;

	/**
   * @brief 麦克峰增强类型. READ/WRITE
   * @return Style.
   */
	virtual StylePro& pro_enhenceMicType() = 0;
	
	/**
   * @brief 麦克峰增强倍数. READ/WRITE
   * @return Scale.
   */
	virtual ScalePro& pro_enhenceMicScale() = 0;

	/**
   * @brief 麦克峰降噪. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_denoise() = 0;
	
	/**
   * @brief agc. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_agc() = 0;
		
	/**
   * @brief aec. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_aec() = 0;
	
	/**
   * @brief 变声. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_voiceChange() = 0;
	
	/**
   * @brief 变声预设. READ/WRITE
   * @return Style.
   */
	virtual StylePro& pro_voiceChangePreset() = 0;
		
	/**
   * @brief 变声剧烈的. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_voiceChangeExquisite() = 0;

	/**
   * @brief 变声音程. READ/WRITE
   * @return Volume.
   */
	virtual VolumePro& pro_voiceChangeSemitone() = 0;

	/**
   * @brief . READ/WRITE
   * @return AudioSourceType.
   */
	virtual AudioSourcePro& pro_audioSource() = 0;

	/**
   * @brief Rovee的变声开启和关闭,注意：退出频道时要关闭。. READ/WRITE
   * @return AudioState.
   * @demo 
   *IChannelAudioLogicPtr audio = comMgr()->getServiceInterface<IChannelAudioLogic>(KSERVICEChannel);
	if (audio)
	{
		audio->pro_roveeVoiceChange() = IChannelAudioLogic::On;
		audio->pro_roveeVoiceChangePitch() = 0.541666;
		audio->pro_roveeVoiceChangeFormat() = 0.734375;
	}
	值对应用的角色：
	1）外星人
	   女声：pro_roveeVoiceChangePitch() = 0.541666;pro_roveeVoiceChangeFormat() = 0.734375;
	   男声：pro_roveeVoiceChangePitch() = 0.83333;pro_roveeVoiceChangeFormat() = 0.822916;
	2）女巫
		女声：0.5;0.5;
		男声：1;0.739583;
	3）小萝莉
		女声：0.75;0.734375;
		男声：1;0.739583;
	4）小鲜肉
		女声：0.291666;0.40625;
		男声：0.541666;0.604166;
	5）熊孩子
		女声：0.75;0.661458;
		男声：1;0.979166;
	6）古典美女
		女声：0.666666;0.666666;
		男声：1;0.739583;
	7）土豪
		女声：0.166666;0.33333;
		男声：0.291666;0.40625;
	8）逗比
		女声：0.583333;0.744791;
		男声：0.875;0.760416;
   */
	virtual SwitchPro& pro_roveeVoiceChange() = 0;

	/**
   * @brief Rovee变声预设. READ/WRITE
   * @return Style.
   */
	virtual StylePro& pro_roveeVoiceChangePreset() = 0;

	/**
   * @brief Rovee变声pitch字段. READ/WRITE
   * @return AudioState.
   */
	virtual VolumePro& pro_roveeVoiceChangePitch() = 0;

	/**
   * @brief Rovee变声format字段. READ/WRITE
   * @return Volume.
   */
	virtual VolumePro& pro_roveeVoiceChangeFormat() = 0;

	/**
   * @brief 强制应用当前rovee变声设置. 
   */
	virtual void applyRoveeVoiceChangeSetting() = 0;

	virtual SwitchPro& pro_vad() = 0;
};

/* Errors and their information

code:Accompany_Permission
type:Information
description:"Sorry, you are temporarily unable to karaoke because it's not your turn or you have no permission!"
args:null

code:Accompany_Select_Player
type:Information
description:null
args:null

code:Player_Not_Exist
type:Warn_Notice
description:null
args:"The player does not exist you settled, please reset a player","Set player"

code:Record_Create_Path_Failed
type:Warn_Notice
description:"Recording file folder creation failed."
args:null

code:Record_Finished
type:None
description:null
args:recordPath|string,recordTime|int

code:Rename_Illegal_Name
type:Warn_Notice
description:"Rename failed! File Name can not be \\,/,*,?,<,>"
args:null

code:Rename_File_Exists
type:Warn_Question
description:"File already exists! Rename it from %1 to %2?"
args: conflictName|string,fixedName|string

code:Rename_File_Not_Found
type:Warn_Notice
description:"Error, file not found."
args:null

code:Rename_File_Cant_Write
type:Warn_Notice
description:"Cannot write! Did you open this file?"
args:null

code:Rename_File_Ivalid_Name
type:Warn_Notice
description:"Invalid file name"
args:null

code:Rename_Unknown_Error
type:Warn_Notice
description:"Unknown error"
args:null

code:Speak_Permission
type:Information
description:"Sorry, you are not allow to speak for no in Mic Queue or no permission."
args:null

code:Speak_Banned
type:Information
description:"You are banned talking by admin."
args:null

code:Speak_Visitor_Banned
type:Information
description:"Visitors are not allowed to speak under chat mode."
args:null

code:Key_Press_Invalid
type:Information
description:null
args:"You have pressed this key many times, please use the Voice Wizard to solve problems if the voice is unusual:","Voice Wizard"

code:Warning_Voice_Change
type:Information
description:"You have enabled voice changer!"
args:null

code:Warning_Speaker_Muted
type:Information
description:"You have closed the channel earphone, click to open the channel voice"
args:null

code:Warning_Sys_Speaker_Muted
type:Information
description:
args:"Your system speaker is mute, please click here to turn it on:","Unmute"

code:Warning_Min_Volume
type:Information
description:"The volume is minimum, please move the slider to control!"
args:null

*/