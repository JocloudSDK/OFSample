#include "stable.h"

#include "duiapplication.h"
#include "duiresourceloader.h"
#include "dwutility/dwxmldocument.h"
#include "dwbase/log.h"
#include "dwbase/perflog.h"
#include "dwutility/environmentpath.h"

//------------------------------------------------------------------------------
namespace
{
	Q_GLOBAL_STATIC(DuiDefaultHandler, _dw_GetDefaultHandler);
	Q_GLOBAL_STATIC(DuiQtHandler, _dw_GetQtHandler);
	Q_GLOBAL_STATIC(DuiAbsoluteHandler, _dw_GetAbsoluteHandler);

	bool isRelativePath(const QString& path)
	{
		if (!QDir::isRelativePath(path) || path.isEmpty())
		{
			return false;
		}

		QRegExp reg("[\\*\\?\\|\\<\\>\\\"\\:]");
		DW_ASSERT(reg.isValid());
		if (path.contains(reg))
		{
			return false;
		}

		return true;
	}
}

class DuiResourceLoaderPrivate
{
	Q_DECLARE_PUBLIC(DuiResourceLoader)
	Q_DISABLE_COPY(DuiResourceLoaderPrivate);
public:
	DuiResourceLoaderPrivate()  : m_warmupTimer(NULL)
	{

	}
	~DuiResourceLoaderPrivate() 
	{ 
		m_handlerList.clear(); 
		m_warmupTasks.clear();
		if (m_warmupTimer)
			DW_SAFE_DELETE(m_warmupTimer);
	}

public:
	void addWarmupXml(const QString& path)
	{
		m_warmupTasks.push_back(WarmupTask(WarmupTask_Xml, path));
	}
	bool doWarmupTask()
	{
		if (m_warmupTasks.empty()) {
			return false;
		}
		
		Q_Q(DuiResourceLoader);
		WarmupTask task = m_warmupTasks.first();
		m_warmupTasks.pop_front();

		if (task.type == WarmupTask_Xml) {
			DWXmlDocument xml;
			xml.setXmlFilter(myXmlFilter, (void*)&m_warmupTasks);
			q->loadXml(task.path, xml);
		}
		else {
			QPixmap& pixmap = q->loadImage(task.path);
			pixmap.size();//强制进行pixmap解码
		}
		return true;
	}

	void stopWarmup()
	{
		if (m_warmupTimer) {
			m_warmupTimer->stop();
			DW_SAFE_DELETE(m_warmupTimer);
		}
		m_warmupTasks.clear();
	}

private:
	static bool __stdcall myXmlFilter(DWXmlDocument::DWXmlElementType type, LPCWSTR name, LPCWSTR value, void* filterContext)
	{
		if (type == DWXmlDocument::DWXmlElement_attr)
		{
			if (_wcsicmp(name, L"icon") == 0
				|| _wcsicmp(name, L"imagePath") == 0)
			{
				QList<WarmupTask>* pWarmupTasks = static_cast<QList<WarmupTask>*>(filterContext);
				pWarmupTasks->push_back(
					WarmupTask(WarmupTask_Image, QStringFromWCharArray(value)));
			}
		}
		return true;
	}

private:
	enum WarmupTaskType 
	{
		WarmupTask_Xml,
		WarmupTask_Image,
	};
	struct WarmupTask {
		WarmupTaskType type;
		QString path;

		WarmupTask(WarmupTaskType _type, const QString& _path)
			: type(_type), path(_path)
		{
		}
	};
	DuiResourceLoader *q_ptr;
	QList<WarmupTask> m_warmupTasks;   
	QList<QPointer<DuiResourceHandler>> m_handlerList;
	QTimer* m_warmupTimer;
};
//------------------------------------------------------------------------------
DuiResourceLoader::DuiResourceLoader() 
	: d_ptr(new DuiResourceLoaderPrivate)
{
	d_ptr->q_ptr = this;

	installResourceHandler(_dw_GetQtHandler());
	installResourceHandler(_dw_GetDefaultHandler());
	installResourceHandler(_dw_GetAbsoluteHandler());
}

DuiResourceLoader::~DuiResourceLoader()
{	
	
}

void DuiResourceLoader::installResourceHandler(DuiResourceHandler* handler)
{
	Q_D(DuiResourceLoader);
	QPointer<DuiResourceHandler> resHandler(handler);

	if (!d->m_handlerList.contains(resHandler))
	{
		d->m_handlerList.push_back(QPointer<DuiResourceHandler>(handler));
	}
}

QPixmap DuiResourceLoader::loadImage(const QString& path)
{
	PerfLogF(F_DUI);
	Q_D(DuiResourceLoader);

	QString trimmedPath = path.trimmed();
	QPixmap pixmap;
	for (int i = 0; i < d->m_handlerList.count(); ++i)
	{
		if (d->m_handlerList[i].isNull())
		{
			continue;
		}

		if (d->m_handlerList[i]->loadImage(trimmedPath, pixmap))
		{
			break;
		}
	}

	return pixmap;
}

QByteArray DuiResourceLoader::loadRawData(const QString& path)
{
	PerfLogF(F_DUI);
	Q_D(DuiResourceLoader);

	QString trimmedPath = path.trimmed();
	QByteArray bytes;
	for (int i = 0; i < d->m_handlerList.count(); ++i)
	{
		if (d->m_handlerList[i].isNull())
		{
			continue;
		}

		if (d->m_handlerList[i]->loadRawData(trimmedPath, bytes))
		{
			break;
		}
	}

	if (bytes.isEmpty())
	{
		LogFinal(LOG::KAddon) << QString("load from file %1 failed.").arg(path);
	}
	return bytes;
}

