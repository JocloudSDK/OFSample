#pragma once

/**
  * @brief 音频引擎功能接口，可创建故能对象，通过root->getServiceInterface<IAudioEngineFeature>(KSERVICECommon)来获取.
  * 目前有IAudioEnginePreview, IAudioEngineEncode, IAudioEngineDecode
  */
DWDEFINE_INTERFACE(IAudioEngineFeature) : public IUnk
{
	/**
	  * @brief 创建功能对象，请用下面的模板创建
	  * @param iid UUID
	  * @param ppv 指针
	  * @return bool 是否创建成功
	  */
	virtual bool createFeature(REFDWUUID iid, void **ppv) = 0;

	/**
	  * @brief 创建功能对象，请实例化此函数 createFeature<Interface>();
	  * @return 功能对象智能指针，创建失败返回空
	  */
	template<typename Interface>
	comptr<Interface> createFeature()
	{
		comptr<Interface> spInterface;
		createFeature(__dwuuidof(Interface), (void**)&spInterface);
		return spInterface;
	}
};
