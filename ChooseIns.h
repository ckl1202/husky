#pragma once
#include "afxwin.h"


// CChooseIns �Ի���

class CChooseIns : public CDialogEx
{
	DECLARE_DYNAMIC(CChooseIns)

public:
	CChooseIns(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChooseIns();

// �Ի�������
	enum { IDD = IDD_INSLIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CEdit m_InsList;
};
