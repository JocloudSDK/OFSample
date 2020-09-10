#include "stable.h"
#include "duifiledialog.h"

#ifdef Q_WS_WIN
#include <qt_windows.h>
#include <QtCore/private/qsystemlibrary_p.h>

#pragma warning(push)
#pragma warning(disable : 4091)
#include <shlobj.h>
#pragma warning(pop)

#endif // Q_WS_WIN

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

#define HRESULT_CHECK_3(hr, action, logLevel) \
	if (FAILED((hr))) \
	{ \
	logLevel() << "HRESULT Failed, lastError: " << ::GetLastError(); \
	action; \
	}

#define HRESULT_CHECK_2(hr, action) HRESULT_CHECK_3(hr, action,	qWarning)
#define HRESULT_CHECK(hr) HRESULT_CHECK_2(hr, return)

//#define DUI_NO_HOOK_QFILEDIALOG_STATIC_METHOD

#ifndef DUI_NO_HOOK_QFILEDIALOG_STATIC_METHOD
typedef QString (*_qt_filedialog_existing_directory_hook)(QWidget *parent, const QString &caption, const QString &dir, QFileDialog::Options options);
Q_GUI_EXPORT _qt_filedialog_existing_directory_hook qt_filedialog_existing_directory_hook;

typedef QString (*_qt_filedialog_open_filename_hook)(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options);
Q_GUI_EXPORT _qt_filedialog_open_filename_hook qt_filedialog_open_filename_hook;

typedef QStringList (*_qt_filedialog_open_filenames_hook)(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options);
Q_GUI_EXPORT _qt_filedialog_open_filenames_hook qt_filedialog_open_filenames_hook;

typedef QString (*_qt_filedialog_save_filename_hook)(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options);
Q_GUI_EXPORT _qt_filedialog_save_filename_hook qt_filedialog_save_filename_hook;
#endif // DUI_NO_HOOK_QFILEDIALOG_STATIC_METHOD

class DuiFileDialogPrivate : public QObject
{
	Q_DECLARE_PUBLIC(DuiFileDialog)

public:
	typedef struct
	{
		enum
		{
			Exec = QEvent::User + 1,
		};
	} Event;

	enum DialogCode
	{
		Rejected,
		Accepted,
	};


public:
	explicit DuiFileDialogPrivate(const QString &caption = QString(),
		const QString &dir = QString(),
		const QString &filter = QString(),
		QString *selectedFilter = 0,
		DuiFileDialog::Options options = 0);
	virtual ~DuiFileDialogPrivate();

public:
	void exec();
	void endDialog_sys();

	QString &directory();

	WId parentWId() const;

public:
	static void showFileDialog(DuiFileDialog *dialog, const QObject *receiver = NULL, const char *member = NULL, bool isAsync = false);

protected:
#ifdef Q_WS_WIN
	DialogCode exec_2k_xp();
	DialogCode exec_vistaLater();

	DialogCode xp_fileDialog();
	DialogCode xp_folderDialog();
	void make_OFN();
	void clean_OFN();
	bool make_IFileDialog(IFileDialog *dialog);
	void appendShellItem(IShellItem *shellItem);
#endif // Q_WS_WIN
	void initDirectory();
	QString resolveSuffixFromFileName(const QStringList &filterList);
	void emitResult(const DialogCode result);

	int selectedFilterIndex(const QStringList &filterList) const;
	void setSelectedFilter(const qint32 index);

	virtual bool event(QEvent *e);

protected:
	DuiFileDialog *q_ptr;
	QString m_caption;
	QString m_directory;
	QString m_filename;
	QString m_filter;
	QString *m_outerSelectedFilter;
	QString m_selectedFilter;
	QStringList m_selectedFiles;
	DuiFileDialog::Options m_options;
	bool m_inLoop;
	bool m_isDefaultDirectory;
#ifdef Q_WS_WIN
	QString m_defaultSuffix;
	QString m_winFilters;
	OPENFILENAME *m_ofn;
#endif // Q_WS_WIN
};

BEGIN_ANONYMOUS_NAMESPACE
const int KMaxNameLen = 1023;
const int KMaxMultiNameLen = 65535;
const char KFileDialogFilterRegexp[] = "^(.*)\\(([a-zA-Z0-9_.*? +;#\\-\\[\\]@\\{\\}/!<>\\$%&=^~:\\|]*)\\)$";

static QStringList dui_make_filters_list(const QString &filters)
{
	QString f(filters);

	if (f.isEmpty())
	{
#ifdef Q_WS_WIN
		f = QFileDialog::tr("All Files (*)");
#else 
		return QStringList();
#endif // Q_WS_WIN
	}

	static const char* gs_sep[] = {";;", "\n", NULL};
	QString sep;
	for (const char *p = *gs_sep; !(sep = QString::fromLatin1(p)).isEmpty(); p += sep.length() + 1)
	{
		if (f.indexOf(sep))
		{
			return f.split(sep);
		}
	}

	return QStringList(f);
}

#ifdef Q_WS_WIN
template <typename T>
static bool ensure_SystemFunction(const QString &lib, const char *funcName, T **ptrFunc)
{
	if (*ptrFunc)
		return true;

	QSystemLibrary systemLib(lib);
	*ptrFunc = (T*)systemLib.resolve(funcName);
	return *ptrFunc;
}

