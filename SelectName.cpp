// SelectName.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SelectName.h"
#include "afxdialogex.h"


// CSelectName �Ի���

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


// CSelectName ��Ϣ�������


BOOL CSelectName::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_selectNameCombobox.AddString(CString("wyd"));
	m_selectNameCombobox.AddString(CString("ckl"));
	m_selectNameCombobox.AddString(CString("guest"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSelectName::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
