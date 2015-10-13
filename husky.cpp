

// husky.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "husky.h"
#include "huskyDlg.h"
#include "Chart.h"
#include "CandleChart.h"
#include "SelectName.h"
#include "DataStruct.h"
#include "ChooseIns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ChuskyApp

BEGIN_MESSAGE_MAP(ChuskyApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// ChuskyApp ����

ChuskyApp::ChuskyApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� ChuskyApp ����

ChuskyApp theApp;


// ChuskyApp ��ʼ��

BOOL ChuskyApp::InitInstance()
{
		//MessageBox(NULL, CString("check1"), CString("1"), MB_OK);
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CWnd* m_pSelectNameWnd;
	CSelectName selectNameDlg;
	m_pSelectNameWnd = &selectNameDlg;
	INT_PTR nResponseSelectName = selectNameDlg.DoModal();


	Chusky dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	/*CChooseIns dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();*/

	/*CCandleChart dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();*/

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

