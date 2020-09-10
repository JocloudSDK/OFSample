#pragma once

class DuiFileDialogPrivate;
class DW_DUIFW_EXPORT DuiFileDialog : public QWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(DuiFileDialog)

public:
	enum Option
	{
		OpenFile              = 0x00000001,
		OpenFiles             = 0x00000002,
		ShowDirsOnly          = 0x00000004,
		SaveFile              = 0x00000008,
		ModeMask              = 0x0000000F,
		DontResolveSymlinks   = 0x00000010,
		DontConfirmOverwrite  = 0x00000020,
		ReadOnly              = 0x00000040,
		HideNameFilterDetails = 0x00000080,
		ApplicationModal      = 0x00000100,
		NoApplicationModal    = 0x00000200,
		DontResolveSuffix     = 0x00000400,
	};
	Q_DECLARE_FLAGS(Options, Option)

	Q_PROPERTY(Options options READ options WRITE setOptions)

public:
	virtual ~DuiFileDialog();

	Options options() const;
	void setOptions(const Options &o);

	QStringList selectedFiles() const;
	QString selectedFile() const;

	QString selectedFilter() const;

signals:
	void finished(const QStringList &result);

protected:
	DuiFileDialog(DuiFileDialogPrivate &dd, QWidget *parent);
	DuiFileDialogPrivate *d_ptr;

	virtual void showEvent(QShowEvent *e);
	virtual void hideEvent(QHideEvent *e);

private:
	explicit DuiFileDialog() {};

public: // static methods
	// sync
	static QString getOpenFileName(QWidget *parent = 0,
		const QString &caption = QString(),
		const QString &dir = QString(),
		const QString &filter = QString(),
		QString *selectedFilter = 0,
		Options options = 0);
	static QString getSaveFileName(QWidget *parent = 0,
		const QString &caption = QString(),
		const QString &dir = QString(),
		const QString &filter = QString(),
		QString *selectedFilter = 0,
		Options options = 0);
	static QString getExistingDirectory(QWidget *parent = 0,
		const QString &caption = QString(),
		const QString &dir = QString(),
		Options options = 0);
	static QStringList getOpenFileNames(QWidget *parent = 0,
		const QString &caption = QString(),
		const QString &dir = QString(),
		const QString &filter = QString(),
		QString *selectedFilter = 0,
		Options options = 0);

	// async - recommend!!!!!
	static void asyncGetOpenFileName(const QObject *receiver,
		const char *member,
		QWidget *parent = 0,
		const QString &caption = QString(),
		const QString &dir = QString(),
		const QString &filter = QString(),
		QString *selectedFilter = 0,
		Options options = 0);
	static void asyncGetSaveFileName(const QObject *receiver,
		const char *member,
		QWidget *parent = 0,
		const QString &caption = QString(),
		const QString &dir = QString(),
		const QString &filter = QString(),
		QString *selectedFilter = 0,
		Options options = 0);
	static void asyncGetExistingDirectory(const QObject *receiver,
		const char *member,
		QWidget *parent = 0,
		const QString &caption = QString(),
		const QString &dir = QString(),
		Options options = 0);
	static void asyncGetOpenFileNames(const QObject *receiver,
		const char *member,
		QWidget *parent = 0,
		const QString &caption = QString(),
		const QString &dir = QString(),
		const QString &filter = QString(),
		QString *selectedFilter = 0,
		Options options = 0);

	static void closeFileDialog(QWidget *parent);
};
