#include "stdafx.h"
#include "huskyDlg.h"
#include "afxdialogex.h"

void Chusky::InitMarketDataList(){
	m_MarketDataList.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_MarketDataList.InsertColumn(0, CString("合约"), LVCFMT_LEFT, 100);
	m_MarketDataList.InsertColumn(1, CString("最新价"), LVCFMT_LEFT, 100);
	m_MarketDataList.InsertColumn(2, CString("涨跌幅"), LVCFMT_LEFT, 100);
	m_MarketDataList.InsertColumn(3, CString("申买价一"), LVCFMT_LEFT, 100);
	m_MarketDataList.InsertColumn(4, CString("申买量一"), LVCFMT_LEFT, 100);
	m_MarketDataList.InsertColumn(5, CString("申卖价一"), LVCFMT_LEFT, 100);
	m_MarketDataList.InsertColumn(6, CString("申卖量一"), LVCFMT_LEFT, 100);
	m_MarketDataList.InsertColumn(7, CString("昨结算"), LVCFMT_LEFT, 100);
	m_MarketDataList.InsertColumn(8, CString("昨持仓"), LVCFMT_LEFT, 100);
	m_MarketDataList.InsertColumn(9, CString("最后更新时间"), LVCFMT_LEFT, 130);
	for (int i = 0; i < numOfIns; ++i) m_MarketDataList.InsertItem(i, CString(contracts[i]));
}
void Chusky::InitWaitOrderList(){
	m_waitOrderList.SetExtendedStyle(m_waitOrderList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_waitOrderList.InsertColumn(0, CString("合约名称"), LVCFMT_LEFT, 120);
	m_waitOrderList.InsertColumn(1, CString("买卖"), LVCFMT_LEFT, 120);
	m_waitOrderList.InsertColumn(2, CString("开平"), LVCFMT_LEFT, 120);
	m_waitOrderList.InsertColumn(3, CString("价格"), LVCFMT_LEFT, 120);
	m_waitOrderList.InsertColumn(4, CString("剩余手数"), LVCFMT_LEFT, 120);
	m_waitOrderList.InsertColumn(5, CString("本地报单编号"), LVCFMT_LEFT, 120);
}
void Chusky::InitDoneOrderList(){
	m_DoneOrderList.InsertColumn(0, CString("合约名称"), LVCFMT_LEFT, 125);
	m_DoneOrderList.InsertColumn(1, CString("买卖"), LVCFMT_LEFT, 125);
	m_DoneOrderList.InsertColumn(2, CString("开平"), LVCFMT_LEFT, 125);
	m_DoneOrderList.InsertColumn(3, CString("价格"), LVCFMT_LEFT, 125);
	m_DoneOrderList.InsertColumn(4, CString("手数"), LVCFMT_LEFT, 125);
	m_DoneOrderList.InsertColumn(5, CString("成交时间"), LVCFMT_LEFT, 125);
}
void Chusky::InitTrader(){
	m_kind_of_topic_combobox.AddString(CString("限价单"));
	m_kind_of_topic_combobox.AddString(CString("市价单"));
	m_kind_of_topic_combobox.AddString(CString("FAK"));
	m_kind_of_topic_combobox.AddString(CString("FOK"));
	m_kind_of_topic_combobox.SetCurSel(1);
	for (int i = 0; i < numOfIns; ++i) {
		m_name_of_topic_combobox.AddString(CString(contracts[i]));
		m_firstInsCombobox.AddString(CString(contracts[i]));
		m_secondInsCombobox.AddString(CString(contracts[i]));
	}
}
void Chusky::InitTimer(){
	SetTimer(1, 100, NULL);
	SetTimer(2, 2000, NULL);
}
void Chusky::UpdateInsData(){
	char strTemp[20];
	int i;
	int nChoose = m_MarketDataList.GetNextItem(-1, LVNI_SELECTED);
	int nFirstInsChoose = m_firstInsCombobox.GetCurSel();
	if (nFirstInsChoose < 0) nFirstInsChoose = 0;
	int nSecondInsChoose = m_secondInsCombobox.GetCurSel();
	if (nSecondInsChoose < 0) nSecondInsChoose = 0;

	for (i = 0; i < numOfIns; ++i){
		//最新价
		if (!InsData[i].modified) continue;
		InsData[i].modified = false;
		sprintf(strTemp, "%.1lf", InsData[i].LastPrice);
		m_MarketDataList.SetItemText(i, 1, CString(strTemp));
		//涨幅
		sprintf(strTemp, "%.2lf%%", InsData[i].rate);
		m_MarketDataList.SetItemText(i, 2, CString(strTemp));
		//买一价
		sprintf(strTemp, "%.1lf", InsData[i].BidPrice1);
		m_MarketDataList.SetItemText(i, 3, CString(strTemp));
		if (i == nChoose) SetDlgItemText(IDC_BUY_PRICE_EDIT, CString(strTemp));
		if (i == nFirstInsChoose) SetDlgItemText(IDC_INS_BUY_EDIT1, CString(strTemp));
		if (i == nSecondInsChoose) SetDlgItemText(IDC_INS_BUY_EDIT2, CString(strTemp));
		//买一量
		sprintf(strTemp, "%d", InsData[i].BidVolume1);
		m_MarketDataList.SetItemText(i, 4, CString(strTemp));
		if (i == nChoose) SetDlgItemText(IDC_BUY_VOLUME_EDIT, CString(strTemp));
		//卖一价
		sprintf(strTemp, "%.1lf", InsData[i].AskPrice1);
		m_MarketDataList.SetItemText(i, 5, CString(strTemp));
		if (i == nChoose) SetDlgItemText(IDC_SELL_PRICE_EDIT, CString(strTemp));
		if (i == nFirstInsChoose) SetDlgItemText(IDC_INS_SELL_EDIT1, CString(strTemp));
		if (i == nSecondInsChoose) SetDlgItemText(IDC_INS_SELL_EDIT2, CString(strTemp));


		if ((i == nSecondInsChoose) || (i == nFirstInsChoose)){
			sprintf(strTemp, "%.1lf", InsData[nFirstInsChoose].BidPrice1 - InsData[nSecondInsChoose].AskPrice1);
			SetDlgItemText(IDC_INS_SELL_DELTA_EDIT, CString(strTemp));
			sprintf(strTemp, "%.1lf", InsData[nFirstInsChoose].AskPrice1 - InsData[nSecondInsChoose].BidPrice1);
			SetDlgItemText(IDC_INS_BUY_DELTA_EDIT, CString(strTemp));
		}
		//卖一量
		sprintf(strTemp, "%d", InsData[i].AskVolume1);
		m_MarketDataList.SetItemText(i, 6, CString(strTemp));
		if (i == nChoose) SetDlgItemText(IDC_SELL_VOLUME_EDIT, CString(strTemp));
		//昨结算
		sprintf(strTemp, "%.1lf", InsData[i].PreSettlementPrice);
		m_MarketDataList.SetItemText(i, 7, CString(strTemp));
		//昨持仓
		sprintf(strTemp, "%d", InsData[i].PreOpenInterest);
		m_MarketDataList.SetItemText(i, 8, CString(strTemp));
		//更新时间
		m_MarketDataList.SetItemText(i, 9, CString(InsData[i].UpdateTime));
	}
}
void Chusky::UpdateMoney(){
	setMoney();
	FILE *m_moneyFile;
	m_moneyFile = fopen("money.txt", "r");
	double tmpMargin, tmpAvailable;
	fscanf(m_moneyFile, "%lf\n%lf", &tmpMargin, &tmpAvailable);
	char tempMoney[20];
	sprintf(tempMoney, "%.2lf", tmpMargin);
	SetDlgItemText(IDC_MARGIN_EDIT, CString(tempMoney));
	sprintf(tempMoney, "%.2lf", tmpAvailable);
	SetDlgItemText(IDC_AVAILABLE_EDIT, CString(tempMoney));
	fclose(m_moneyFile);
}
void Chusky::UpdateDoneOrder(){
	traderHandler.getDoneOrder();
	FILE *doneOrderFile;
	doneOrderFile = fopen("doneOrder.txt", "r");
	doneOrder aOrder;
	while (fscanf(doneOrderFile, "%s %c %c %lf %d %s\n",
		&aOrder.InstrumentID,
		&aOrder.Direction,
		&aOrder.OffsetFlag,
		&aOrder.TraderPrice,
		&aOrder.TraderVolumn,
		&aOrder.TradeTime) != EOF){
		
		m_DoneOrderList.InsertItem(0, CString(aOrder.InstrumentID));
		if (aOrder.Direction == USTP_FTDC_D_Buy) m_DoneOrderList.SetItemText(0, 1, CString("买"));
		else m_DoneOrderList.SetItemText(0, 1, CString("卖"));
		if (aOrder.OffsetFlag == USTP_FTDC_OF_Open) m_DoneOrderList.SetItemText(0, 2, CString("开"));
		else m_DoneOrderList.SetItemText(0, 2, CString("平"));
		char tempDigitalToString[20];
		sprintf(tempDigitalToString, "%lf", aOrder.TraderPrice);
		m_DoneOrderList.SetItemText(0, 3, CString(tempDigitalToString));
		sprintf(tempDigitalToString, "%d", aOrder.TraderVolumn);
		m_DoneOrderList.SetItemText(0, 4, CString(tempDigitalToString));
		m_DoneOrderList.SetItemText(0, 5, CString(aOrder.TradeTime));
	}
	fclose(doneOrderFile);
}
void Chusky::UpdateWaitOrder(){
	traderHandler.getWaitOrder();
	FILE *waitOrderFile;
	waitOrderFile = fopen("waitOrder.txt", "r");
	waitOrder bOrder;
	while (fscanf(waitOrderFile, "%s %c %c %lf %d %s\n",
		&bOrder.InstrumentID,
		&bOrder.Direction,
		&bOrder.OffsetFlag,
		&bOrder.LimitPrice,
		&bOrder.Volumn,
		&bOrder.UserOrderLocalID) != EOF){

		int i = 0;
		CString oldLocalID = m_waitOrderList.GetItemText(0, 5), newLocalID = CString(bOrder.UserOrderLocalID);
		while (oldLocalID != CString("")){
			if (oldLocalID == newLocalID) break;
			++i;
			oldLocalID = m_waitOrderList.GetItemText(i, 5);
		}
		if (bOrder.Volumn == 0){
			m_waitOrderList.DeleteItem(i);
			continue;
		}
		if (oldLocalID == CString("")) {
			i = 0;
			m_waitOrderList.InsertItem(i, CString(bOrder.InstrumentID));
		}
		if (bOrder.Direction == USTP_FTDC_D_Buy) m_waitOrderList.SetItemText(i, 1, CString("买"));
		else m_waitOrderList.SetItemText(i, 1, CString("卖"));
		if (bOrder.OffsetFlag == USTP_FTDC_OF_Open) m_waitOrderList.SetItemText(i, 2, CString("开"));
		else m_waitOrderList.SetItemText(i, 2, CString("平"));
		char tempDigitalToString[20];
		sprintf(tempDigitalToString, "%lf", bOrder.LimitPrice);
		m_waitOrderList.SetItemText(i, 3, CString(tempDigitalToString));
		sprintf(tempDigitalToString, "%d", bOrder.Volumn);
		m_waitOrderList.SetItemText(i, 4, CString(tempDigitalToString));
		m_waitOrderList.SetItemText(i, 5, CString(bOrder.UserOrderLocalID));
	}
	fclose(waitOrderFile);
}
void Chusky::UpdatePosition(){
	CUstpFtdcQryInvestorPositionField userInvestorPosition;
	strcpy_s(userInvestorPosition.BrokerID, BrokerID);
	strcpy_s(userInvestorPosition.ExchangeID, "CFFEX");
	strcpy_s(userInvestorPosition.InvestorID, "01937586");
	strcpy_s(userInvestorPosition.UserID, UserID);
	for (int i = 0; i < numOfIns; ++i){
		strcpy_s(userInvestorPosition.InstrumentID, contracts[i]);
		++g_UserOrderLocalID;
		pTraderApi->ReqQryInvestorPosition(&userInvestorPosition, g_UserOrderLocalID);
	}
}