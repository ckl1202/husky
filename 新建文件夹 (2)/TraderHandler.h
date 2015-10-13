#pragma once
#include "USTPFtdcTraderApi.h"
#include "afxcmn.h"
#include "resource.h"
#include "afxwin.h"
#include "DataStruct.h"

class CTraderHandler : public CUstpFtdcTraderSpi
{
private:
	CUstpFtdcTraderApi *m_pUserApi;
	int g_UserOrderLocalID;

	char BrokerID[10], UserID[20], Password[50];

	bool bConnectSuccessful;

	CDialogEx *pFatherDlg;

	doneOrder aDoneOrder[1000];
	waitOrder aWaitOrder[1000];

	int headDoneOrder, tailDoneOrder;
	int headWaitOrder, tailWaitOrder;

	char **contracts;
	int numOfIns;
	PositionMsg *Position;
	int time;

	int nNeedNotify;

	int strategyID1, strategyPosition1;
	int strategyID2, strategyPosition2;

public:
	CTraderHandler(){};
	CTraderHandler(CUstpFtdcTraderApi *pUserApi) : m_pUserApi(pUserApi){ 
		bConnectSuccessful = false;
		nNeedNotify = 0;
		headDoneOrder = 0;
		tailDoneOrder = 0;
		headWaitOrder = 0;
		tailWaitOrder = 0;
		FILE *userSelectFile;
		while(!(userSelectFile = fopen("userName.txt", "r")));
		int user;
		fscanf(userSelectFile, "%d", &user);
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

		contracts = NULL;
		Position = NULL;
		time = 2;

		FILE *InsListFile;
		InsListFile = fopen("insList.txt", "r");
		fscanf(InsListFile, "%d\n", &numOfIns);
		contracts = new char* [numOfIns];
		for (int i = 0; i < numOfIns; ++i){
			contracts[i] = new char[6];
			fscanf(InsListFile, "%s\n", contracts[i]);
		}
		fclose(InsListFile);

		Position = new PositionMsg[2 * numOfIns];

		//aDoneOrder = new doneOrder[10000];
		//aWaitOrder = new waitOrder[10000];
	};

	void SetFatherDlg(CDialogEx *pDlg){pFatherDlg = pDlg;}; 
	void OnFrontConnected();

	void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspuserLogin,
						CUstpFtdcRspInfoField *pRspInfo,
						int nRequstID,
						bool bIsLast);

	int getUserOrderLocalID(){
		return g_UserOrderLocalID;
	};

	void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount,
								CUstpFtdcRspInfoField *pRspInfo,
								int nRequestID,
								bool bIsLast);

	void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pInvestorPosition,
								CUstpFtdcRspInfoField *pRspInfo,
								int nRequestID,
								bool bIsLast);

	bool getStatus(){ return bConnectSuccessful; };

	void OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder,
						CUstpFtdcRspInfoField *pRspInfo,
						int nRequestID,
						bool bIsLast);

	void OnRtnTrade(CUstpFtdcTradeField *pTrade);

	void OnRtnOrder(CUstpFtdcOrderField *pOrder);

	void OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction,
						CUstpFtdcRspInfoField *pRspInfo,
						int nRequestID,
						bool bIsLast);

	void getDoneOrder();

	void getWaitOrder();

	void getPosition();

	void setNotify(int Notify){   
		nNeedNotify = Notify;
	};

	void regStrategy(int order, int localID);
	int getStrategy(int order);

	~CTraderHandler(void){
		--time;
		if (time == 0){
			for (int i = 0; i < numOfIns; ++i){
				delete[] contracts[i];
				//fclose(InsDataFile[i]);
			}
			delete[] contracts;
			delete[] Position;
			//delete[] aDoneOrder;
			//delete[] aWaitOrder;
		}
	};
};

