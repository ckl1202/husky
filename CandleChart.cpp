// CandleChart.cpp : 实现文件
//

#include "stdafx.h"
#include "husky.h"
#include "CandleChart.h"
#include "afxdialogex.h"


// CCandleChart 对话框

IMPLEMENT_DYNAMIC(CCandleChart, CDialogEx)

CCandleChart::CCandleChart(char *Instrument, CWnd* pParent /*=NULL*/)
	: CDialogEx(CCandleChart::IDD, pParent)
{
	strcpy(InstrumentID, Instrument);
}

CCandleChart::~CCandleChart()
{
	fclose(dataFile);
}

void CCandleChart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRICE_CHARTCTL, m_priceChart);
	DDX_Control(pDX, IDC_VOLUME_CHARTCTL, m_volumeChart);
}


BEGIN_MESSAGE_MAP(CCandleChart, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCandleChart::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCandleChart::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CCandleChart::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_LEFT_BUTTON, &CCandleChart::OnBnClickedLeftButton)
	ON_BN_CLICKED(IDC_RIGHT_BUTTON, &CCandleChart::OnBnClickedRightButton)
END_MESSAGE_MAP()


// CCandleChart 消息处理程序


BOOL CCandleChart::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	showMethod = 1;

	pAxis = NULL;
	pPriceDateAxis = NULL;
	pVolumeDateAxis = NULL;
	//priceChart's bottom axis
	pPriceDateAxis = m_priceChart.CreateDateTimeAxis(CChartCtrl::BottomAxis);
	pPriceDateAxis->SetAutomatic(false);
	//pPriceDateAxis->ScrollBarEnabled();
	pPriceDateAxis->SetTickLabelFormat(true, _T("%H:%M"));
	tBegin.SetTime(9, 15, 0);
	tEnd.SetTime(10, 15, 0);
	pPriceDateAxis->SetMinMax(tBegin, tEnd);
	
	//priceChart's left axis
	pAxis = m_priceChart.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	//volumeChart's bottom axis
	pVolumeDateAxis = m_volumeChart.CreateDateTimeAxis(CChartCtrl::BottomAxis);
	pVolumeDateAxis->SetAutomatic(false);
	pVolumeDateAxis->SetTickLabelFormat(true, _T("%H:%M"));
	pVolumeDateAxis->SetMinMax(tBegin, tEnd);

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
		
	char filename[20] = "data/";
	strcat(filename, InstrumentID);
	strcat(filename, ".txt");
	dataFile = fopen(filename, "r");

	pPriceSerie = m_priceChart.CreateCandlestickSerie();
	pPriceSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie = m_volumeChart.CreateBarSerie(false, true);
	pHaveSerie = m_volumeChart.CreateLineSerie();
	pHaveSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie->SetBarWidth(5);

	preVolume = 0;
	while (Minute.GetFromFile(dataFile)){
		if (Minute.hour < 9) continue;
		COleDateTime t1(COleDateTime::GetCurrentTime());
		t1.SetTime(Minute.hour, Minute.minu, 0);

		pPriceSerie->AddPoint(t1.m_dt, Minute.lowPrice, Minute.highPrice, Minute.openPrice, Minute.closePrice);

		pHaveSerie->AddPoint(t1.m_dt, Minute.totalVolume);

		pDealSerie->AddPoint(t1.m_dt, Minute.dealVolume - preVolume);
		preVolume = Minute.dealVolume;
	}

	m_priceChart.EnableRefresh(true);
	m_volumeChart.EnableRefresh(true);

	SetTimer(1, 1000, NULL);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCandleChart::OnBnClickedButton1()
{
	if (showMethod == 1) return;
	GetDlgItem(IDC_LEFT_BUTTON)->EnableWindow(true);
	GetDlgItem(IDC_RIGHT_BUTTON)->EnableWindow(true);
	fclose(dataFile);
	char filename[20] = "data/";
	strcat(filename, InstrumentID);
	strcat(filename, ".txt");
	dataFile = fopen(filename, "r");
	m_priceChart.RemoveAllSeries();
	m_volumeChart.RemoveAllSeries();

	pPriceDateAxis->SetTickLabelFormat(true, _T("%H:%M"));
	pVolumeDateAxis->SetTickLabelFormat(true, _T("%H:%M"));
	tBegin.SetTime(9, 15, 0);
	tEnd.SetTime(10, 15, 0);
	pPriceDateAxis->SetAutomatic(false);
	pPriceDateAxis->SetMinMax(tBegin, tEnd);
	pVolumeDateAxis->SetAutomatic(false);
	pVolumeDateAxis->SetMinMax(tBegin, tEnd);

	pPriceSerie = m_priceChart.CreateCandlestickSerie();
	pPriceSerie->SetSeriesOrdering(poNoOrdering);
	//pPriceSerie->SetWidth(1);
	pDealSerie = m_volumeChart.CreateBarSerie(false, true);
	pHaveSerie = m_volumeChart.CreateLineSerie();
	pHaveSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie->SetBarWidth(5);

	preVolume = 0;

	while (Minute.GetFromFile(dataFile)){
		if (Minute.hour < 9) continue;
		COleDateTime t1(COleDateTime::GetCurrentTime());
		t1.SetTime(Minute.hour, Minute.minu, 0);

		pPriceSerie->AddPoint(t1.m_dt, Minute.lowPrice, Minute.highPrice, Minute.openPrice, Minute.closePrice);

		pHaveSerie->AddPoint(t1.m_dt, Minute.totalVolume);

		pDealSerie->AddPoint(t1.m_dt, Minute.dealVolume - preVolume);
		preVolume = Minute.dealVolume;
	}

	showMethod = 1;
	// TODO: 在此添加控件通知处理程序代码
}


