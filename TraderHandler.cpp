#include "StdAfx.h"
#include "TraderHandler.h"

void CTraderHandler::OnFrontConnected(){
	CUstpFtdcReqUserLoginField reqUserLogin;

	//会员ID
	strcpy(reqUserLogin.BrokerID, BrokerID);
	//用户ID
	strcpy(reqUserLogin.UserID, UserID);
	//密码
	strcpy(reqUserLogin.Password, Password);

	m_pUserApi -> ReqUserLogin(&reqUserLogin, 0);
}

void CTraderHandler::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspuserLogin,
									CUstpFtdcRspInfoField *pRspInfo,
									int nRequstID,
									bool bIsLast){
	if (strcmp(pRspuserLogin->MaxOrderLocalID, "")) g_UserOrderLocalID = atoi(pRspuserLogin -> MaxOrderLocalID);
	else g_UserOrderLocalID = 0;
	if (pRspInfo -> ErrorID != 0) MessageBox(NULL, CString(pRspInfo -> ErrorMsg), CString("登录错误"), 0);
	else bConnectSuccessful = true;
}

void CTraderHandler::OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount,
											CUstpFtdcRspInfoField *pRspInfo,
											int nRequestID,
											bool bIsLast){

	FILE *m_moneyfile;
	m_moneyfile = fopen("money.txt", "w");
	fprintf(m_moneyfile, "%.2lf\n%.2lf", pRspInvestorAccount->LongMargin + pRspInvestorAccount->ShortMargin, pRspInvestorAccount -> Available);
	fclose(m_moneyfile);
}

void CTraderHandler::OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pInvestorPosition,
											CUstpFtdcRspInfoField *pRspInfo,
											int nRequestID,
											bool bIsLast){
	if (pInvestorPosition == NULL) return;
	int nNoOfIns;

	for (int i = 0; i < numOfIns; ++i){
		bool check = true;
		for (int j = 0; j < 6; ++j) if (pInvestorPosition->InstrumentID[j] != contracts[i][j]){
			check = false;
			break;
		}
		if (check){
			nNoOfIns = i;
			break;
		}
	}
	/*FILE* posFile = fopen("position.txt", "a+");

		fprintf(posFile, "%s %c %d %lf %d %lf %lf\n",
							pInvestorPosition->InstrumentID,
							pInvestorPosition->Direction,
							pInvestorPosition->Position,
							pInvestorPosition->PositionCost,
							pInvestorPosition->YdPosition,
							pInvestorPosition->YdPositionCost,
							pInvestorPosition->UsedMargin);
	fclose(posFile);*/
	if (pInvestorPosition->Direction == '0') Position[2 * nNoOfIns].Set(pInvestorPosition);
	else Position[2 * nNoOfIns + 1].Set(pInvestorPosition);
}
void CTraderHandler::OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder,
									CUstpFtdcRspInfoField *pRspInfo,
									int nRequestID,
									bool bIsLast){
	if (pRspInfo -> ErrorID != 0) MessageBox(NULL, CString(pRspInfo -> ErrorMsg), CString("报单回馈"), MB_OK);
	else if (nNeedNotify == 0) MessageBox(NULL, CString("报单录入成功"), CString("报单回馈"), MB_OK);
	else nNeedNotify--;
}

void CTraderHandler::OnRtnTrade(CUstpFtdcTradeField *pTrade){
	++tailDoneOrder;
	tailDoneOrder %= 1000;
	aDoneOrder[tailDoneOrder].Direction = pTrade->Direction;
	strcpy(aDoneOrder[tailDoneOrder].InstrumentID, pTrade->InstrumentID);
	aDoneOrder[tailDoneOrder].OffsetFlag = pTrade->OffsetFlag;
	aDoneOrder[tailDoneOrder].TraderPrice = pTrade->TradePrice;
	aDoneOrder[tailDoneOrder].TraderVolumn = pTrade->TradeVolume;
	strcpy(aDoneOrder[tailDoneOrder].TradeTime, pTrade->TradeTime);
}

