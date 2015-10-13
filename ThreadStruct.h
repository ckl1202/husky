#pragma once
#include "TraderHandler.h"


struct SendToThread{
	MinuteMarketData *pMarketData;
	CUstpFtdcTraderApi *pTraderApi;
	CTraderHandler *traderHandler;
	InstrumentMsg *insData;
	int *mark;
	int * UserOrderLocalID;
	char BrokerID[10];
	char UserID[20];
	char ExchangeID[10];
	char InvestorID[10];
};