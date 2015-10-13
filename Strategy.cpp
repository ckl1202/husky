#include "StdAfx.h"
#include "Strategy.h"

CStrategy::CStrategy(SendToThread *msg) : account(msg)
{
}

void CStrategy::InitData(){
	FILE *paraFile = fopen("para/para.txt", "r");
	fscanf(paraFile, "%s\n%d %d %d %d %d",&InstrumentID,  
		&orderOfIns, &kindOfK, &numOfK, &maxPosition, &minuteGap);
	tailOfData = 1;

	direction = -1;

	char temp[100];
	strcpy(temp, "data/");
	strcat(temp, InstrumentID);
	strcat(temp, "Y.txt");

	dataFile = fopen(temp, "a+");
	fclose(dataFile);
	dataFile = fopen(temp, "r");

	while (data[tailOfData].GetFromFile(dataFile)){
		tailOfData++;
	}
	fclose(dataFile);
	//上海凯纳金融建模大赛
	//342914924
	strcpy(temp, "data/");
	strcat(temp, InstrumentID);
	strcat(temp, ".txt");
	dataFile = fopen(temp, "r");
	UpdateData();
}

void CStrategy::UpdateData(){
	while (data[tailOfData].GetFromFile(dataFile)){
		tailOfData++;
	}
}

void CStrategy::Judgement(){
	data[tailOfData] = (account->pMarketData)[orderOfIns];
	int currentPosition = tailOfData;
	while ((currentPosition % kindOfK) != 0){
		++currentPosition;
		data[currentPosition] = (account->pMarketData)[orderOfIns];
	}
	double totalE = 0;
	for (int i = 0; i < numOfK; ++i){
		double open, close, high, low;
		int volume2, volume1;
		for (int j = 0; j < kindOfK; ++j){
			if (j == 0){
				close = data[currentPosition].closePrice;
				high = data[currentPosition].highPrice;
				low = data[currentPosition].lowPrice;
				volume2 = data[currentPosition].dealVolume;
			}
			if (j == (kindOfK - 1)){
				open = data[currentPosition].openPrice;
				volume1 = data[currentPosition].dealVolume;
			}
			if (data[currentPosition].highPrice > high) high = data[currentPosition].highPrice;
			if (data[currentPosition].lowPrice < low) low = data[currentPosition].lowPrice;
			--currentPosition;
		}
		double e = 0;
		if (high == low) e = 0;
		else e = ((open - close) / (high - low)) * (volume2 - volume1);
		totalE += e;
	}
	if (direction == -1){
		lastTime = COleDateTime::GetCurrentTime();			
		if (totalE > 0){
			int rest = maxPosition;
			direction = BUY;
			for (int stage = 0; stage < 4; ++stage){
				int ID = MakeOrder(USTP_FTDC_D_Buy, USTP_FTDC_OF_Open, stage, rest);
				(account->traderHandler)->regStrategy(1, ID);
				Sleep(10000);
				if (rest != 0) rest = (account->traderHandler)->getStrategy(1);
				if (rest != 0) cancelOrder(ID);
			}
		}
		else {//totalE < 0)
			int rest = maxPosition;
			direction = SELL;
			for (int stage = 0; stage < 4; ++stage){
				int ID = MakeOrder(USTP_FTDC_D_Sell, USTP_FTDC_OF_Open, stage, rest);
				(account->traderHandler)->regStrategy(1, ID);
				Sleep(10000);
				if (rest != 0) rest = (account->traderHandler)->getStrategy(1);
				if (rest != 0) cancelOrder(ID);
			}
		}
	}
	else{
		COleDateTime nowTime(COleDateTime::GetCurrentTime());
		int totalSecond = 0;
		totalSecond += (nowTime.GetHour() - lastTime.GetHour()) * 3600;
		totalSecond += (nowTime.GetMinute() - lastTime.GetMinute()) * 60;
		totalSecond += (nowTime.GetSecond() - lastTime.GetSecond());
		if (totalSecond < 60 * minuteGap) return; 
		if (totalE > 0){
			if (direction == BUY) return;
			direction = BUY;
			int rest1 = maxPosition;
			int rest2 = maxPosition;
			int ID1, ID2;
			for (int stage = 0; stage < 4; ++stage){
				if (rest1 != 0){
					ID1 = MakeOrder(USTP_FTDC_D_Buy, USTP_FTDC_OF_Close, stage, rest1);
					(account->traderHandler)->regStrategy(1, ID1);
				}
				if (rest2 != 0){
					ID2 = MakeOrder(USTP_FTDC_D_Buy, USTP_FTDC_OF_Open, stage, rest2);
					(account->traderHandler)->regStrategy(2, ID2);
				}
				Sleep(10000);
				if (rest1 != 0) rest1 = (account->traderHandler)->getStrategy(1);
				if (rest1 != 0) cancelOrder(ID1);
				if (rest2 != 0) rest2 = (account->traderHandler)->getStrategy(2);
				if (rest2 != 0) cancelOrder(ID2);
			}
		}
		else{
			if (direction == SELL) return;
			direction = SELL;
			int rest1 = maxPosition;
			int rest2 = maxPosition;
			int ID1, ID2;
			for (int stage = 0; stage < 4; ++stage){
				if (rest1 != 0){
					ID1 = MakeOrder(USTP_FTDC_D_Sell, USTP_FTDC_OF_Close, stage, rest1);
					(account->traderHandler)->regStrategy(1, ID1);
				}
				if (rest2 != 0){
					ID2 = MakeOrder(USTP_FTDC_D_Sell, USTP_FTDC_OF_Open, stage, rest2);
					(account->traderHandler)->regStrategy(2, ID2);
				}
				Sleep(10000);
				if (rest1 != 0) rest1 = (account->traderHandler)->getStrategy(1);
				if (rest1 != 0) cancelOrder(ID1);
				if (rest2 != 0) rest2 = (account->traderHandler)->getStrategy(2);
				if (rest2 != 0) cancelOrder(ID2);
			}
		}
	}
}

