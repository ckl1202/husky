#pragma once
#include "afxwin.h"
#include "husky.h"
#include "DataStruct.h"


// CSelectName �Ի���

class CSelectName : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectName)

public:
	CSelectName(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectName();

// �Ի�������
	enum { IDD = IDD_USER_SELECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_selectNameCombobox;
	afx_msg void OnBnClickedOk();
};