template <typename T>
static bool ensure_Shell32Function(const char *funcName, T **ptrFunc)
{
	return ensure_SystemFunction(QLatin1String("Shell32"), funcName, ptrFunc);
}

#define ENSURE_FUNC_ARGS(func_name) #func_name, &func##func_name

typedef HRESULT (WINAPI *_func_SHCreateItemFromParsingName)(PCWSTR pszPath, IBindCtx *pbc, REFIID riid, void **ppv);
static _func_SHCreateItemFromParsingName funcSHCreateItemFromParsingName = NULL;
typedef BOOL (WINAPI *_func_SHGetPathFromIDListW)(PCIDLIST_ABSOLUTE pidl, LPWSTR pszPath);
static _func_SHGetPathFromIDListW funcSHGetPathFromIDListW = NULL;
typedef PIDLIST_ABSOLUTE (WINAPI *_func_SHBrowseForFolderW)(LPBROWSEINFOW lpbi);
static _func_SHBrowseForFolderW funcSHBrowseForFolderW = NULL;
typedef HRESULT (WINAPI *_func_SHGetMalloc)(IMalloc **ppMalloc);
static _func_SHGetMalloc funcSHGetMalloc = NULL;

static int CALLBACK dui_win_browse_callback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		if (lpData)
		{
			DuiFileDialogPrivate *that = (DuiFileDialogPrivate*)lpData;
			if (!that->directory().isEmpty())
			{
				::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, LPARAM(that->directory().utf16()));
			}
		}
		break;
	case BFFM_SELCHANGED:
		{
			wchar_t path[MAX_PATH];
			funcSHGetPathFromIDListW(LPITEMIDLIST(lParam), path);
			int enabled = path[0] ? 1 : 0;
			::SendMessage(hwnd, BFFM_ENABLEOK, enabled, enabled);
			::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 1, LPARAM(path));
		}
		break;
	}

	return 0;
}

static QStringList dui_extract_filter(const QRegExp &r)
{
	return r.cap(2).split(QLatin1Char(' '));
}

static QString dui_extract_filter(const QString &rawFilter)
{
	QString result;
	QRegExp r(QString::fromLatin1(KFileDialogFilterRegexp));
	if (r.indexIn(rawFilter) >= 0)
	{
		result = r.cap(2);
	}

	return result;
}

static QStringList dui_win_make_filters_list(const QStringList &filterList)
{
	QStringList result;
	QRegExp r(QString::fromLatin1(KFileDialogFilterRegexp));
	for (QStringList::ConstIterator it = filterList.constBegin(); it != filterList.constEnd(); ++it)
	{
		if (r.indexIn(*it) < 0) // An empty string won't match this regexp
			continue;

		result.append(r.cap(1));
		result.append(dui_extract_filter(r).join(";"));
	}
	return result;
}

static void dui_win_close_dialog(WId parent, DWORD threadId)
{
	HWND that = 0;
	while (that = ::FindWindowEx(0, that, WC_DIALOG, NULL))
	{
		DWORD thatThreadId = ::GetWindowThreadProcessId(that, NULL);
		HWND thatParent = ::GetParent(that);
		if (threadId == thatThreadId && (!parent || parent == thatParent))
		{
			dui_win_close_dialog(that, threadId);
			::EndDialog(that, 0);
			break;
		}
	}
}

template <class T>
class DuiComPtr
{
public:
	typedef T _interfaceTy;
	T *ptr;
	DuiComPtr()
		: ptr(NULL)
	{
	}
	~DuiComPtr()
	{
		if (ptr)
		{
			ptr->Release();
		}
	}
	T * operator ->() const
	{
		return ptr;
	}
	T & operator *() const
	{
		return *ptr;
	}
	T ** operator &()
	{
		return &ptr;
	}
	T *const * operator &() const
	{
		return &ptr;
	}
	DuiComPtr<T> & operator =(T *r)
	{
		if (ptr)
		{
			ptr->Release();
		}

		ptr = r;

		if (ptr)
		{
			ptr->AddRef();
		}
		return *this;
	}

	operator T*() const
	{
		return ptr;
	}
	operator bool() const
	{
		return ptr;
	}

private:
	DuiComPtr(DuiComPtr &) {};
};
#endif // Q_WS_WIN

#ifndef DUI_NO_HOOK_QFILEDIALOG_STATIC_METHOD
static DuiFileDialog::Options translateQFileDialogOptions(QFileDialog::Options options)
{
	DuiFileDialog::Options ret = 0;
	if (options & QFileDialog::DontResolveSymlinks)
	{
		ret |= DuiFileDialog::DontResolveSymlinks;
	}
	if (options & QFileDialog::DontConfirmOverwrite)
	{
		ret |= DuiFileDialog::DontConfirmOverwrite;
	}
	if (options & QFileDialog::ReadOnly)
	{
		ret |= DuiFileDialog::ReadOnly;
	}
	if (options & QFileDialog::HideNameFilterDetails)
	{
		ret |= DuiFileDialog::HideNameFilterDetails;
	}
	return ret;
}

class QFileDialogHook
{
private:
	QFileDialogHook()
	{
		qt_filedialog_existing_directory_hook = filedialog_existing_directory;
		qt_filedialog_open_filename_hook = filedialog_open_filename;
		qt_filedialog_open_filenames_hook = filedialog_open_filenames;
		qt_filedialog_save_filename_hook = filedialog_save_filename;
	}

