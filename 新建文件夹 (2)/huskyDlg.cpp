// ShowMd.cpp : 实现文件
//

#include "stdafx.h"
#include "huskyDlg.h"
#include "afxdialogex.h"



// CShowMd 对话框

IMPLEMENT_DYNAMIC(Chusky, CDialogEx)

Chusky::Chusky(CWnd* pParent /*=NULL*/)
	: CDialogEx(Chusky::IDD, pParent)
	, m_directRadioGroup(0)
	, m_makeRadioGroup(0)
	, m_num_of_topic_edit(0)
	, m_price_of_topic_edit(0)
	, pThread(NULL)
{
	m_pChart = NULL;
	m_pCandleChart = NULL;
	FILE *userSelectFile;
	while(!(userSelectFile = fopen("userName.txt", "r")));
	int user;
	fscanf(userSelectFile, "%d", &user);
	fscanf(userSelectFile, "%d", &needLine);
	fclose(userSelectFile);
	strcpy(BrokerID, "1004");
	if (user == 0){
		strcpy(UserID, "0193755301");
		strcpy(Password, "b82c5d504683007ac12478e703b19531");
	}
	if (user == 1){
		strcpy(UserID, "0193755302");
		strcpy(Password, "bad4d60aef1a060441dbbe47977d0dd1");
	}
	if (user == 2){
		strcpy(UserID, "0193755303");
		strcpy(Password, "f99a47bd147441718d35939f1c38a766");
	}

	pUserApi = CUstpFtdcMduserApi::CreateFtdcMduserApi();
	CLoginHandler tempLoginHandler(pUserApi);
	loginHandler = tempLoginHandler;
	loginHandler.getIns(numOfIns, contracts);
	loginHandler.getData(InsData);
	pUserApi -> RegisterSpi(&loginHandler);
	if (needLine)
		pUserApi -> SubscribeMarketDataTopic(100, USTP_TERT_RESTART);
	else
		pUserApi -> SubscribeMarketDataTopic(100, USTP_TERT_QUICK);
	pUserApi -> RegisterFront("tcp://117.185.125.4:17199");
	pUserApi -> Init();

	pTraderApi = CUstpFtdcTraderApi::CreateFtdcTraderApi();
	CTraderHandler tempTraderHandler(pTraderApi);
	traderHandler = tempTraderHandler;
	pTraderApi -> RegisterSpi(&traderHandler);
	pTraderApi -> SubscribePrivateTopic(USTP_TERT_RESTART);
	pTraderApi -> SubscribePublicTopic(USTP_TERT_RESTART);
	pTraderApi -> RegisterFront("tcp://117.185.125.4:17198");
	pTraderApi -> Init();

	CUstpFtdcQryInvestorAccountField qryInvestorAccount;	
	strcpy(qryInvestorAccount.BrokerID, BrokerID);
	strcpy(qryInvestorAccount.InvestorID, UserID);
	strcpy(qryInvestorAccount.UserID, Password);
	++g_UserOrderLocalID;
	int loopTime = 0;
	while (!traderHandler.getStatus()){
		++loopTime;
		if (loopTime > 50000000) break;
	}
	if (loopTime > 50000000){
		MessageBox(CString("登陆系统失败，请检查网络是否通畅及是否在系统开放时间"), CString("登陆失败"), MB_OK);
		OnClose();
	}
	setMoney();
	g_UserOrderLocalID = traderHandler.getUserOrderLocalID();
}

Chusky::~Chusky()
{

	CUstpFtdcReqUserLogoutField UserLogout;
	strcpy_s(UserLogout.BrokerID, BrokerID);
	strcpy_s(UserLogout.UserID, UserID);
	++g_UserOrderLocalID;
	int err = pUserApi->ReqUserLogout(&UserLogout, g_UserOrderLocalID);
	if (err != 0) MessageBox(CString("登出失败"), CString(""), MB_OK);
	pUserApi->Release();
	if (m_pChart != NULL) delete m_pChart;
	if (m_pCandleChart != NULL) delete m_pCandleChart;
}

void Chusky::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_MarketDataList);
	DDX_Radio(pDX, IDC_BUY_RADIO, m_directRadioGroup);
	DDX_Radio(pDX, IDC_OPEN_RADIO, m_makeRadioGroup);
	DDX_Control(pDX, IDC_NAME_OF_TOPIC_COMBO, m_name_of_topic_combobox);
	DDX_Text(pDX, IDC_NUM_OF_TOPIC_EDIT, m_num_of_topic_edit);
	DDX_Text(pDX, IDC_PRICE_OF_TOPIC_EDIT, m_price_of_topic_edit);
	DDX_Control(pDX, IDC_KIND_OF_TOPIC_COMBO, m_kind_of_topic_combobox);
	DDX_Control(pDX, IDC_WAIT_ORDER_LIST, m_waitOrderList);
	DDX_Control(pDX, IDC_DONE_ORDER_LIST, m_DoneOrderList);
}


