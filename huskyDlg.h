#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "UstpFtdcMduserApi.h"
#include "USTPFtdcTraderApi.h"
#include "loginHandler.h"
#include "TraderHandler.h"
#include "afxwin.h"
#include <math.h>
#include "DataStruct.h"
#include "ThreadStruct.h"
#include "Chart.h"
#include "CandleChart.h"
#include "Position.h"
#include "Strategy.h"


// Chusky 对话框

class Chusky : public CDialogEx
{
	DECLARE_DYNAMIC(Chusky)

private:
	CUstpFtdcMduserApi *pUserApi;
	CUstpFtdcTraderApi *pTraderApi;
	CLoginHandler loginHandler;
	CTraderHandler traderHandler;
	int g_UserOrderLocalID;
	int g_nRequestID;

	char BrokerID[10], UserID[20], Password[50];

	int numOfIns;
	char** contracts;
	InstrumentMsg* InsData;

	char temp[20];
	CChart *m_pChart;
	CCandleChart *m_pCandleChart;

	int needLine;

public:
	Chusky(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Chusky();

// 对话框数据
	enum { IDD = IDD_HUSKY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_MarketDataList;
	//CRect m_ShowMdRect;
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
//	virtual BOOL OnInitDialog();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	int m_directRadioGroup;
	int m_makeRadioGroup;
	void changeShowList(CUstpFtdcDepthMarketDataField *pMarketData);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	CComboBox m_name_of_topic_combobox;
	afx_msg void OnCbnSelchangeNameOfTopicCombo();
	int m_num_of_topic_edit;
	double m_price_of_topic_edit;
	CComboBox m_kind_of_topic_combobox;
	afx_msg void OnBnClickedMakeSureButton();
	void setMoney();
	void setPosition(char *InstrumentID);
	CListCtrl m_waitOrderList;
	CListCtrl m_DoneOrderList;
	afx_msg void OnBnClickedCancelOrderButton();
	char cancelUserOrderLocalID[15];
	afx_msg void OnNMClickWaitOrderList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedTrendButton();
	afx_msg void OnBnClickedButton();
	afx_msg void OnBnClickedOrdersButton();

private:
	void InitMarketDataList();
	void InitWaitOrderList();
	void InitDoneOrderList();
	void InitTrader();
	void InitTimer();
	void UpdateInsData();
	void UpdateMoney();
	void UpdateDoneOrder();
	void UpdateWaitOrder();
	void UpdatePosition();
	
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedPositionButton();
	afx_msg void OnBnClickedStrategyButton();
protected:
	CWinThread* pThread;
public:
	CComboBox m_firstInsCombobox;
	CComboBox m_secondInsCombobox;
	CEdit m_firstSellEdit;
	afx_msg void OnEnChangeInsBuyEdit1();
	CEdit m_firstBuyEdit;
	CEdit m_secondSellEdit;
	CEdit m_secondBuyEdit;
	CEdit m_sellDeltaEdit;
	CEdit m_buyDeltaEdit;
	afx_msg void OnBnClickedSellDeltaButton();
	afx_msg void OnBnClickedBuyDeltaButton();
};