	~QFileDialogHook()
	{
		qt_filedialog_existing_directory_hook = NULL;
		qt_filedialog_open_filename_hook = NULL;
		qt_filedialog_open_filenames_hook = NULL;
		qt_filedialog_save_filename_hook = NULL;
	}

	static QString filedialog_existing_directory(QWidget *parent, const QString &caption, const QString &dir, QFileDialog::Options options)
	{
		return DuiFileDialog::getExistingDirectory(parent, caption, dir, translateQFileDialogOptions(options));
	}

	static QString filedialog_open_filename(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options)
	{
		return DuiFileDialog::getOpenFileName(parent, caption, dir, filter, selectedFilter, translateQFileDialogOptions(options));
	}

	static QStringList filedialog_open_filenames(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options)
	{
		return DuiFileDialog::getOpenFileNames(parent, caption, dir, filter, selectedFilter, translateQFileDialogOptions(options));
	}

	static QString filedialog_save_filename(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options)
	{
		return DuiFileDialog::getSaveFileName(parent, caption, dir, filter, selectedFilter, translateQFileDialogOptions(options));
	}

	static QFileDialogHook gs_instance;
};

QFileDialogHook QFileDialogHook::gs_instance;
#endif // DUI_NO_HOOK_QFILEDIALOG_STATIC_METHOD
END_ANONYMOUS_NAMESPACE

DuiFileDialogPrivate::DuiFileDialogPrivate( const QString &caption /*= QString()*/, const QString &dir /*= QString()*/, const QString &filter /*= QString()*/, QString *selectedFilter /*= 0*/, DuiFileDialog::Options options /*= 0*/ )
: m_caption(caption)
, m_directory(dir)
, m_filter(filter)
, m_outerSelectedFilter(selectedFilter)
, m_selectedFilter(selectedFilter ? *selectedFilter : QString())
, m_options(options)
, m_isDefaultDirectory(false)
, m_inLoop(false)
, m_ofn(NULL)
{

}

DuiFileDialogPrivate::~DuiFileDialogPrivate()
{
#ifdef Q_WS_WIN
	clean_OFN();
#endif // Q_WS_WIN
}

void DuiFileDialogPrivate::exec()
{
	DialogCode result = Accepted;
	m_selectedFiles.clear();

#ifdef Q_WS_WIN
	if (QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA && QSysInfo::WindowsVersion < QSysInfo::WV_NT_based && ensure_Shell32Function(ENSURE_FUNC_ARGS(SHCreateItemFromParsingName)))
	{
		result = exec_vistaLater();
	}
	else
	{
		result = exec_2k_xp();
	}
#endif // Q_WS_WIN
	emitResult(result);

	if (!q_ptr)
	{
		deleteLater();
	}
	else
	{
		q_ptr->close();
	}
}

void DuiFileDialogPrivate::endDialog_sys()
{
#ifdef Q_WS_WIN
	dui_win_close_dialog(parentWId(), ::GetCurrentThreadId());
#endif // Q_WS_WIN
}

QString &DuiFileDialogPrivate::directory()
{
	return m_directory;
}

WId DuiFileDialogPrivate::parentWId() const
{
	Q_Q(const DuiFileDialog);

	QWidget *parentWidget = q->parentWidget();
	return parentWidget ? parentWidget->effectiveWinId() : NULL;
}

void DuiFileDialogPrivate::showFileDialog( DuiFileDialog *dialog, const QObject *receiver /*= NULL*/, const char *member /*= NULL*/, bool isAsync /*= false*/ )
{
	dialog->show();
	if (isAsync) // async
	{
		dialog->setAttribute(Qt::WA_DeleteOnClose);
		dialog->d_ptr->m_outerSelectedFilter = NULL;
		if (receiver && member)
		{
			receiver->connect(dialog, SIGNAL(finished(const QStringList &)), member);
		}
	}
	else // sync
	{
		QApplication::sendPostedEvents(dialog->d_ptr, Event::Exec);
	}
}

#ifdef Q_WS_WIN
DuiFileDialogPrivate::DialogCode DuiFileDialogPrivate::exec_2k_xp()
{
	switch (m_options & DuiFileDialog::ModeMask)
	{
	case DuiFileDialog::OpenFile:
	case DuiFileDialog::OpenFiles:
	case DuiFileDialog::SaveFile:
		return xp_fileDialog();
	case DuiFileDialog::ShowDirsOnly:
		return xp_folderDialog();
	}

	Q_ASSERT(false);
	return Rejected;
}

HRESULT GetFileNameFromShellItem(IShellItem* pShellItem, SIGDN type, std::wstring & str)
{
    Q_ASSERT(pShellItem != NULL);

    LPWSTR lpstrName = NULL;
    HRESULT hRet = pShellItem->GetDisplayName(type, &lpstrName);

    if(SUCCEEDED(hRet))
    {
        str = lpstrName;
        ::CoTaskMemFree(lpstrName);
    }

    return hRet;
}

class CFileDialogEvents : public IFileDialogEvents
{
public: //IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
        {
            if(riid == __uuidof(IUnknown) || riid == __uuidof(IFileDialogEvents))
            {
                *ppvObject = this;
                AddRef();
                return S_OK;
            }else
            {
                return E_NOINTERFACE;
            }
        }