BEGIN_MESSAGE_MAP(Chusky, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &Chusky::OnLvnItemchangedList2)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_NAME_OF_TOPIC_COMBO, &Chusky::OnCbnSelchangeNameOfTopicCombo)
	ON_BN_CLICKED(IDC_MAKE_SURE_BUTTON, &Chusky::OnBnClickedMakeSureButton)
	ON_BN_CLICKED(IDC_CANCEL_ORDER_BUTTON, &Chusky::OnBnClickedCancelOrderButton)
	ON_NOTIFY(NM_CLICK, IDC_WAIT_ORDER_LIST, &Chusky::OnNMClickWaitOrderList)
	ON_BN_CLICKED(IDC_TREND_BUTTON, &Chusky::OnBnClickedTrendButton)
	ON_BN_CLICKED(IDC_BUTTON, &Chusky::OnBnClickedButton)
	ON_BN_CLICKED(IDC_ORDERS_BUTTON, &Chusky::OnBnClickedOrdersButton)
	ON_BN_CLICKED(IDC_BUTTON1, &Chusky::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_POSITION_BUTTON, &Chusky::OnBnClickedPositionButton)
	ON_BN_CLICKED(IDC_STRATEGY_BUTTON, &Chusky::OnBnClickedStrategyButton)
END_MESSAGE_MAP()


// Chusky 消息处理程序


void Chusky::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void Chusky::changeShowList(CUstpFtdcDepthMarketDataField *pMarketData){
	int i = 0;
	while ((m_MarketDataList.GetItemText(i, 0)).Compare(CString(pMarketData -> InstrumentID))){
		++i;
		//TRACE("%d", i);
	}
	char changeTmp[100];
	sprintf(changeTmp, "%lf", pMarketData -> LastPrice);
	m_MarketDataList.SetItemText(i, 1, CString(changeTmp));
}

