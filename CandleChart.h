#include "ChartCtrl\ChartCtrl.h"
#include "ChartCtrl\ChartTitle.h"
#include "ChartCtrl\ChartLineSerie.h"
#include "ChartCtrl\ChartDateTimeAxis.h"
#include "ChartCtrl\ChartBarSerie.h"
#include "ChartCtrl\ChartCandlestickSerie.h"
#include "DataStruct.h"
#pragma once


// CCandleChart �Ի���

class CCandleChart : public CDialogEx
{
	DECLARE_DYNAMIC(CCandleChart)
private:
	char InstrumentID[20];
	FILE *dataFile;
	int preVolume;
	MinuteMarketData Minute;

	CChartAxis *pAxis;
	CChartDateTimeAxis *pPriceDateAxis, *pVolumeDateAxis;

	CChartCandlestickSerie *pPriceSerie;
	CChartBarSerie *pDealSerie;
	CChartLineSerie *pHaveSerie;

	int showMethod;
	int numOfData;

	double open, close, high, low;

	COleDateTime tBegin;
	COleDateTime tEnd;

public:
	CCandleChart(char * Instrument, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCandleChart();

// �Ի�������
	enum { IDD = IDD_CANDLE_CHART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CChartCtrl m_priceChart;
	CChartCtrl m_volumeChart;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedLeftButton();
	afx_msg void OnBnClickedRightButton();
};
