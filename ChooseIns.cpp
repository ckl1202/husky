// ChooseIns.cpp : 实现文件
//

#include "stdafx.h"
#include "husky.h"
#include "ChooseIns.h"
#include "afxdialogex.h"


// CChooseIns 对话框

IMPLEMENT_DYNAMIC(CChooseIns, CDialogEx)

CChooseIns::CChooseIns(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChooseIns::IDD, pParent)
{

}

CChooseIns::~CChooseIns()
{
}

void CChooseIns::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_InsList);
}


BEGIN_MESSAGE_MAP(CChooseIns, CDialogEx)
	ON_BN_CLICKED(IDOK, &CChooseIns::OnBnClickedOk)
END_MESSAGE_MAP()


// CChooseIns 消息处理程序


BOOL CChooseIns::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	char text[10000] = "";
	char temp[20];
	FILE *InsFile;
	InsFile = fopen("insList.txt", "r");
	while (fscanf(InsFile, "%s", &temp) != EOF){
		strcat(text, temp);
		strcat(text, "\r\n");
	}
	SetDlgItemText(IDC_EDIT1, CString(text));
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CChooseIns::OnBnClickedOk()
{
	FILE *InsFile;
	InsFile = fopen("insList.txt", "w");
	TCHAR s[100];
	char *tempS = (char *) s;
	int nLineCount = m_InsList.GetLineCount();
	for (int i = 0; i < nLineCount; ++i){
		int len = m_InsList.LineLength(m_InsList.LineIndex(i));
		m_InsList.GetLine(i, s);
		s[len] = '\0';
		fprintf(InsFile, "%s", s);
		fprintf(InsFile, "\n");
		//m_InsList.GetWindowTextW(s);
	}
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
