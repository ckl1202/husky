#pragma once
#include "afxcmn.h"


// CPosition �Ի���

class CPosition : public CDialogEx
{
	DECLARE_DYNAMIC(CPosition)

public:
	CPosition(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPosition();

// �Ի�������
	enum { IDD = IDD_POSITION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_positionList;
	virtual BOOL OnInitDialog();
};