bool DuiResourceLoader::loadXml(const QString& path, DWXmlDocument& xml)
{
	PerfLogF(F_DUI);
	Q_D(DuiResourceLoader);

	DW_ASSERT(!path.contains(' '));
	DW_ASSERT(!path.isEmpty());

	QString trimmedPath = path.trimmed();
	if(trimmedPath.isEmpty())
	{
		return false;
	}

	if (trimmedPath.startsWith("layout/"))
	{
		trimmedPath.prepend(":/");
#ifndef OFFICIAL_BUILD
		if(!QFile::exists(trimmedPath))
		{
			trimmedPath.remove(0, 2);
		}
#endif
	}

	for (int i = 0; i < d->m_handlerList.count(); ++i)
	{
		if (d->m_handlerList[i].isNull())
		{
			continue;
		}

		if (d->m_handlerList[i]->loadXml(trimmedPath, xml))
		{
			return true;
		}
	}

	return false;
}

void DuiResourceLoader::addWarmupXml(const QString& path)
{
	Q_D(DuiResourceLoader);
	d->addWarmupXml(path);
}

void DuiResourceLoader::doWarmup()
{
	Q_D(DuiResourceLoader);
	if (d->m_warmupTimer == NULL) {
		d->m_warmupTimer = new QTimer();
		connect(d->m_warmupTimer, SIGNAL(timeout()), this, SLOT(on_warmup()));
		d->m_warmupTimer->start(10);
	}
}

void DuiResourceLoader::stopWarmup()
{
	Q_D(DuiResourceLoader);
	d->stopWarmup();
}

void DuiResourceLoader::on_warmup()
{
	Q_D(DuiResourceLoader);
	if (!d->doWarmupTask()) {
		stopWarmup();
	}
}

bool DuiAbsoluteHandler::loadImage(const QString& path, QPixmap& pixmap)
{
	PerfLogF(F_DUI);
	if (QDir::isAbsolutePath(path) && !path.isEmpty())
	{
		return pixmap.load(path);
	}

	return false;
}

bool DuiAbsoluteHandler::loadRawData(const QString& path, QByteArray &bytes)
{
	PerfLogF(F_DUI);
	if (QDir::isAbsolutePath(path) && !path.isEmpty())
	{
		QFile file(path);
		if (file.open(QIODevice::ReadOnly))
		{
			bytes = file.readAll();
			return true;
		}
	}

	return false;
}

bool DuiAbsoluteHandler::loadXml(const QString& path, DWXmlDocument& xml)
{
	PerfLogF(F_DUI);
	if (QDir::isAbsolutePath(path) && !path.isEmpty())
	{
		return xml.loadFromFile(path.toStdWString());
	}

	return false;
}

//------------------------------------------------------------------------------
bool DuiQtHandler::loadImage(const QString& path, QPixmap& pixmap)
{
	PerfLogF(F_DUI);
	static bool shallLoadExternalImage = QFile::exists(QStringFromWString(DwUtility::app::appRootPath()) + "/load_external_image");
	if(path.startsWith(':'))
	{
		if(shallLoadExternalImage)
		{
			QString imageFullPath = QStringFromWString(DwUtility::app::appRootPath());
			imageFullPath += "/";
			imageFullPath += path.right(path.length() - 1);

			if(QFile::exists(imageFullPath))
			{
				return pixmap.load(imageFullPath);
			}
		}
		return pixmap.load(path);
	}

	return false;
}

bool DuiQtHandler::loadRawData(const QString& path, QByteArray &bytes)
{
	PerfLogF(F_DUI);
	if( path.startsWith(":/") )
	{
		QFile file(path);

		if ( file.open(QFile::ReadOnly) )
		{
			bytes = file.readAll();

			return true;
		}
	}
	return false;
}

bool DuiQtHandler::loadXml(const QString& path, DWXmlDocument& xml)
{
	PerfLogF(F_DUI);
	if( path.startsWith(":/") )
	{
		QFile file(path);

		if ( file.open(QFile::ReadOnly) )
		{
			QByteArray bytes = file.readAll();
			return xml.loadFromString(bytes.constData(), bytes.count());
		}
	}

	return false;
}

//------------------------------------------------------------------------------
QString DuiDefaultHandler::reletivePath2AbsolutePath(const QString& path)
{
	if (duiApp())
	{
		return duiApp()->appResourcePath() + path;
	}
	else
	{
		return path;
	}
}

bool DuiDefaultHandler::loadImage(const QString& path, QPixmap& pixmap)
{
	PerfLogF(F_DUI);
	if (isRelativePath(path))
	{
		return pixmap.load(reletivePath2AbsolutePath(path));
	}

	return false;
}


bool DuiDefaultHandler::loadRawData(const QString& path, QByteArray &bytes)
{
	PerfLogF(F_DUI);
	if (isRelativePath(path))
	{
		QFile file(reletivePath2AbsolutePath(path));
		if (file.open(QIODevice::ReadOnly))
		{
			bytes = file.readAll();
			return true;
		}
		//DW_ASSERT_X(false, __FUNCTION__, QString("load from file %1 failed.").arg(path).toLatin1().constData());
	}

	return false;
}

bool DuiDefaultHandler::loadXml(const QString& path, DWXmlDocument& xml)
{
	PerfLogF(F_DUI);
	if (isRelativePath(path))
	{
		return xml.loadFromFile(reletivePath2AbsolutePath(path).toStdWString());
	}

	return false;
}

//------------------------------------------------------------------------------
Q_GLOBAL_STATIC(DuiResourceLoader, _dw_GetResourceLoader);
DuiResourceLoader *getResourceLoader()
{
	return _dw_GetResourceLoader();
}
