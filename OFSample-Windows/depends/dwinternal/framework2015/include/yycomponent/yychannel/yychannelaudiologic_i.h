
#pragma once
#include "dwbase/dwcomex.h"
#include "yyproperty.h"
#include "yyaudiovolumecompare.h"

DWDEFINE_INTERFACE(IChannelAudioLogic):public IUnk
{
public:
	enum AudioState
	{ 
		Off // READ:�رյ�. WRITE:�ر�.
		,On // READ:�򿪵�. WRITE:����.
	}; 
	enum NewDeviceState
	{ 
		Detected // READONLY: ��⵽���豸.
		,Connect // WRITEONLY: ʹ�����豸.
		,Enabled // READONLY: �Ѿ�����.
		,Disconnected // READONLY: �Ѿ��Ͽ�.
	}; 
	enum SpeakStyle
	{
		Mouse = 1 // READ/WRITE: ������˵��.
		,Keyboard = 2 // READ/WRITE: �ü��̼�˵��.
		,Free = 3 // READ/WRITE: ����˵��.
		,Switch = 4 // WRITEONLY: ������˵���Ͱ���(��������̼�)˵�����л�
	}; 
	typedef double Volume; // Volume: ����������С. �Ϸ�ֵ���� [0.0, 1.0] .

	// ErrorCode: �������.������ඨ������ע��. Accompany_Select_Player �Ѿ�����
	enum ErrorCode{ Accompany_Permission, Accompany_Select_Player, Player_Not_Exist, Record_Create_Path_Failed
		, Record_Finished, Rename_Illegal_Name, Rename_File_Exists, Rename_File_Not_Found
		, Rename_File_Cant_Write, Rename_File_Ivalid_Name, Rename_Unknown_Error, Speak_Permission
		, Speak_Banned,Speak_Visitor_Banned, Key_Press_Invalid,Warning_Voice_Change, Warning_Speaker_Muted
		, Warning_Sys_Speaker_Muted	,Warning_Min_Volume, ErrorCodeMax,GetKeyEventFailed
	};

	// ErrorType: ��������.
	enum ErrorType
	{
		None // ������.
		,Information // ��Ϣ
		,Prompt // ��ʾ
		,Warning // ����
		,Warn_Notice // ����-ע��
		,Warn_Question // ����-ѡ��
	};
	
	// Error: ������ඨ������ע��.
	struct Error
	{
		ErrorCode code; // �������.
		ErrorType type; // ��������.
		QString description; // ��������.
		QVariantList args; // �������.
	};	

	enum AudioSourceType
	{
		MIC = 0,
		OUTER_PCM,
		MIX
	};

	// ����: ������˵����,��Ҫ���κ� property ����κ� rule.

	typedef yyproperty::property<AudioState> SwitchPro; // { On, Off }
	typedef yyproperty::property<Volume,VolumeCmpare> VolumePro; // [0.0,1.0]
	typedef yyproperty::property<std::wstring> StringPro; // "*"
	typedef yyproperty::property<int> StylePro; // 1,2,3 ... ,ͨ������
	typedef yyproperty::property<NewDeviceState> NewDevicePro; // �� pro_newDevice ʹ��
	typedef yyproperty::property<SpeakStyle> SpeakStylePro; // Mouse,Keyboard,Free
	typedef yyproperty::property<double> ScalePro;// for pro_enhenceMicScale only
	typedef yyproperty::property<AudioSourceType> AudioSourcePro;// for pro_enhenceMicScale only

	typedef _def_boost_signals2_signal_type<void (const Error&)>::type _SIG_ERROR_;
	/**
   * @brief Error signal.
   */
	virtual _SIG_ERROR_& sigError() = 0;

	/**
   * @brief ��Ƶ�ܿ���, READ/WRITE. 
   * @return AudioState.
   */
	virtual SwitchPro& pro_masterSwitch() = 0;
	
	/**
   * @brief ���࿪��, READ/WRITE.
   * @return AudioState.
   */
	virtual SwitchPro& pro_accompany() = 0;
	
	/**
   * @brief ¼������, READ/WRITE.
   * @return AudioState
   */
	virtual SwitchPro& pro_record() = 0;
		
	/**
   * @brief ����������, READ/WRITE.
   * @return AudioState.
   */
	virtual SwitchPro& pro_speaker() = 0;

	/**
   * @brief ��˷忪��, READ/WRITE.
   * @return AudioState.
   */
	virtual SwitchPro& pro_micphone() = 0;

	/**
   * @brief ���쿪��, READ/WRITE.
   * @return AudioState.
   */
	virtual SwitchPro& pro_reverb() = 0;

	/**
   * @brief ������������С(����), READ/WRITE.
   * @return Volume.
   */
	virtual VolumePro& pro_speakerVolume() = 0;
	
	/**
   * @brief ��˷�������С(����), READ/WRITE.
   * @return Volume.
   */
	virtual VolumePro& pro_micphoneVolume() = 0;
	
	/**
   * @brief ��������ƽ��С, READONLY.
   * @return Volume.
   */
	virtual const VolumePro& pro_speakerLevel() = 0;
	
	/**
   * @brief ��˷��ƽ��С, READONLY
   * @return Volume.
   */
	virtual const VolumePro& pro_micphoneLevel() = 0;

	/**
   * @brief ˵��״̬, READONLY
   * @return AudioState, AudioState::On : ����˵��,AudioState::Off : û��˵��.
   */
	virtual const SwitchPro& pro_talking() = 0;
	
	/**
   * @brief ˵���ȼ�, READONLY.
   * @return std::wtring ����. ���ü�������ʱ,�����ǰ׺ "Pressing-"
   */
	virtual const StringPro& pro_speakKey() = 0;

	/**
   * @brief ˵����ʽ, READ/WRITE.
   * @return SpeakStyle.
   */
	virtual SpeakStylePro& pro_speakStyle() = 0;

	/**
   * @brief ¼���ļ���/������¼���ļ�. READ/WRITE
   * @return std::wtring �ļ���.
   */
	virtual StringPro& pro_recordName() = 0;

	/**
   * @brief ���豸״̬.  READ/WRITE
   * @return NewDeviceState.
   */
	virtual NewDevicePro& pro_newDevice() = 0;	
	
	/**
   * @brief ϵͳ����������. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_systemSpeaker() = 0;
	
	/**
   * @brief �������. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_softAccompany() = 0;
	
	/**
   * @brief ����ǿ��. READ/WRITE
   * @return Volume.
   */
	virtual VolumePro& pro_reverbVolume() = 0;
	
	/**
   * @brief ��������. READ/WRITE
   * @return Volume.
   */
	virtual VolumePro& pro_reverbInputVolume() = 0;
	
	/**
   * @brief ����ģʽ. READ/WRITE
   * @return Style.
   */
	virtual StylePro& pro_reverbPreset() = 0;

	/**
   * @brief ��˷���ǿ. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_enhanceMic() = 0;

	/**
   * @brief ��˷���ǿ����. READ/WRITE
   * @return Style.
   */
	virtual StylePro& pro_enhenceMicType() = 0;
	
	/**
   * @brief ��˷���ǿ����. READ/WRITE
   * @return Scale.
   */
	virtual ScalePro& pro_enhenceMicScale() = 0;

	/**
   * @brief ��˷彵��. READ/WRITE
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
   * @brief ����. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_voiceChange() = 0;
	
	/**
   * @brief ����Ԥ��. READ/WRITE
   * @return Style.
   */
	virtual StylePro& pro_voiceChangePreset() = 0;
		
	/**
   * @brief �������ҵ�. READ/WRITE
   * @return AudioState.
   */
	virtual SwitchPro& pro_voiceChangeExquisite() = 0;

	/**
   * @brief ��������. READ/WRITE
   * @return Volume.
   */
	virtual VolumePro& pro_voiceChangeSemitone() = 0;

	/**
   * @brief . READ/WRITE
   * @return AudioSourceType.
   */
	virtual AudioSourcePro& pro_audioSource() = 0;

	/**
   * @brief Rovee�ı��������͹ر�,ע�⣺�˳�Ƶ��ʱҪ�رա�. READ/WRITE
   * @return AudioState.
   * @demo 
   *IChannelAudioLogicPtr audio = comMgr()->getServiceInterface<IChannelAudioLogic>(KSERVICEChannel);
	if (audio)
	{
		audio->pro_roveeVoiceChange() = IChannelAudioLogic::On;
		audio->pro_roveeVoiceChangePitch() = 0.541666;
		audio->pro_roveeVoiceChangeFormat() = 0.734375;
	}
	ֵ��Ӧ�õĽ�ɫ��
	1��������
	   Ů����pro_roveeVoiceChangePitch() = 0.541666;pro_roveeVoiceChangeFormat() = 0.734375;
	   ������pro_roveeVoiceChangePitch() = 0.83333;pro_roveeVoiceChangeFormat() = 0.822916;
	2��Ů��
		Ů����0.5;0.5;
		������1;0.739583;
	3��С����
		Ů����0.75;0.734375;
		������1;0.739583;
	4��С����
		Ů����0.291666;0.40625;
		������0.541666;0.604166;
	5���ܺ���
		Ů����0.75;0.661458;
		������1;0.979166;
	6���ŵ���Ů
		Ů����0.666666;0.666666;
		������1;0.739583;
	7������
		Ů����0.166666;0.33333;
		������0.291666;0.40625;
	8������
		Ů����0.583333;0.744791;
		������0.875;0.760416;
   */
	virtual SwitchPro& pro_roveeVoiceChange() = 0;

	/**
   * @brief Rovee����Ԥ��. READ/WRITE
   * @return Style.
   */
	virtual StylePro& pro_roveeVoiceChangePreset() = 0;

	/**
   * @brief Rovee����pitch�ֶ�. READ/WRITE
   * @return AudioState.
   */
	virtual VolumePro& pro_roveeVoiceChangePitch() = 0;

	/**
   * @brief Rovee����format�ֶ�. READ/WRITE
   * @return Volume.
   */
	virtual VolumePro& pro_roveeVoiceChangeFormat() = 0;

	/**
   * @brief ǿ��Ӧ�õ�ǰrovee��������. 
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