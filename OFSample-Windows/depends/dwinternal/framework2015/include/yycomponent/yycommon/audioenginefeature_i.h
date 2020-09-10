#pragma once

/**
  * @brief ��Ƶ���湦�ܽӿڣ��ɴ������ܶ���ͨ��root->getServiceInterface<IAudioEngineFeature>(KSERVICECommon)����ȡ.
  * Ŀǰ��IAudioEnginePreview, IAudioEngineEncode, IAudioEngineDecode
  */
DWDEFINE_INTERFACE(IAudioEngineFeature) : public IUnk
{
	/**
	  * @brief �������ܶ������������ģ�崴��
	  * @param iid UUID
	  * @param ppv ָ��
	  * @return bool �Ƿ񴴽��ɹ�
	  */
	virtual bool createFeature(REFDWUUID iid, void **ppv) = 0;

	/**
	  * @brief �������ܶ�����ʵ�����˺��� createFeature<Interface>();
	  * @return ���ܶ�������ָ�룬����ʧ�ܷ��ؿ�
	  */
	template<typename Interface>
	comptr<Interface> createFeature()
	{
		comptr<Interface> spInterface;
		createFeature(__dwuuidof(Interface), (void**)&spInterface);
		return spInterface;
	}
};
