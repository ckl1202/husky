#include "ChartCtrl\ChartCtrl.h"
#include "ChartCtrl\ChartTitle.h"
#include "ChartCtrl\ChartLineSerie.h"
#include "ChartCtrl\ChartDateTimeAxis.h"
#include "ChartCtrl\ChartBarSerie.h"
#include "DataStruct.h"
#pragma once


// CChart �Ի���

class CChart : public CDialogEx
{
	DECLARE_DYNAMIC(CChart)

private:
	char InstrumentID[20];
	FILE* dataFile;
	int nNumOfData;
	double totalPrice;
	int preVolume;
	MinuteMarketData Minute;
	CChartAxis *pAxis;
	CChartDateTimeAxis *pDateAxis;

	CChartLineSerie *pLineSerie, *pAverSerie;
	CChartLineSerie *pHaveSerie;
	CChartBarSerie *pDealSerie;
public:
	CChart(char *Instrument, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChart();

// �Ի�������
	enum { IDD = IDD_CHART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CChartCtrl m_priceChart;
	CChartCtrl m_volumeChart;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
