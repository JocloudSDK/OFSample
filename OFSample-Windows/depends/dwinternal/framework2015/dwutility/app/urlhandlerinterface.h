#pragma once

#include <string>

#include "dwglobal.h"

namespace DwUtility
{
	namespace app
	{
		struct IUrlHandler
		{
			virtual bool process(const std::wstring& url) = 0;
		};

		struct IUrlProcessor
		{
			virtual void registerUrlHandler( IUrlHandler * hanlder ) = 0;

			virtual void unRegisterUrlHandler( IUrlHandler * hanlder ) = 0;
		};

		DW_DWUTILITY_EXPORT IUrlProcessor* getUrlProcessor();
	}
}
