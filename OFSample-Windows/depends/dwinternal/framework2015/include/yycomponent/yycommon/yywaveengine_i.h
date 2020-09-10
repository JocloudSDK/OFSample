#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"


#define WAVE_ANSWER_QUESTION_1			1	//����1
#define WAVE_ANSWER_QUESTION_2			2	//����2
#define WAVE_ANSWER_QUESTION_3			3	//����3
#define WAVE_ANSWER_QUESTION_4			4	//����4
#define WAVE_JOIN						5	//����Ƶ��
#define WAVE_LEAVE						6	//�뿪Ƶ��
#define WAVE_NOTICE						7	//������Ա�߳�����/������Ա��ֹ˵��/������Ƶ��ʧ��
#define WAVE_TUOREN						8	//������Ա�϶�������Ƶ��
#define WAVE_EMERGENCY					9	//��̨�����������
#define WAVE_IM_MESSAGE					10	//IM������Ϣ
#define WAVE_IM_SYSTEM					11	//IMϵͳ��Ϣ
#define WAVE_PRIVATE_CHAT				12	//˽����Ϣ
#define WAVE_GROUP_MSG					13  //Ⱥ��Ϣ����
#define WAVE_GROUP_SYSMSG				14  //Ⱥϵͳ��Ϣ����
#define WAVE_IM_VOICE					15  //������������
#define	WAVE_ACTIVITY_WND_FIRST_SHOWN	16	//����ѵ�������
#define WAVE_ACTIVITY_BEGIN				17	//���ʼʱ����

#define WAVE_CMD_JOIN					18	//����ָ��ͨ��Ƶ��
#define WAVE_CMD_LEAVE					19	//�뿪ָ��ͨ��Ƶ��
#define WAVE_CMD_START_SPEAK			20	//ָ��ͨ����ʼ����˵��ʱ

#define WAVE_UNMUTE_MICROPHONE			21	//ʹ���ȼ���Ƶ����˷�ʱ��������

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYWaveEngine): public IUnk
{

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool playWave(const std::wstring &fileName, bool bStopCur /*= false*/) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void playWave(int id, bool bStopCur /*= false*/) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void enableWave(bool bEnable) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool playMp3(const std::wstring &fileName, UINT32 fromPeerUid /*=0*/ ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void stopPlay(UINT32 fromPeerId /*=0*/ ) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void uninit() = 0;
};