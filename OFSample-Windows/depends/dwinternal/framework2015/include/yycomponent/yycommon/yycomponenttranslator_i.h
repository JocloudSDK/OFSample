#pragma once
#include <qobject>

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#ifdef DW_COMPONENT_ENV
#define HAS_DEFINE_DW_COMPONENT_ENV
#undef DW_COMPONENT_ENV
#endif
#include "dwbase/dwcommgr_i.h"
#ifdef HAS_DEFINE_DW_COMPONENT_ENV
#define DW_COMPONENT_ENV
#endif

class QString;

/**
   * @brief 组件翻译接口，通过root->getServiceInterface<IYYComponentTranslator>(KSERVICECommon)来获取.
   */
DWDEFINE_INTERFACE(IYYComponentTranslator): public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool addComponentTranslator(const QString &path) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool removeComponentTranslator(const QString &path) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString comtr(const char *context, const char *sc) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString comtr2(const char *context, const char *s, const char *c) = 0;
};

#define DW_COM_TR\
	static inline QString comtr(const char *s, const char *c = 0) \
	{	\
		QString translation;	\
		do	\
		{	\
			IDWComRootPtr root = comMgr()->getComRoot(KComponentNameUsedInternal__);	\
			if (!root)	\
			{	\
				DW_ASSERT(false);	\
				break;	\
			}	\
				\
			IYYComponentTranslatorPtr translator = root->getServiceInterface<IYYComponentTranslator>(KSERVICECommonComponentLevel);	\
			if (!translator)	\
			{	\
				DW_ASSERT(false);	\
				break;	\
			}	\
				\
			translation = translator->comtr2(staticMetaObject.className(), s, c);	\
		} while (false);	\
			\
		return translation;	\
	}	\
	static inline QString TR(const char *s, const char *c = 0) {return comtr(s,c);}