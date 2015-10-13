// Position.cpp : 实现文件
//

#include "stdafx.h"
#include "husky.h"
#include "Position.h"
#include "afxdialogex.h"


// CPosition 对话框

IMPLEMENT_DYNAMIC(CPosition, CDialogEx)

CPosition::CPosition(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPosition::IDD, pParent)
{

}

CPosition::~CPosition()
{
}

void CPosition::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_positionList);
}


BEGIN_MESSAGE_MAP(CPosition, CDialogEx)
END_MESSAGE_MAP()


// CPosition 消息处理程序


BOOL CPosition::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_positionList.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_positionList.InsertColumn(0, CString("合约"), LVCFMT_LEFT, 100);
	m_positionList.InsertColumn(1, CString("开平"), LVCFMT_LEFT, 100);
	m_positionList.InsertColumn(2, CString("今日持仓"), LVCFMT_LEFT, 100);
	m_positionList.InsertColumn(3, CString("今日持仓成本"), LVCFMT_LEFT, 100);
	m_positionList.InsertColumn(4, CString("昨日持仓"), LVCFMT_LEFT, 100);
	m_positionList.InsertColumn(5, CString("昨日持仓成本"), LVCFMT_LEFT, 100);
	m_positionList.InsertColumn(6, CString("占用保证金"), LVCFMT_LEFT, 100);
	
	FILE *posFile;
	posFile = fopen("position.txt", "r");
	char InstrumentID[20];
	int direction, position, YdPosition, YdPositionPrice;
	double price, YdPrice, margin;
	int i = 0;char temp[100];
	while (fscanf(posFile, "%s %d %d %lf %d %lf %lf",
						&InstrumentID,
						&direction,
						&position,
						&price,
						&YdPosition,
						&YdPrice,
						&margin) != EOF){
							
		m_positionList.InsertItem(i, CString(InstrumentID));
		if (direction == 0) m_positionList.SetItemText(i, 1, CString("买"));
		else m_positionList.SetItemText(i, 1, CString("卖"));

		sprintf(temp, "%d", position);
		m_positionList.SetItemText(i, 2, CString(temp));

		sprintf(temp, "%.2lf", price);
		m_positionList.SetItemText(i, 3, CString(temp));

		sprintf(temp, "%d", YdPosition);
		m_positionList.SetItemText(i, 4, CString(temp));

		sprintf(temp, "%.2lf", YdPrice);
		m_positionList.SetItemText(i, 5, CString(temp));

		sprintf(temp, "%.2lf", margin);
		m_positionList.SetItemText(i, 6, CString(temp));
	}


	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