void CCandleChart::OnBnClickedButton2()
{
	if (showMethod == 2) return;
	GetDlgItem(IDC_LEFT_BUTTON)->EnableWindow(false);
	GetDlgItem(IDC_RIGHT_BUTTON)->EnableWindow(false);
	fclose(dataFile);
	char filename[30] = "data/";
	strcat(filename, InstrumentID);
	strcat(filename, ".txt");
	dataFile = fopen(filename, "r");
	m_priceChart.RemoveAllSeries();
	m_volumeChart.RemoveAllSeries();

	pPriceDateAxis->SetTickLabelFormat(true, _T("%H:%M"));
	pVolumeDateAxis->SetTickLabelFormat(true, _T("%H:%M"));
	tBegin.SetTime(9, 15, 0);
	tEnd.SetTime(15, 15, 0);
	pPriceDateAxis->SetAutomatic(false);
	pPriceDateAxis->SetMinMax(tBegin, tEnd);
	pVolumeDateAxis->SetAutomatic(false);
	pVolumeDateAxis->SetMinMax(tBegin, tEnd);

	pPriceSerie = m_priceChart.CreateCandlestickSerie();
	pPriceSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie = m_volumeChart.CreateBarSerie(false, true);
	pHaveSerie = m_volumeChart.CreateLineSerie();
	pHaveSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie->SetBarWidth(5);

	numOfData = 0;
	preVolume = 0;

	while (Minute.GetFromFile(dataFile)){
		if (Minute.hour < 9) continue;
		COleDateTime t1(COleDateTime::GetCurrentTime());
		t1.SetTime(Minute.hour, Minute.minu - 4, 0);

		if (Minute.highPrice > high) high = Minute.highPrice;
		if (Minute.lowPrice < low) low = Minute.lowPrice;

		++numOfData;
		if (numOfData == 1){
			open = Minute.openPrice;
			high = Minute.highPrice;
			low = Minute.lowPrice;
		}
		if (numOfData == 5){
			close = Minute.closePrice;
			pPriceSerie->AddPoint(t1.m_dt, low, high, open, close);
			pHaveSerie->AddPoint(t1.m_dt, Minute.totalVolume);
			pDealSerie->AddPoint(t1.m_dt, Minute.dealVolume - preVolume);
			preVolume = Minute.dealVolume;
			numOfData = 0;
		}
	}
	showMethod = 2;
	// TODO: 在此添加控件通知处理程序代码
}


