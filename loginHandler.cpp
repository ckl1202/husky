#include "StdAfx.h"
#include "loginHandler.h"

void CLoginHandler::OnFrontConnected(){
	CUstpFtdcReqUserLoginField reqUserLogin;
	//交易日
	strcpy(reqUserLogin.TradingDay, m_pUserApi -> GetTradingDay());
	//会员ID
	strcpy(reqUserLogin.BrokerID, BrokerID);
	//用户ID
	strcpy(reqUserLogin.UserID, UserID);
	//密码
	strcpy(reqUserLogin.Password, Password);

	//登录
	m_pUserApi -> ReqUserLogin(&reqUserLogin, 0);
}

void CLoginHandler::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, 
									CUstpFtdcRspInfoField *pRspInfo,
									int nRequestID, 
									bool bIsLast){
	if (pRspInfo -> ErrorID != 0){
		MessageBox(NULL, CString(pRspInfo->ErrorMsg), CString("登陆失败"), MB_OK);
	}
	//合约信息输入

	//注册合约
	m_pUserApi -> SubMarketData(contracts, numOfIns);
}

void CLoginHandler::OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pMarketData){
	
	int nNoOfIns;

	for (int i = 0; i < numOfIns; ++i){
		bool check = true;
		int len = strlen(contracts[i]);
		for (int j = 0; j < len; ++j) if (pMarketData->InstrumentID[j] != contracts[i][j]){
			check = false;
			break;
		}
		if (check){
			nNoOfIns = i;
			break;
		}
	}

	if (pMarketData->AskPrice1 < 10000) Instrument[nNoOfIns].AskPrice1 = pMarketData->AskPrice1;
		//Instrument[nNoOfIns].AskPrice1 = pMarketData->HighestPrice;
	if (pMarketData->AskVolume1 < 100000) Instrument[nNoOfIns].AskVolume1 = pMarketData->AskVolume1;
		//Instrument[nNoOfIns].AskVolume1 = pMarketData->Volume;
	if (pMarketData->BidPrice1 <10000) Instrument[nNoOfIns].BidPrice1 = pMarketData->BidPrice1;
		//Instrument[nNoOfIns].BidPrice1 = pMarketData->LowestPrice;
	if (pMarketData->BidVolume1 < 100000) Instrument[nNoOfIns].BidVolume1 = pMarketData->BidVolume1;
		//Instrument[nNoOfIns].BidVolume1 = pMarketData->OpenInterest;
	if (pMarketData->LastPrice < 10000) Instrument[nNoOfIns].LastPrice = pMarketData->LastPrice;
	if (pMarketData->PreOpenInterest < 1000000)Instrument[nNoOfIns].PreOpenInterest = pMarketData->PreOpenInterest;
	if (pMarketData->PreSettlementPrice < 10000) Instrument[nNoOfIns].PreSettlementPrice = pMarketData->PreSettlementPrice;
		//Instrument[nNoOfIns].PreSettlementPrice = pMarketData->OpenPrice;
	strcpy(Instrument[nNoOfIns].UpdateTime,  pMarketData->UpdateTime);
	Instrument[nNoOfIns].modified = true;
	Instrument[nNoOfIns].rate = 100 * (Instrument[nNoOfIns].LastPrice - Instrument[nNoOfIns].PreSettlementPrice) / Instrument[nNoOfIns].PreSettlementPrice;	

	int hour, minu;
	sscanf(pMarketData->UpdateTime, "%02d:%02d", &hour, &minu);
	
	if ((hour == nowMinute[nNoOfIns].hour) &&
		(minu == nowMinute[nNoOfIns].minu)){
			nowMinute[nNoOfIns].PutData(pMarketData->LastPrice,
										pMarketData->OpenInterest,
										pMarketData->Volume);
	}
	else{
		char filename[30] = "data/";
		strcat(filename, contracts[nNoOfIns]);
		strcat(filename, ".txt");
		InsDataFile[nNoOfIns] = fopen(filename, "a+");
		nowMinute[nNoOfIns].PutToFile(InsDataFile[nNoOfIns]);
		nowMinute[nNoOfIns].Initial(pMarketData->LastPrice, hour, minu);
		fclose(InsDataFile[nNoOfIns]);
	}

	if ((hour == 15) && (minu == 15)){
		char filename[30] = "data/";
		strcat(filename, contracts[nNoOfIns]);
		strcat(filename, "K.txt");
		InsDataFile[nNoOfIns] = fopen(filename, "a+");
		COleDateTime t(COleDateTime::GetCurrentTime());
		fprintf(InsDataFile[nNoOfIns], "%d/%d/%d ", t.GetYear(), t.GetMonth(), t.GetDay());
		fprintf(InsDataFile[nNoOfIns], "%.2lf %.2lf %.2lf %.2lf ", 
			pMarketData->OpenPrice,
			pMarketData->LastPrice,
			pMarketData->HighestPrice,
			pMarketData->LowestPrice);
		fprintf(InsDataFile[nNoOfIns], "%.0lf %d\n", pMarketData->OpenInterest, pMarketData->Volume);
		fclose(InsDataFile[nNoOfIns]);
	}
}

void CLoginHandler::PrintData(){
	FILE *m_dataFile;
	m_dataFile = fopen("data.txt", "w");
	int i;
	for (i = 0; i < numOfIns; ++i){
		fprintf(m_dataFile, "%lf %lf %lf %d %lf %d %lf %d\n",
			Instrument[i].LastPrice,
			Instrument[i].rate,
			Instrument[i].BidPrice1,
			Instrument[i].BidVolume1,
			Instrument[i].AskPrice1,
			Instrument[i].AskVolume1,
			Instrument[i].PreSettlementPrice,
			Instrument[i].PreOpenInterest);
	}
	fclose(m_dataFile);
}

void CLoginHandler::getIns(int &num, char **&con){
	num = numOfIns;
	con = contracts;
}
void CLoginHandler::getData(InstrumentMsg* &InstrumentMsg){
	InstrumentMsg = Instrument;
}

void CLoginHandler::getNowMinute(MinuteMarketData* &MinuteMsg){
	 MinuteMsg = nowMinute;
}