void Chusky::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1){
		UpdateInsData();
	}
	if (nIDEvent == 2){
		UpdateMoney();
		UpdateDoneOrder();
		UpdateWaitOrder();
		UpdatePosition();
		traderHandler.getPosition();
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL Chusky::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitMarketDataList();
	InitWaitOrderList();
	InitDoneOrderList();
	InitTrader();
	InitTimer();
	UpdateInsData();
	UpdateMoney();

	if (!needLine){
		GetDlgItem(IDC_TREND_BUTTON)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON)->EnableWindow(false);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void Chusky::OnCbnSelchangeNameOfTopicCombo()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Chusky::OnBnClickedMakeSureButton()
{
	CUstpFtdcInputOrderField order;

	strcpy(order.BrokerID, BrokerID);
	strcpy(order.ExchangeID, "CFFEX");
	strcpy(order.InvestorID, "01937553");
	strcpy(order.UserID, UserID);

	UpdateData(TRUE);

	//InstrumentID
	int nSelOfName = m_name_of_topic_combobox.GetCurSel();
	strcpy(order.InstrumentID, contracts[nSelOfName]);

	if (IsDlgButtonChecked(IDC_BUY_RADIO)) order.Direction = USTP_FTDC_D_Buy;
	if (IsDlgButtonChecked(IDC_SELL_RADIO)) order.Direction = USTP_FTDC_D_Sell;

	if (IsDlgButtonChecked(IDC_OPEN_RADIO)) order.OffsetFlag = '0';
	if (IsDlgButtonChecked(IDC_CLOSE_RADIO)) order.OffsetFlag = '1';

	order.HedgeFlag = '1';

	order.Volume = m_num_of_topic_edit;

	order.VolumeCondition = USTP_FTDC_VC_AV;

	order.StopPrice = 0;

	order.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;

	order.IsAutoSuspend = 0;

	++g_UserOrderLocalID;
	sprintf(order.UserOrderLocalID, "%012d", g_UserOrderLocalID);

	int nSelOfKind = m_kind_of_topic_combobox.GetCurSel();
	if (nSelOfKind == 0){//限价
		order.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		order.LimitPrice = m_price_of_topic_edit;
		order.TimeCondition = USTP_FTDC_TC_GFD;
	}

	if (nSelOfKind == 1){//市价
		order.OrderPriceType = USTP_FTDC_OPT_AnyPrice;
		order.TimeCondition = USTP_FTDC_TC_IOC;
		order.LimitPrice = 0;
	}

	if (nSelOfKind == 2){//FAK
		order.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		order.LimitPrice = m_price_of_topic_edit;
		order.TimeCondition = USTP_FTDC_TC_IOC;
		order.VolumeCondition = USTP_FTDC_VC_AV;
	}

	if (nSelOfKind == 3){//FOK
		order.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		order.LimitPrice = m_price_of_topic_edit;
		order.TimeCondition = USTP_FTDC_TC_IOC;
		order.VolumeCondition = USTP_FTDC_VC_CV;
	}

	m_price_of_topic_edit = 0;
	m_num_of_topic_edit = 0;

	int err = pTraderApi -> ReqOrderInsert(&order, g_UserOrderLocalID);
	// TODO: 在此添加控件通知处理程序代码
}

void Chusky::setMoney(){
	CUstpFtdcQryInvestorAccountField qryInvestorAccount;	
	strcpy(qryInvestorAccount.BrokerID, BrokerID);
	strcpy(qryInvestorAccount.InvestorID, "01937553");
	strcpy(qryInvestorAccount.UserID, UserID);
	++g_UserOrderLocalID;
	int err = pTraderApi -> ReqQryInvestorAccount(&qryInvestorAccount, g_UserOrderLocalID); 
	//if (err != 0) MessageBox(CString("%d", err), CString("fail"), MB_OK);
}

void Chusky::setPosition(char *InstrumentID){
	CUstpFtdcQryInvestorPositionField QryUserInvestorPosition;
	strcpy(QryUserInvestorPosition.BrokerID, BrokerID);
	strcpy(QryUserInvestorPosition.ExchangeID, "CFFEX");
	strcpy(QryUserInvestorPosition.InstrumentID, InstrumentID);
	strcpy(QryUserInvestorPosition.InvestorID, "01937553");
	strcpy(QryUserInvestorPosition.UserID, UserID);
	++g_UserOrderLocalID;
	pTraderApi -> ReqQryInvestorPosition(&QryUserInvestorPosition, g_UserOrderLocalID);
}

void Chusky::OnBnClickedCancelOrderButton()
{
	CUstpFtdcOrderActionField OrderAction;
	strcpy(OrderAction.ExchangeID, "CFFEX");
	strcpy(OrderAction.BrokerID, BrokerID);
	strcpy(OrderAction.InvestorID, "01937553");
	strcpy(OrderAction.UserID, UserID);
	++g_UserOrderLocalID;
	sprintf(OrderAction.UserOrderActionLocalID, "%012d", g_UserOrderLocalID);
	sprintf(OrderAction.UserOrderLocalID, cancelUserOrderLocalID);
	sprintf(OrderAction.OrderSysID, "");
	OrderAction.ActionFlag = '0';
	OrderAction.LimitPrice = 0;
	OrderAction.VolumeChange = 0;
	int err = pTraderApi->ReqOrderAction(&OrderAction, g_UserOrderLocalID);
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(CString(OrderAction.UserOrderLocalID), CString("撤单"), MB_OK);

}


void Chusky::OnNMClickWaitOrderList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nSelectLine = m_waitOrderList.GetNextItem(-1, LVNI_SELECTED);
	CString cancelLocalOrderID = m_waitOrderList.GetItemText(nSelectLine, 5);
	int tempUserOrderLocalID = _ttoi(cancelLocalOrderID);
	sprintf(cancelUserOrderLocalID, "%012d", tempUserOrderLocalID);
	*pResult = 0;
}



void Chusky::OnBnClickedTrendButton()
{
	int nChoose = m_MarketDataList.GetNextItem(-1, LVNI_SELECTED);
	if (nChoose == -1){
		MessageBox(CString("请先在左边选择合约"), CString("打开错误"), MB_OK | MB_ICONSTOP);
		return;
	}
	int lenOfCString = WideCharToMultiByte(CP_ACP,0,m_MarketDataList.GetItemText(nChoose, 0),-1,NULL,0,NULL,NULL);
	char *InstrumentID = new char[lenOfCString + 1];
	WideCharToMultiByte(CP_ACP, 0, m_MarketDataList.GetItemText(nChoose, 0), -1,InstrumentID, lenOfCString, NULL, NULL);

	if (m_pChart != NULL){
		delete m_pChart;
	}
	m_pChart = new CChart(InstrumentID);
	m_pChart->Create(IDD_CHART, this);
	m_pChart->ShowWindow(SW_SHOW);

	delete[] InstrumentID;
	// TODO: 在此添加控件通知处理程序代码
}


