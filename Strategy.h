#pragma once
#include "DataStruct.h"
#include "USTPFtdcTraderApi.h"
#include "ThreadStruct.h"

const int SELL = 0;
const int BUY = 1;

class CStrategy
{
	MinuteMarketData data[550];
	int orderOfIns, numOfK, kindOfK, maxPosition, minuteGap;
	char InstrumentID[20];
	int tailOfData;

	COleDateTime lastTime;

	SendToThread *account;
	FILE *dataFile;

	int direction;
public:
	CStrategy(SendToThread *msg);

	void InitData();
	void UpdateData();
	void Judgement();
	int MakeOrder(char direction, char offsetflag, int stage, int position);
	void cancelOrder(int id);

	~CStrategy(void);
};

