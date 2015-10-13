#pragma once
#include "UstpFtdcMduserApi.h"
#include "DataStruct.h"

class CLoginHandler : public CUstpFtdcMduserSpi
{
private:
	//指向实例的指针
	CUstpFtdcMduserApi *m_pUserApi;

	char BrokerID[10], UserID[20], Password[50];

	InstrumentMsg* Instrument;
	char **contracts;
	int numOfIns;
	int time;
	FILE **InsDataFile;
	MinuteMarketData *nowMinute;
public:	
	//构造函数 
	CLoginHandler(){
	};

	CLoginHandler(CLoginHandler &oldLoginHandler){
		m_pUserApi = oldLoginHandler.m_pUserApi;
		strcpy(BrokerID, oldLoginHandler.BrokerID);
		strcpy(UserID, oldLoginHandler.UserID);
		strcpy(Password, oldLoginHandler.Password);
	}

	CLoginHandler(CUstpFtdcMduserApi *pUserApi) : m_pUserApi(pUserApi) {
		contracts = NULL;
		Instrument = NULL;
		time = 2;

		FILE *InsListFile;
		InsListFile = fopen("insList.txt", "r");
		fscanf(InsListFile, "%d\n", &numOfIns);
		contracts = new char* [numOfIns];
		for (int i = 0; i < numOfIns; ++i){
			contracts[i] = new char[20];
			fscanf(InsListFile, "%s\n", contracts[i]);
		}
		fclose(InsListFile);

		Instrument = new InstrumentMsg[numOfIns];	
		for (int i = 0; i < numOfIns; ++i){
			Instrument[i].AskPrice1 = 0;
			Instrument[i].AskVolume1 = 0;
			Instrument[i].BidPrice1 = 0;
			Instrument[i].BidVolume1 = 0;
			Instrument[i].LastPrice = 0;
			Instrument[i].PreOpenInterest = 0;;
			Instrument[i].PreSettlementPrice = 0;
			Instrument[i].rate = 0;	
			strcpy(Instrument[i].UpdateTime, "");
			Instrument[i].modified = false;
		}

		InsDataFile = new FILE* [numOfIns];
		for (int i = 0; i < numOfIns; ++i){
			char filename[30] = "data/";
			strcat(filename, contracts[i]);
			strcat(filename, ".txt");
			/*InsDataFile[i] = fopen(filename,"r");
			int year, month, day;
			fscanf(InsDataFile[i], "%d.%d.%d", &year, &month, &day);
			COleDateTime t1(COleDateTime::GetCurrentTime());
			int nowYear = t1.GetYear(), nowMonth = t1.GetMonth(), nowDay = t1.GetDay();
			fclose(InsDataFile[i]);
			if ((year == nowYear) &&
				(month == nowMonth) &&
				(day == nowDay))
				InsDataFile[i] = fopen(filename, "a+");
			else {
				InsDataFile[i] = fopen(filename, "w");
				fprintf(InsDataFile[i], "%d.%d.%d\n", nowYear, nowMonth, nowDay);
			}*/
			InsDataFile[i] = fopen(filename, "w");
			fclose(InsDataFile[i]);
		}
		
		FILE *userSelectFile;
		while(!(userSelectFile = fopen("userName.txt", "r")));
		int user;
		fscanf(userSelectFile, "%d", &user);
		fclose(userSelectFile);
		strcpy(BrokerID, "1004");
		if (user == 0){
			strcpy(UserID, "0193758601");
			strcpy(Password, "ace5f5f8e09e3465e0ec53269439107f");
		}
		if (user == 1){
			strcpy(UserID, "0193758602");
			strcpy(Password, "8c79e2dcca1b4f2c021420f7caea07ab");
		}
		if (user == 2){
			strcpy(UserID, "0193758603");
			strcpy(Password, "8094cd0da911d17d9216153f03f31e99");
		}

		nowMinute = new MinuteMarketData[numOfIns];
		for (int i = 0; i < numOfIns; ++i){
			nowMinute[i].Initial(0, 0, 0);
		}
	};

	~CLoginHandler() {
		--time;
		if (time == 0){
			for (int i = 0; i < numOfIns; ++i){
				delete[] contracts[i];
				//fclose(InsDataFile[i]);
			}
			delete[] contracts;
			delete[] Instrument;
			delete[] InsDataFile;
			delete[] nowMinute;
		}
	};

	//客户端进行登录
	void OnFrontConnected();

	//客户端登录信息回调
	void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, 
						CUstpFtdcRspInfoField *pRspInfo,
						int nRequestID, 
						bool bIsLast);

	//深度行情通知
	void OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pMarketData);

	//出错通知
	void OnRspError(CUstpFtdcRspInfoField *pRspInfo,
					int nRequestID,
					bool bIsLast){};

	void OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
	};

	void PrintData();

	void getIns(int &, char** &);
	void getData(InstrumentMsg* &);
	void getNowMinute(MinuteMarketData * &);

	/*void OnFrontDisconnected(int nReason){
		char temp[100];
		sprintf(temp, "%d", nReason);
		MessageBox(NULL, CString(temp), CString("掉线"), MB_OK);
	}*/
};

