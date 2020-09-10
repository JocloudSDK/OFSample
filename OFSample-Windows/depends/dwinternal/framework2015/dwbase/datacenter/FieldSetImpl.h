#pragma once

#include "IFieldSet.h"
#include "ITableEdit.h"
#include "TableDataImpl.h"
#include "DataCenterPub.h"
#include "CriticalSection.h"

namespace Data
{
	class CFieldSetDataImpl;

	class CFieldSetImpl : public IFieldSet
	{
	public:
		struct COLS
		{
			BYTE * 			arType; 	//����
			WORD * 			arId;		//id��
			DWORD			dwCount;
		};

		static CFieldSetImpl*	createInstance(const NAME_ID &fieldSetName, CDatabaseContextImpl *pDatabaseContext);

		//IUnk
		virtual void			addRef();
		virtual void			release();
		virtual	IWeakRef*		weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);

		virtual IFieldSetDataPtr	getData();

		//д�ֶ�
		virtual BOOL			setStr(const NAME_ID &name, LPCWSTR lpwszValue);
		virtual BOOL			setDword64(const NAME_ID &name, DWORD64 dw64Value);
		virtual BOOL			setDword(const NAME_ID &name, DWORD dwValue);
		virtual BOOL			setWord(const NAME_ID &name, WORD wValue);
		virtual BOOL			setByte(const NAME_ID &name, BYTE byValue);
		virtual BOOL			setBool(const NAME_ID &name, BOOL bValue);
		virtual BOOL			setBlob(const NAME_ID &name, const BYTE * pbData, DWORD cbSize);
		virtual BOOL			setSubTable(const NAME_ID &name, ITableEdit *pSubTable);
		virtual BOOL			removeData(const NAME_ID &name);

		//���ֶ�
		virtual BOOL			hasData(const NAME_ID &name);
		virtual LPCWSTR			getStr(const NAME_ID &name);
		virtual DWORD64			getDword64(const NAME_ID &name);
		virtual DWORD			getDword(const NAME_ID &name);
		virtual WORD			getWord(const NAME_ID &name);
		virtual BYTE			getByte(const NAME_ID &name);
		virtual BOOL			getBool(const NAME_ID &name);
		virtual const BYTE *	getBlob(const NAME_ID &name, DWORD * lpdwSize);
		virtual comptr<ITableEdit>	getSubTable(const NAME_ID &name);
		
		virtual void			dump();//���ڵ������ºܷ���Ĳ鿴Row������


		virtual void			beginEdit();
		virtual void			endEdit();

		virtual const NAME_ID	getFieldSetName() { return m_fieldSetName; }
		virtual void			getCols(std::vector<COL_PROP> & cols);

		//�¼��ź�
		virtual xsignals::signal<void (comptr<IFieldSetData>)> & getSigChanged() { return m_sigChanged; }
		virtual xsignals::signal<void (comptr<IFieldSetData>)> & getQueuedSigChanged() { return m_sigQueuedChanged; }

		BOOL					init(const FIELDSET_STRUCT *pStruct);	
		BOOL					findCol(const NAME_ID &col, DWORD *lpIndex, E_FIELD *lpType);
		inline const COLS *		getCols() const { return &m_cols; }
		void					getContentStr(std::wstring &wstrOut);
		inline 	BOOL			isStructInitialized() const { return m_bStructInitialized; }

	private:
		CFieldSetImpl(const NAME_ID &fieldSetName, CDatabaseContextImpl *pDatabaseContext);
		~CFieldSetImpl(void);

		BOOL					checkStructInitialized();

		void					detach();			
		BOOL					verifyEditEnable();


		DWORD					m_dwRef;
		CCriticalLock			m_csEditLock;

		NAME_ID					m_fieldSetName;
		COLS					m_cols;
	
		CFieldSetDataImpl *		m_pSetData;		
		BOOL					m_bCloneDataOnWrite; //�Ƿ���Ϊ�޸��Ѿ���¡��һ������

		BOOL					m_bStructInitialized;
		CDatabaseContextImpl *	m_pDatabaseContext;	//���������ü���

		xsignals::signal<void (comptr<IFieldSetData>)> m_sigChanged;
		xsignals::signal<void (comptr<IFieldSetData>)> m_sigQueuedChanged;
	};
};