void CTraderHandler::OnRtnOrder(CUstpFtdcOrderField *pOrder){
	++tailWaitOrder;
	tailWaitOrder %= 1000;
	aWaitOrder[tailWaitOrder].Direction = pOrder->Direction;
	strcpy(aWaitOrder[tailWaitOrder].InstrumentID, pOrder->InstrumentID);
	aWaitOrder[tailWaitOrder].OffsetFlag = pOrder->OffsetFlag;
	aWaitOrder[tailWaitOrder].LimitPrice = pOrder->LimitPrice;
	aWaitOrder[tailWaitOrder].Volumn = pOrder->VolumeRemain;
	strcpy(aWaitOrder[tailWaitOrder].UserOrderLocalID,pOrder->UserOrderLocalID);
	if (pOrder->OrderStatus == USTP_FTDC_OS_Canceled) aWaitOrder[tailWaitOrder].Volumn = 0;
	int id;
	sscanf(aWaitOrder[tailWaitOrder].UserOrderLocalID, "%d", &id);
	if (id == strategyID1) strategyPosition1 = aWaitOrder[tailWaitOrder].Volumn;
	if (id == strategyID2) strategyPosition2 = aWaitOrder[tailWaitOrder].Volumn;
}

void CTraderHandler::OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction,
						CUstpFtdcRspInfoField *pRspInfo,
						int nRequestID,
						bool bIsLast){
	if (pRspInfo->ErrorID != 0)
		MessageBox(NULL, CString(pRspInfo->ErrorMsg), CString("撤单回馈"), MB_OK);
	else MessageBox(NULL, CString("撤单成功"), CString("撤单回馈"), MB_OK);
}

void CTraderHandler::getDoneOrder(){
	FILE *doneOrderFile;
	doneOrderFile = fopen("doneOrder.txt", "w");
	while (headDoneOrder != tailDoneOrder){
		++headDoneOrder;
		headDoneOrder %= 1000;
		fprintf(doneOrderFile, "%s %c %c %lf %d %s\n",
			aDoneOrder[headDoneOrder].InstrumentID,
			aDoneOrder[headDoneOrder].Direction,
			aDoneOrder[headDoneOrder].OffsetFlag,
			aDoneOrder[headDoneOrder].TraderPrice,
			aDoneOrder[headDoneOrder].TraderVolumn,
			aDoneOrder[headDoneOrder].TradeTime);
	}
	fclose(doneOrderFile);
}

void CTraderHandler::getWaitOrder(){
	FILE *waitOrderFile;
	waitOrderFile = fopen("waitOrder.txt", "w");
	while (headWaitOrder != tailWaitOrder){
		++headWaitOrder;
		headWaitOrder %= 1000;
		fprintf(waitOrderFile, "%s %c %c %lf %d %s\n",
			aWaitOrder[headWaitOrder].InstrumentID,
			aWaitOrder[headWaitOrder].Direction,
			aWaitOrder[headWaitOrder].OffsetFlag,
			aWaitOrder[headWaitOrder].LimitPrice,
			aWaitOrder[headWaitOrder].Volumn,
			aWaitOrder[headWaitOrder].UserOrderLocalID);
	}
	fclose(waitOrderFile);
}

void CTraderHandler::getPosition(){
	FILE* posFile;
	posFile = fopen("position.txt", "w");
	for (int i = 0; i < 2 * numOfIns; ++i){
		Position[i].PutToFile(posFile);
	}
	fclose(posFile);
}

void CTraderHandler::regStrategy(int order, int localID){
	if (order == 1){
		strategyID1 = localID;
	}
	if (order == 2){
		strategyID2 = localID;
	}
}

int CTraderHandler::getStrategy(int order){
	if (order == 1) return strategyPosition1;
	if (order == 2) return strategyPosition2;
}
