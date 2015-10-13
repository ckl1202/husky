#pragma once
#include "afxwin.h"
#include "husky.h"
#include "DataStruct.h"


// CSelectName 对话框

class CSelectName : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectName)

public:
	CSelectName(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectName();

// 对话框数据
	enum { IDD = IDD_USER_SELECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_selectNameCombobox;
	afx_msg void OnBnClickedOk();
};