void Chusky::OnBnClickedButton()
{
	int nChoose = m_MarketDataList.GetNextItem(-1, LVNI_SELECTED);
	if (nChoose == -1){
		MessageBox(CString("请先在左边选择合约"), CString("打开错误"), MB_OK | MB_ICONSTOP);
		return;
	}

	int lenOfCString = WideCharToMultiByte(CP_ACP,0,m_MarketDataList.GetItemText(nChoose, 0),-1,NULL,0,NULL,NULL);
	char *InstrumentID = new char[lenOfCString + 1];
	WideCharToMultiByte(CP_ACP, 0, m_MarketDataList.GetItemText(nChoose, 0), -1,InstrumentID, lenOfCString, NULL, NULL);

	if (m_pCandleChart != NULL){
		delete m_pCandleChart;
	}
	m_pCandleChart = new CCandleChart(InstrumentID);
	m_pCandleChart->Create(IDD_CANDLE_CHART, this);
	m_pCandleChart->ShowWindow(SW_SHOW);

	delete[] InstrumentID;
	// TODO: 在此添加控件通知处理程序代码
}


void Chusky::OnBnClickedOrdersButton()
{
	CUstpFtdcInputOrderField order;

	strcpy(order.BrokerID, BrokerID);
	strcpy(order.ExchangeID, "CFFEX");
	strcpy(order.InvestorID, "01937553");
	strcpy(order.UserID, UserID);

	UpdateData(TRUE);
	strcpy(order.InstrumentID, "IF1510");
	order.HedgeFlag = '1';
	order.Volume = 1;
	order.VolumeCondition = USTP_FTDC_VC_AV;
	order.StopPrice = 0;
	order.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;
	order.IsAutoSuspend = 0;
	order.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
	order.LimitPrice = m_price_of_topic_edit;
	order.TimeCondition = USTP_FTDC_TC_GFD;
	traderHandler.setNotify(50);
	for (int i = 0; i < 26; ++i){
		++g_UserOrderLocalID;
		order.Direction = USTP_FTDC_D_Buy;
		order.OffsetFlag = '0';
		order.LimitPrice += 30;
		sprintf(order.UserOrderLocalID, "%012d", g_UserOrderLocalID);
		int err = 1;
		while (err){
			err = pTraderApi -> ReqOrderInsert(&order, g_UserOrderLocalID);
		}
		++g_UserOrderLocalID;
		order.Direction = USTP_FTDC_D_Sell;
		order.OffsetFlag = '1';
		order.LimitPrice -= 30;
		sprintf(order.UserOrderLocalID, "%012d", g_UserOrderLocalID);
		err = 1;
		while (err){
			err = pTraderApi -> ReqOrderInsert(&order, g_UserOrderLocalID);
		}
	}
	// TODO: 在此添加控件通知处理程序代码
}


void Chusky::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Chusky::OnBnClickedPositionButton()
{
	CWnd *m_positionWnd;
	CPosition positionDlg;
	m_positionWnd = &positionDlg;
	INT_PTR nResponsePosition = positionDlg.DoModal();
	// TODO: 在此添加控件通知处理程序代码
}

SendToThread para;

UINT mainloop(LPVOID lpPraram){
	SendToThread *pPrara = (SendToThread*) lpPraram;
	CStrategy strategyHandler(pPrara);
	strategyHandler.InitData();
	while (1){
		strategyHandler.UpdateData();
		strategyHandler.Judgement();
	}
	return 0;
}


void Chusky::OnBnClickedStrategyButton()
{
	static int mark = 1;
	if (mark == 1){
		strcpy(para.BrokerID, BrokerID);
		//para.InsData = InsData;
		loginHandler.getNowMinute(para.pMarketData);
		strcpy(para.ExchangeID, "CFFEX");
		strcpy(para.InvestorID, "01937553");
		strcpy(para.UserID, UserID);
		para.pTraderApi = pTraderApi;
		para.traderHandler = &traderHandler;
		para.UserOrderLocalID = &g_UserOrderLocalID;
		pThread = AfxBeginThread(mainloop, &para);


		mark = 0;
		GetDlgItem(IDC_STRATEGY_BUTTON)->SetWindowText(CString("关闭策略"));
	}
	else{
		mark = 1;
		GetDlgItem(IDC_STRATEGY_BUTTON)->SetWindowText(CString("执行策略"));
	}
	// TODO: 在此添加控件通知处理程序代码
}
