#pragma once

#include <set>

#include "dwglobal.h"
#include "urlhandlerinterface.h"

namespace DwUtility
{
	namespace app
	{
		class DwUrlProcessor : public IUrlProcessor
		{
		public:
			DwUrlProcessor();
			~DwUrlProcessor();

			virtual void registerUrlHandler( IUrlHandler * hanlder );
			virtual void unRegisterUrlHandler( IUrlHandler * hanlder );

			std::set<IUrlHandler*> urlHanders() const;

		private:
			std::set<IUrlHandler*> m_handers;
		};
	}
}