int CStrategy::MakeOrder(char direction, char offsetflag, int stage, int position){
	CUstpFtdcInputOrderField order;

	strcpy(order.BrokerID, account->BrokerID);
	strcpy(order.ExchangeID, "CFFEX");
	strcpy(order.InvestorID, "01937586");
	strcpy(order.UserID, account->UserID);
	strcpy(order.InstrumentID, InstrumentID);
	order.Direction = direction;
	order.OffsetFlag = offsetflag;
	order.HedgeFlag = '1';
	order.Volume = position;
	order.VolumeCondition = USTP_FTDC_VC_AV;
	order.StopPrice = 0;
	order.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;
	order.IsAutoSuspend = 0;

	int id = *account->UserOrderLocalID;
	++id;
	sprintf(order.UserOrderLocalID, "%012d", id);
	*account->UserOrderLocalID = id;

	if (stage == 0){
		order.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		order.LimitPrice = data[tailOfData].closePrice;
		order.TimeCondition = USTP_FTDC_TC_GFD;
	}
	if (stage == 1){
		order.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		order.LimitPrice = (account->insData)[orderOfIns].BidPrice1;
		order.TimeCondition = USTP_FTDC_TC_GFD;
	}
	if (stage == 2){
		order.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		order.TimeCondition = USTP_FTDC_TC_GFD;
		double middle = (account->insData)[orderOfIns].AskPrice1 + (account->insData)[orderOfIns].BidPrice1;
		middle /= 2;
		middle *= 5;
		int nMiddle = (int) middle;
		middle = nMiddle / 5.0;
		order.LimitPrice = middle;
	}
	if (stage == 3){
		order.OrderPriceType = USTP_FTDC_OPT_AnyPrice;
		order.TimeCondition = USTP_FTDC_TC_IOC;
		order.LimitPrice = 0;
	}

	account->pTraderApi -> ReqOrderInsert(&order, id);

	return id;
}

void CStrategy::cancelOrder(int id){
	CUstpFtdcOrderActionField OrderAction;
	strcpy(OrderAction.ExchangeID, "CFFEX");
	strcpy(OrderAction.BrokerID, account->BrokerID);
	strcpy(OrderAction.InvestorID, "01937586");
	strcpy(OrderAction.UserID, account->UserID);
	int g_UserOrderLocalID = *account->UserOrderLocalID;
	++g_UserOrderLocalID;
	*account->UserOrderLocalID = g_UserOrderLocalID;
	sprintf(OrderAction.UserOrderActionLocalID, "%012d", g_UserOrderLocalID);
	sprintf(OrderAction.UserOrderLocalID, "%012d", id);
	sprintf(OrderAction.OrderSysID, "");
	OrderAction.ActionFlag = '0';
	OrderAction.LimitPrice = 0;
	OrderAction.VolumeChange = 0;
	int err = (account->pTraderApi)->ReqOrderAction(&OrderAction, g_UserOrderLocalID);
}

CStrategy::~CStrategy(void)
{
	fclose(dataFile);
}
