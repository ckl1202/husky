// Chart.cpp : 实现文件
//

#include "stdafx.h"
#include "husky.h"
#include "Chart.h"
#include "afxdialogex.h"


// CChart 对话框

IMPLEMENT_DYNAMIC(CChart, CDialogEx)

CChart::CChart(char *Instrument, CWnd* pParent /*=NULL*/)
	: CDialogEx(CChart::IDD, pParent)
{
	strcpy(InstrumentID, Instrument);
}

CChart::~CChart()
{
	fclose(dataFile);
}

void CChart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRICE_CHARTCTL, m_priceChart);
	DDX_Control(pDX, IDC_VOLUME_CHARTCTL, m_volumeChart);
}


BEGIN_MESSAGE_MAP(CChart, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CChart 消息处理程序


BOOL CChart::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pAxis = NULL;
	pDateAxis = NULL;
	//priceChart's bottom axis
	pDateAxis = m_priceChart.CreateDateTimeAxis(CChartCtrl::BottomAxis);
	pDateAxis->SetAutomatic(false);
	pDateAxis->SetTickLabelFormat(true, _T("%H:%M"));
	COleDateTime tBegin(COleDateTime::GetCurrentTime());
	COleDateTime tEnd(COleDateTime::GetCurrentTime());
	tBegin.SetTime(9, 15, 0);
	tEnd.SetTime(15, 15, 0);
	pDateAxis->SetMinMax(tBegin, tEnd);
	
	//priceChart's left axis
	pAxis = m_priceChart.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	//volumeChart's bottom axis
	pDateAxis = m_volumeChart.CreateDateTimeAxis(CChartCtrl::BottomAxis);
	pDateAxis->SetAutomatic(false);
	pDateAxis->SetTickLabelFormat(true, _T("%H:%M"));
	pDateAxis->SetMinMax(tBegin, tEnd);

	//volumeChart's left axis
	pAxis = m_volumeChart.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	//volumeChart's right axis
	pAxis = m_volumeChart.CreateStandardAxis(CChartCtrl::RightAxis);
	pAxis->SetAutomatic(true);

	//title
	TChartString str1;
	str1 = CString(InstrumentID);
	m_priceChart.GetTitle()->AddString(str1);

	//openFile
	char filename[30] = "data/";
	strcat(filename, InstrumentID);
	strcat(filename, ".txt");
	dataFile = fopen(filename, "r");
	//int year, month, day;
	//fscanf(dataFile, "%d.%d.%d\n", &year, &month, &day);
		
	m_priceChart.EnableRefresh(false);
	m_volumeChart.EnableRefresh(false);

	pLineSerie = m_priceChart.CreateLineSerie();
	pAverSerie = m_priceChart.CreateLineSerie();
	pLineSerie->SetSeriesOrdering(poNoOrdering);
	pAverSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie = m_volumeChart.CreateBarSerie(false, true);
	pHaveSerie = m_volumeChart.CreateLineSerie();
	pHaveSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie->SetBarWidth(1);

	nNumOfData = 0;
	totalPrice = 0;
	preVolume = 0;;
	while (Minute.GetFromFile(dataFile)){
		if (Minute.hour < 9) continue;
		COleDateTime t1(COleDateTime::GetCurrentTime());
		t1.SetTime(Minute.hour, Minute.minu, 0);

		pLineSerie->AddPoint(t1.m_dt, Minute.closePrice);

		++nNumOfData;
		totalPrice += Minute.closePrice;
		pAverSerie->AddPoint(t1.m_dt, totalPrice / nNumOfData);

		pHaveSerie->AddPoint(t1.m_dt, Minute.totalVolume);

		pDealSerie->AddPoint(t1.m_dt, Minute.dealVolume - preVolume);
		preVolume = Minute.dealVolume;
	}

	m_priceChart.EnableRefresh(true);
	m_volumeChart.EnableRefresh(true);
	// TODO:  在此添加额外的初始化
	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CChart::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	while (Minute.GetFromFile(dataFile)){
		if (Minute.hour < 9) continue;
		COleDateTime t1(COleDateTime::GetCurrentTime());
		t1.SetTime(Minute.hour, Minute.minu, 0);

		pLineSerie->AddPoint(t1.m_dt, Minute.closePrice);

		++nNumOfData;
		totalPrice += Minute.closePrice;
		pAverSerie->AddPoint(t1.m_dt, totalPrice / nNumOfData);

		pHaveSerie->AddPoint(t1.m_dt, Minute.totalVolume);

		pDealSerie->AddPoint(t1.m_dt, Minute.dealVolume - preVolume);
		preVolume = Minute.dealVolume;
	}
	CDialogEx::OnTimer(nIDEvent);
}
