// SelectName.cpp : 实现文件
//

#include "stdafx.h"
#include "SelectName.h"
#include "afxdialogex.h"


// CSelectName 对话框

IMPLEMENT_DYNAMIC(CSelectName, CDialogEx)

CSelectName::CSelectName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectName::IDD, pParent)
{

}

CSelectName::~CSelectName()
{
}

void CSelectName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELECT_NAME_COMBO, m_selectNameCombobox);
}


BEGIN_MESSAGE_MAP(CSelectName, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectName::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectName 消息处理程序


BOOL CSelectName::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_selectNameCombobox.AddString(CString("wyd"));
	m_selectNameCombobox.AddString(CString("ckl"));
	m_selectNameCombobox.AddString(CString("guest"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSelectName::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int userName = m_selectNameCombobox.GetCurSel();
	FILE *userNameFile;
	userNameFile = fopen("userName.txt", "w");
	fprintf(userNameFile, "%d", userName);
	if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED){
		fprintf(userNameFile, "\n1");
	}
	else{
		fprintf(userNameFile, "\n0");
	}
	fclose(userNameFile);
	CDialogEx::OnOK();
}