void CCandleChart::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (showMethod == 1){
		while (Minute.GetFromFile(dataFile)){
			if (Minute.hour < 9) continue;
			COleDateTime t1(COleDateTime::GetCurrentTime());
			t1.SetTime(Minute.hour, Minute.minu, 0);

			pPriceSerie->AddPoint(t1.m_dt, Minute.lowPrice, Minute.highPrice, Minute.openPrice, Minute.closePrice);

			pHaveSerie->AddPoint(t1.m_dt, Minute.totalVolume);

			pDealSerie->AddPoint(t1.m_dt, Minute.dealVolume - preVolume);
			preVolume = Minute.dealVolume;
		}
	}
	if (showMethod == 2){

		while (Minute.GetFromFile(dataFile)){
			if (Minute.hour < 9) continue;
			COleDateTime t1(COleDateTime::GetCurrentTime());
			t1.SetTime(Minute.hour, Minute.minu - 4, 0);

			if (Minute.highPrice > high) high = Minute.highPrice;
			if (Minute.lowPrice < low) low = Minute.lowPrice;

			++numOfData;
			if (numOfData == 1){
				open = Minute.openPrice;
				high = Minute.highPrice;
				low = Minute.lowPrice;
			}
			if (numOfData == 5){
				close = Minute.closePrice;
				pPriceSerie->AddPoint(t1.m_dt, low, high, open, close);
				pHaveSerie->AddPoint(t1.m_dt, Minute.totalVolume);
				pDealSerie->AddPoint(t1.m_dt, Minute.dealVolume - preVolume);
				preVolume = Minute.dealVolume;
				numOfData = 0;
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CCandleChart::OnBnClickedButton3()
{
	if (showMethod == 3) return;
	GetDlgItem(IDC_LEFT_BUTTON)->EnableWindow(false);
	GetDlgItem(IDC_RIGHT_BUTTON)->EnableWindow(false);
	fclose(dataFile);
	char filename[30] = "data/";
	strcat(filename, InstrumentID);
	strcat(filename, "K.txt");
	dataFile = fopen(filename, "a+");
	fclose(dataFile);
	dataFile = fopen(filename, "r");
	m_priceChart.RemoveAllSeries();
	m_volumeChart.RemoveAllSeries();

	pPriceDateAxis->SetTickLabelFormat(false, _T("%m.%d"));
	pPriceDateAxis->SetAutomatic(true);
	pVolumeDateAxis->SetTickLabelFormat(false, _T("%m.%d"));
	pVolumeDateAxis->SetAutomatic(true);

	pPriceSerie = m_priceChart.CreateCandlestickSerie();
	pPriceSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie = m_volumeChart.CreateBarSerie(false, true);
	pHaveSerie = m_volumeChart.CreateLineSerie();
	pHaveSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie->SetSeriesOrdering(poNoOrdering);
	pDealSerie->SetBarWidth(1);

	int year, month, day;
	double openPrice, closePrice, highPrice, lowPrice;
	int totalVolume, dealVolume;
	while (fscanf(dataFile,
		"%d/%d/%d %lf %lf %lf %lf %d %d\n",
								&year, 
								&month,
								&day,
								&openPrice,
								&closePrice,
								&highPrice,
								&lowPrice,
								&totalVolume,
								&dealVolume) != EOF){
		COleDateTime t1(year, month, day, 0, 0, 0);
		t1.SetDate(year, month, day);
		//t1.SetTime(0, 0, 0);
		pPriceSerie->AddPoint(t1.m_dt, lowPrice, highPrice, openPrice, closePrice);

		pHaveSerie->AddPoint(t1.m_dt, totalVolume);

		pDealSerie->AddPoint(t1.m_dt, dealVolume);
	}
	showMethod = 3;
	// TODO: 在此添加控件通知处理程序代码
}


void CCandleChart::OnBnClickedLeftButton()
{
	if ((tBegin.GetHour() == 9) && (tBegin.GetMinute() == 15)) return;
	COleDateTimeSpan tsp(0, 0, -15, 0);
	tBegin += tsp;
	tEnd += tsp;
	pPriceDateAxis->SetMinMax(tBegin, tEnd);
	pVolumeDateAxis->SetMinMax(tBegin, tEnd);
	pAxis->SetAutomatic(true);
	// TODO: 在此添加控件通知处理程序代码
}


void CCandleChart::OnBnClickedRightButton()
{
	if ((tEnd.GetHour() == 15) && (tEnd.GetMinute() == 15)) return;
	COleDateTimeSpan tsp(0, 0, 15, 0);
	tBegin += tsp;
	tEnd += tsp;
	pPriceDateAxis->SetMinMax(tBegin, tEnd);
	pVolumeDateAxis->SetMinMax(tBegin, tEnd);
	pAxis->SetAutomatic(true);
	// TODO: 在此添加控件通知处理程序代码
}
