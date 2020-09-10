#include "stable.h"
#include "urlhandler.h"

namespace DwUtility
{
	namespace app
	{
		DwUrlProcessor::DwUrlProcessor()
		{

		}

		DwUrlProcessor::~DwUrlProcessor()
		{

		}

		void DwUrlProcessor::registerUrlHandler( IUrlHandler * hanlder )
		{
			m_handers.insert(hanlder);
		}

		void DwUrlProcessor::unRegisterUrlHandler( IUrlHandler * hanlder )
		{
			m_handers.erase(hanlder);
		}

		std::set<IUrlHandler*> DwUrlProcessor::urlHanders() const
		{
			return m_handers;
		}

		DwUrlProcessor *g_urlProcessor = NULL;
		IUrlProcessor *getUrlProcessor()
		{
			if (g_urlProcessor == NULL)
			{
				g_urlProcessor = new DwUrlProcessor;
			}

			return g_urlProcessor;
		}
	}
}