    virtual ULONG STDMETHODCALLTYPE AddRef( void)
    {
        return (ULONG)::InterlockedIncrement(&m_lRef);
    }

    virtual ULONG STDMETHODCALLTYPE Release( void)
    {
        LONG lRef = ::InterlockedDecrement(&m_lRef);
        if(lRef == 0)
        {
            delete this;
        }
        return lRef;
    }
    
public://IFileDialogEvents
    virtual HRESULT STDMETHODCALLTYPE OnFileOk( 
        /* [in] */ __RPC__in_opt IFileDialog *pfd)
        {
            DuiComPtr<IShellItem> spShellItem;
            if(SUCCEEDED(pfd->GetResult(&spShellItem)))
            {//选择文件夹时如果文件夹的路径为空（vista的库）时，不能返回IDOK
                std::wstring strPath;
                GetFileNameFromShellItem(spShellItem,SIGDN_FILESYSPATH,strPath);
                if(strPath.empty()) return S_FALSE;
            }

            return S_OK;
        }
    virtual HRESULT STDMETHODCALLTYPE OnFolderChanging( 
        /* [in] */ __RPC__in_opt IFileDialog *pfd,
        /* [in] */ __RPC__in_opt IShellItem *psiFolder)
    {
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE OnFolderChange( 
        /* [in] */ __RPC__in_opt IFileDialog *pfd)
    {
        HWND hWnd = ::GetActiveWindow();
        HWND hBtn = ::GetDlgItem(hWnd,IDOK);
        if(!hBtn) return S_OK;
        wchar_t szBtnClass[31]={0};
        ::GetClassNameW(hBtn,szBtnClass,30);
        if(wcscmp(szBtnClass,L"Button")!=0) return S_OK;

        DWORD dwOpt=0;
        if(pfd->GetOptions(&dwOpt) == S_OK && dwOpt & FOS_PICKFOLDERS)
        {//选择文件夹时禁止选择win7库目录
            BOOL bEnable = FALSE;
            IShellItem *pItem=NULL;
            HRESULT hr=pfd->GetFolder(&pItem);
            if(SUCCEEDED(hr))
            {
                std::wstring strFolder;
                GetFileNameFromShellItem(pItem,SIGDN_FILESYSPATH,strFolder);
                if(!strFolder.empty())
                {
                   bEnable =TRUE; 
                }
            }
            ::EnableWindow(hBtn,bEnable);
        }

        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE OnSelectionChange( 
        /* [in] */ __RPC__in_opt IFileDialog *pfd)
    {
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE OnShareViolation( 
        /* [in] */ __RPC__in_opt IFileDialog *pfd,
        /* [in] */ __RPC__in_opt IShellItem *psi,
        /* [out] */ __RPC__out FDE_SHAREVIOLATION_RESPONSE *pResponse)
    {
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE OnTypeChange( 
        /* [in] */ __RPC__in_opt IFileDialog *pfd)
    {
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE OnOverwrite( 
        /* [in] */ __RPC__in_opt IFileDialog *pfd,
        /* [in] */ __RPC__in_opt IShellItem *psi,
        /* [out] */ __RPC__out FDE_OVERWRITE_RESPONSE *pResponse)
    {
        return S_OK;
    } 
    
public:
    static CFileDialogEvents * CreateInstance()
    {
        return new CFileDialogEvents;
    }
protected:
    CFileDialogEvents():m_lRef(0){}
    ~CFileDialogEvents()
    {
    
    }
    LONG m_lRef;

};

DuiFileDialogPrivate::DialogCode DuiFileDialogPrivate::exec_vistaLater()
{
	DuiComPtr<IFileOpenDialog> spFileOpenDialog;
	DuiComPtr<IFileDialog> spFileDialog;
	DWORD dwEvtCookie =0;
	
	
	HRESULT hr = S_OK;
	switch (m_options & DuiFileDialog::ModeMask)
	{
	case DuiFileDialog::OpenFiles:
	case DuiFileDialog::OpenFile:
	case DuiFileDialog::ShowDirsOnly:
		{
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&spFileOpenDialog));
			spFileDialog = spFileOpenDialog;
		}
		break;
	case DuiFileDialog::SaveFile:
		{
			DuiComPtr<IFileSaveDialog> spFileSaveDialog;
			hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&spFileSaveDialog));
			LogFinal(LOG::KDuiFw) << "save file last error:" << GetLastError(); // 卡
			spFileDialog = spFileSaveDialog;
		}
		break;
	}
	HRESULT_CHECK_2(hr, return Rejected);

	if (!make_IFileDialog(spFileDialog))
	{
		LogFinal(LOG::KDuiFw) << "Make file dialog faile.";
		return Rejected;
	}

	m_inLoop = true;
    DuiComPtr<IFileDialogEvents> spFDEvts;
    spFDEvts = CFileDialogEvents::CreateInstance();
	spFileDialog->Advise(spFDEvts,&dwEvtCookie);
	hr = spFileDialog->Show(parentWId());
	spFileDialog->Unadvise(dwEvtCookie);
	m_inLoop = false;
	HRESULT_CHECK_2(hr, return Rejected);

	UINT filterIndex = 0;
	hr = spFileDialog->GetFileTypeIndex(&filterIndex);
	HRESULT_CHECK_2(hr, return Rejected);
	setSelectedFilter(filterIndex - 1);

	if (m_options & DuiFileDialog::OpenFiles)
	{
		DuiComPtr<IShellItemArray> spShellItemArray;
		hr = spFileOpenDialog->GetResults(&spShellItemArray);
		HRESULT_CHECK_2(hr, return Rejected);

		DWORD count = 0;
		hr = spShellItemArray->GetCount(&count);
		HRESULT_CHECK_2(hr, return Rejected);

		for (DWORD i = 0; i < count; ++i)
		{
			DuiComPtr<IShellItem> spShellItem;
			hr = spShellItemArray->GetItemAt(i, &spShellItem);
			HRESULT_CHECK_2(hr, continue);

			appendShellItem(spShellItem);
		}
	}
	else
	{
		DuiComPtr<IShellItem> spShellItem;
		hr = spFileDialog->GetResult(&spShellItem);
		HRESULT_CHECK_2(hr, return Rejected);

		appendShellItem(spShellItem);
	}

	return Accepted;
}

DuiFileDialogPrivate::DialogCode DuiFileDialogPrivate::xp_fileDialog()
{
	make_OFN();

	m_inLoop = true;
	BOOL ret = TRUE;
	bool multiSelected = false;
	switch (m_options & DuiFileDialog::ModeMask)
	{
	case DuiFileDialog::OpenFiles:
		multiSelected = true;
	case DuiFileDialog::OpenFile:
		ret = ::GetOpenFileName(m_ofn);
		break;
	case DuiFileDialog::SaveFile:
		ret = ::GetSaveFileName(m_ofn);
		break;
	}
	m_inLoop = false;

	if (ret == FALSE)
		return Rejected;

	setSelectedFilter(m_ofn->nFilterIndex - 1);

	QString path(QString::fromWCharArray(m_ofn->lpstrFile));
	QFileInfo fi(path);
	if (multiSelected && m_ofn->lpstrFile[path.length() + 1] != 0)
	{
		int offset = path.length() + 1;
		QDir dir(path);
		QString f;
		while(!(f = QString::fromWCharArray(m_ofn->lpstrFile + offset)).isEmpty())
		{
			fi.setFile(dir, f);
			QString res = fi.absoluteFilePath();
			if (!res.isEmpty())
			{
				m_selectedFiles.append(res);
			}
			offset += f.length() + 1;
		}
	}
	else
	{
		m_selectedFiles.push_back(fi.absoluteFilePath());
	}

	return Accepted;
}

DuiFileDialogPrivate::DialogCode DuiFileDialogPrivate::xp_folderDialog()
{
	if (!ensure_Shell32Function(ENSURE_FUNC_ARGS(SHBrowseForFolderW))
		|| !ensure_Shell32Function(ENSURE_FUNC_ARGS(SHGetPathFromIDListW))
		|| !ensure_Shell32Function(ENSURE_FUNC_ARGS(SHGetMalloc)))
		return Rejected;

	initDirectory();

	wchar_t displayName[MAX_PATH] = {0};

	BROWSEINFOW bi;
	bi.hwndOwner = parentWId();
	bi.pidlRoot = NULL;
	//### This does not seem to be respected? - the dialog always displays "Browse for folder"
	bi.lpszTitle = reinterpret_cast<LPCWSTR>(m_caption.utf16());
	bi.pszDisplayName = displayName;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_NEWDIALOGSTYLE;
	bi.lpfn = dui_win_browse_callback;
	bi.lParam = (LPARAM)this;

	LPITEMIDLIST pItemIDList = funcSHBrowseForFolderW(&bi);
	if (!pItemIDList)
		return Rejected;

	wchar_t result[MAX_PATH] = {0};
	funcSHGetPathFromIDListW(pItemIDList, result);
	DuiComPtr<IMalloc> spMalloc;
	if (funcSHGetMalloc(&spMalloc) == NOERROR)
	{
		spMalloc->Free(pItemIDList);
	}
	m_selectedFiles.append(QString::fromWCharArray(result));

	return Accepted;
}

void DuiFileDialogPrivate::make_OFN()
{
	clean_OFN();

	initDirectory();

	int maxLen = (m_options & DuiFileDialog::ModeMask) == DuiFileDialog::OpenFiles ? KMaxMultiNameLen : KMaxNameLen;
	wchar_t *tInitSel = new wchar_t[maxLen + 1];
	if (m_filename.length() > 0 && m_filename.length() <= maxLen)
	{
		memcpy(tInitSel, m_filename.utf16(), (m_filename.length() + 1) * sizeof(wchar_t));
	}
	else
	{
		tInitSel[0] = 0;
	}

	Q_ASSERT(!m_ofn);

	m_ofn = new OPENFILENAME;
	memset(m_ofn, 0, sizeof(OPENFILENAME));
	
	QStringList filterList(dui_make_filters_list(m_filter));
	m_defaultSuffix = resolveSuffixFromFileName(filterList);
	if (m_options & DuiFileDialog::HideNameFilterDetails)
	{
		m_winFilters = dui_win_make_filters_list(filterList).join(QChar()).append(QChar());
	}
	else
	{
		m_winFilters.clear();

		QStringList list(dui_win_make_filters_list(filterList));
		for (int i = 0, c = list.length(); i < c; i += 2)
		{
			m_winFilters.append(QString("%1 (%2)").arg(list[i]).arg(list[i + 1]));
			m_winFilters.append(QChar());
			m_winFilters.append(list[i + 1]);
			m_winFilters.append(QChar());
		}
		m_winFilters.append(QChar());
	}

	DuiFileDialog::Options mode = (DuiFileDialog::Options)(m_options & DuiFileDialog::ModeMask);
	m_ofn->lStructSize = sizeof(OPENFILENAME);
	m_ofn->hwndOwner = parentWId();
	m_ofn->lpstrFilter = (wchar_t*)m_winFilters.utf16();
	m_ofn->nFilterIndex = selectedFilterIndex(filterList) + 1;
	m_ofn->lpstrFile = tInitSel;
	m_ofn->nMaxFile = maxLen;
	m_ofn->lpstrInitialDir = (wchar_t*)m_directory.utf16();
	m_ofn->lpstrTitle = (wchar_t*)m_caption.utf16();
	m_ofn->lpstrDefExt = reinterpret_cast<LPCTSTR>(m_defaultSuffix.utf16());
	m_ofn->lCustData = (LPARAM)this;
	m_ofn->Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST;
	if (mode == DuiFileDialog::OpenFile || mode == DuiFileDialog::OpenFiles)
	{
		m_ofn->Flags |= OFN_FILEMUSTEXIST;
	}
	if (mode == DuiFileDialog::OpenFiles)
	{
		m_ofn->Flags |= OFN_ALLOWMULTISELECT;
	}
	if (!(m_options & DuiFileDialog::DontConfirmOverwrite))
	{
		m_ofn->Flags |= OFN_OVERWRITEPROMPT;
	}
}

void DuiFileDialogPrivate::clean_OFN()
{
	if (m_ofn)
	{
		delete [] m_ofn->lpstrFile;
		delete m_ofn;
		m_ofn = NULL;
	}
}

bool DuiFileDialogPrivate::make_IFileDialog( IFileDialog *dialog )
{
	initDirectory();

	HRESULT hr;
	if (!m_isDefaultDirectory)
	{
		DuiComPtr<IShellItem> spShellItem;
		hr = funcSHCreateItemFromParsingName(reinterpret_cast<LPCTSTR>(m_directory.utf16()), NULL, IID_PPV_ARGS(&spShellItem));

		if (SUCCEEDED(hr))
		{
			hr = dialog->SetFolder(spShellItem);
		}
		else
		{
			LogFinal(LOG::KDuiFw) << "funcSHCreateItemFromParsingName failed.";
		}
	}

	if (!m_filename.isEmpty())
	{
		hr = dialog->SetFileName(reinterpret_cast<LPCTSTR>(m_filename.utf16()));
	}

	if (!m_caption.isEmpty())
	{
		hr = dialog->SetTitle(reinterpret_cast<LPCTSTR>(m_caption.utf16()));
	}

	QStringList filterList(dui_make_filters_list(m_filter));
	m_defaultSuffix = resolveSuffixFromFileName(filterList);
	if (!m_defaultSuffix.isEmpty())
	{
		hr = dialog->SetDefaultExtension(reinterpret_cast<LPCTSTR>(m_defaultSuffix.utf16()));
	}

	if (!filterList.isEmpty())
	{
		QStringList winFilterList(dui_win_make_filters_list(filterList));
		int c = winFilterList.length() / 2;
		COMDLG_FILTERSPEC *filterSpec = new COMDLG_FILTERSPEC[c];
		for (int i = 0; i < c; ++i)
		{
			filterSpec[i].pszName = reinterpret_cast<LPCTSTR>(winFilterList[i * 2].utf16());
			filterSpec[i].pszSpec = reinterpret_cast<LPCTSTR>(winFilterList[i * 2 + 1].utf16());
		}
		dialog->SetFileTypes(c, filterSpec);
		dialog->SetFileTypeIndex(selectedFilterIndex(filterList) + 1);
		delete [] filterSpec;
	}

	DWORD options;
	hr = dialog->GetOptions(&options);
	if (SUCCEEDED(hr))
	{
		options |= FOS_NOCHANGEDIR;
		DuiFileDialog::Options mode = m_options & DuiFileDialog::ModeMask;
		if (mode == DuiFileDialog::OpenFile || mode == DuiFileDialog::OpenFiles)
		{
			options |= FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST;
		}
		if (mode == DuiFileDialog::OpenFiles)
		{
			options |= FOS_ALLOWMULTISELECT;
		}
		if (!(m_options & DuiFileDialog::DontConfirmOverwrite))
		{
			options |= FOS_OVERWRITEPROMPT;
		}
		if (mode == DuiFileDialog::ShowDirsOnly)
		{
			options |= FOS_PICKFOLDERS;
		}
		hr = dialog->SetOptions(options);
	}

	return true;
}
#endif // Q_WS_WIN

bool DuiFileDialogPrivate::event(QEvent *e)
{
	switch (e->type())
	{
	case Event::Exec:
		if (!m_inLoop)
		{
			exec();
		}

		return true;
	case QEvent::DeferredDelete:
		if (m_inLoop)
		{
			// Please don't delete me, I'll commit suicide later QAQ
			return true;
		}
		break;
	}

	return QObject::event(e);
}

void DuiFileDialogPrivate::initDirectory()
{
	if (m_directory.startsWith(QLatin1String("file:")))
	{
		m_directory.remove(0, 5);
	}
	QFileInfo fi(m_directory);

	m_isDefaultDirectory = false;
	if (!m_directory.isEmpty())
	{
		if (fi.isDir())
		{
			m_directory = fi.absoluteFilePath();
		}
		else
		{
			m_directory = fi.absolutePath();
			m_filename = fi.fileName();
		}
	}
	else
	{
		m_directory = QDir::homePath();
		m_isDefaultDirectory = true;
	}

	m_directory = QDir::toNativeSeparators(m_directory);
	if (!m_filename.isEmpty())
	{
		m_filename.remove(QLatin1Char('<'))
			.remove(QLatin1Char('>'))
			.remove(QLatin1Char('\"'))
			.remove(QLatin1Char('|'));
	}
}

QString DuiFileDialogPrivate::resolveSuffixFromFileName( const QStringList &filterList )
{
	QRegExp r(QString::fromLatin1(KFileDialogFilterRegexp));
	//match selected filterlist
	if (!m_selectedFilter.isEmpty() || m_filename.isEmpty() || (m_options & DuiFileDialog::DontResolveSuffix))
	{
		QString filter = m_selectedFilter;
		if (filter.isEmpty() && !filterList.isEmpty())
		{
			filter = filterList.first();
		}
		if (r.indexIn(filter) >= 0)
		{
			QStringList extList = dui_extract_filter(r);
			if (!extList.isEmpty())
			{
				return QFileInfo(extList.first()).completeSuffix();
			}
		}
		return QString();
	}
	//match all filterlist
	foreach (QString filter, filterList)
	{
		if (r.indexIn(filter) < 0)
			continue;

		foreach (QString wildcard, dui_extract_filter(r))
		{
			QRegExp reg(wildcard.trimmed());
			reg.setPatternSyntax(QRegExp::Wildcard);
			if (reg.exactMatch(m_filename))
			{
				m_selectedFilter = filter;
				return QFileInfo(wildcard).completeSuffix();
			}
		}
	}

	return QFileInfo(m_filename).suffix();
}

void DuiFileDialogPrivate::emitResult( const DialogCode result )
{
	if (!m_selectedFiles.isEmpty())
	{
		QFileInfo fi(m_selectedFiles.first());
		if (fi.isDir())
		{
			m_directory = fi.absoluteFilePath();
			m_filename.clear();
		}
		else
		{
			m_directory = fi.absolutePath();
			m_filename = fi.fileName();
		}
	}

	if (q_ptr)
	{
		emit q_ptr->finished(m_selectedFiles);
	}
}

int DuiFileDialogPrivate::selectedFilterIndex( const QStringList &filterList ) const
{
	return m_selectedFilter.isEmpty() ? -1 : filterList.indexOf(m_selectedFilter);
}

void DuiFileDialogPrivate::setSelectedFilter( const qint32 index )
{
	if (index < 0)
		return;

	QStringList filterList(dui_make_filters_list(m_filter));
	if (index >= filterList.length() || m_selectedFilter == filterList[index])
		return;

	m_selectedFilter = filterList[index];

	if (m_outerSelectedFilter)
	{
		*m_outerSelectedFilter = m_selectedFilter;
	}
}

void DuiFileDialogPrivate::appendShellItem( IShellItem *shellItem )
{
	PWSTR pszFilePath = NULL;
	HRESULT hr = shellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
	HRESULT_CHECK(hr);

	QFileInfo fi(QString::fromWCharArray(pszFilePath));
	m_selectedFiles.push_back(fi.absoluteFilePath());

	CoTaskMemFree(pszFilePath);
}

/************************************************************************/
/* DuiFileDialog                                                        */
/************************************************************************/
DuiFileDialog::DuiFileDialog( DuiFileDialogPrivate &dd, QWidget *parent )
: QWidget(parent)
, d_ptr(&dd)
{
	Q_D(DuiFileDialog);

	d->q_ptr = this;

	setAttribute(Qt::WA_DontShowOnScreen);
	setAttribute(Qt::WA_QuitOnClose, false);
	if (parent)
	{
		setWindowModality(d->m_options & ApplicationModal ? Qt::ApplicationModal : Qt::WindowModal);
	}
	else if (!(d->m_options & NoApplicationModal))
	{
		setWindowModality(Qt::ApplicationModal);
	}
}

DuiFileDialog::~DuiFileDialog()
{
	d_ptr->q_ptr = NULL;
	d_ptr->deleteLater();
}

DuiFileDialog::Options DuiFileDialog::options() const
{
	Q_D(const DuiFileDialog);
	return d->m_options;
}

void DuiFileDialog::setOptions( const Options &o )
{
	Q_D(DuiFileDialog);
	d->m_options = o;
}

QStringList DuiFileDialog::selectedFiles() const
{
	Q_D(const DuiFileDialog);
	return d->m_selectedFiles;
}

QString DuiFileDialog::selectedFile() const
{
	Q_D(const DuiFileDialog);
	return d->m_selectedFiles.isEmpty() ? QString() : d->m_selectedFiles.first();
}

QString DuiFileDialog::selectedFilter() const
{
	Q_D(const DuiFileDialog);
	return d->m_selectedFilter;
}

/*!
    \reimp
*/
void DuiFileDialog::showEvent( QShowEvent *e )
{
#ifdef Q_WS_WIN
	Q_D(DuiFileDialog);

	QApplication::postEvent(d, new QEvent((QEvent::Type)DuiFileDialogPrivate::Event::Exec));
#endif // Q_WS_WIN

	QWidget::showEvent(e);
}

/*!
    \reimp
*/
void DuiFileDialog::hideEvent( QHideEvent *e )
{
#ifdef Q_WS_WIN
	Q_D(DuiFileDialog);

	QApplication::removePostedEvents(d, (QEvent::Type)DuiFileDialogPrivate::Event::Exec);

	d->endDialog_sys();
#endif // Q_WS_WIN

	QWidget::hideEvent(e);
}

QString DuiFileDialog::getOpenFileName( QWidget *parent /*= 0*/, const QString &caption /*= QString()*/, const QString &dir /*= QString()*/, const QString &filter /*= QString()*/, QString *selectedFilter /*= 0*/, Options options /*= 0*/ )
{
	DuiFileDialog fileDialog(*(new DuiFileDialogPrivate(caption, dir, filter, selectedFilter, options & ~ModeMask | OpenFile)), parent);
	DuiFileDialogPrivate::showFileDialog(&fileDialog);

	return fileDialog.selectedFile();
}

QString DuiFileDialog::getSaveFileName( QWidget *parent /*= 0*/, const QString &caption /*= QString()*/, const QString &dir /*= QString()*/, const QString &filter /*= QString()*/, QString *selectedFilter /*= 0*/, Options options /*= 0*/ )
{
	DuiFileDialog fileDialog(*(new DuiFileDialogPrivate(caption, dir, filter, selectedFilter, options & ~0xF | SaveFile)), parent);
	DuiFileDialogPrivate::showFileDialog(&fileDialog);

	return fileDialog.selectedFile();
}

QString DuiFileDialog::getExistingDirectory( QWidget *parent /*= 0*/, const QString &caption /*= QString()*/, const QString &dir /*= QString()*/, Options options /*= 0*/ )
{
	DuiFileDialog fileDialog(*(new DuiFileDialogPrivate(caption, dir, QString(), NULL, options & ~0xF | ShowDirsOnly)), parent);
	DuiFileDialogPrivate::showFileDialog(&fileDialog);

	return fileDialog.selectedFile();
}

QStringList DuiFileDialog::getOpenFileNames( QWidget *parent /*= 0*/, const QString &caption /*= QString()*/, const QString &dir /*= QString()*/, const QString &filter /*= QString()*/, QString *selectedFilter /*= 0*/, Options options /*= 0*/ )
{
	DuiFileDialog fileDialog(*(new DuiFileDialogPrivate(caption, dir, filter, selectedFilter, options & ~0xF | OpenFiles)), parent);
	DuiFileDialogPrivate::showFileDialog(&fileDialog);

	return fileDialog.selectedFiles();
}

void DuiFileDialog::asyncGetOpenFileName( const QObject *receiver, const char *member, QWidget *parent /*= 0*/, const QString &caption /*= QString()*/, const QString &dir /*= QString()*/, const QString &filter /*= QString()*/, QString *selectedFilter /*= 0*/, Options options /*= 0*/ )
{
	DuiFileDialog *fileDialog = new DuiFileDialog(*(new DuiFileDialogPrivate(caption, dir, filter, selectedFilter, options & ~ModeMask | OpenFile)), parent);
	DuiFileDialogPrivate::showFileDialog(fileDialog, receiver, member, true);
}

void DuiFileDialog::asyncGetSaveFileName( const QObject *receiver, const char *member, QWidget *parent /*= 0*/, const QString &caption /*= QString()*/, const QString &dir /*= QString()*/, const QString &filter /*= QString()*/, QString *selectedFilter /*= 0*/, Options options /*= 0*/ )
{
	DuiFileDialog *fileDialog = new DuiFileDialog(*(new DuiFileDialogPrivate(caption, dir, filter, selectedFilter, options & ~0xF | SaveFile)), parent);
	DuiFileDialogPrivate::showFileDialog(fileDialog, receiver, member, true);
}

void DuiFileDialog::asyncGetExistingDirectory( const QObject *receiver, const char *member, QWidget *parent /*= 0*/, const QString &caption /*= QString()*/, const QString &dir /*= QString()*/, Options options /*= 0*/ )
{
	DuiFileDialog *fileDialog = new DuiFileDialog(*(new DuiFileDialogPrivate(caption, dir, QString(), NULL, options & ~0xF | ShowDirsOnly)), parent);
	DuiFileDialogPrivate::showFileDialog(fileDialog, receiver, member, true);
}

void DuiFileDialog::asyncGetOpenFileNames( const QObject *receiver, const char *member, QWidget *parent /*= 0*/, const QString &caption /*= QString()*/, const QString &dir /*= QString()*/, const QString &filter /*= QString()*/, QString *selectedFilter /*= 0*/, Options options /*= 0*/ )
{
	DuiFileDialog *fileDialog = new DuiFileDialog(*(new DuiFileDialogPrivate(caption, dir, filter, selectedFilter, options & ~0xF | OpenFiles)), parent);
	DuiFileDialogPrivate::showFileDialog(fileDialog, receiver, member, true);
}

void DuiFileDialog::closeFileDialog( QWidget *parent )
{
	foreach (QObject *p, parent->children())
	{
		if (DuiFileDialog *fileDialog = qobject_cast<DuiFileDialog*>(p))
		{
			fileDialog->close();
		}
	}
}
