#pragma once
#include "afxcmn.h"


// CPosition 对话框

class CPosition : public CDialogEx
{
	DECLARE_DYNAMIC(CPosition)

public:
	CPosition(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPosition();

// 对话框数据
	enum { IDD = IDD_POSITION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_positionList;
	virtual BOOL OnInitDialog();
};
