#ifndef DATAH
#define DATAH

#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcTraderApi.h"
//#include "TraderHandler.h"

struct doneOrder{
	TUstpFtdcInstrumentIDType InstrumentID;
	TUstpFtdcDirectionType Direction;
	TUstpFtdcOffsetFlagType OffsetFlag;
	TUstpFtdcPriceType TraderPrice;
	TUstpFtdcVolumeType TraderVolumn;
	TUstpFtdcTimeType TradeTime;
};

struct waitOrder{
	TUstpFtdcInstrumentIDType InstrumentID;
	TUstpFtdcDirectionType Direction;
	TUstpFtdcOffsetFlagType OffsetFlag;
	TUstpFtdcPriceType LimitPrice;
	TUstpFtdcVolumeType Volumn;
	TUstpFtdcUserOrderLocalIDType UserOrderLocalID;
};


struct InstrumentMsg{
	TUstpFtdcInstrumentIDType InstrumentID;
	TUstpFtdcPriceType LastPrice;
	double rate;
	TUstpFtdcPriceType BidPrice1;
	TUstpFtdcVolumeType BidVolume1;
	TUstpFtdcPriceType AskPrice1;
	TUstpFtdcVolumeType AskVolume1;
	TUstpFtdcPriceType PreSettlementPrice;
	TUstpFtdcVolumeType PreOpenInterest;
	TUstpFtdcTimeType UpdateTime;
	bool modified;
};

struct MinuteMarketData{
	//HMGNV-WCYXV-X7G9W-YCX63-B98R2
	TUstpFtdcPriceType openPrice;
	TUstpFtdcPriceType closePrice;
	TUstpFtdcPriceType highPrice;
	TUstpFtdcPriceType lowPrice;
	TUstpFtdcVolumeType totalVolume;
	TUstpFtdcVolumeType dealVolume;
	int hour, minu;

	void Initial(TUstpFtdcPriceType LastPrice, int nowHour, int nowMin){
		openPrice = LastPrice;
		closePrice = LastPrice;
		highPrice = LastPrice;
		lowPrice = LastPrice;
		hour = nowHour; minu = nowMin;
	};

	void PutData(TUstpFtdcPriceType LastPrice,
				TUstpFtdcLargeVolumeType TVolume,
				TUstpFtdcVolumeType DVolume){
		closePrice = LastPrice;
		if (LastPrice > highPrice) highPrice = LastPrice;
		if (LastPrice < lowPrice) lowPrice = LastPrice;
		totalVolume = int(TVolume);
		dealVolume = DVolume;
	};

	void PutToFile(FILE* outputFile){
		if (hour < 9) return;
		fprintf(outputFile, "%.2lf %.2lf %.2lf %.2lf %d %d %d:%d\n", 
			openPrice,
			closePrice,
			highPrice,
			lowPrice,
			totalVolume,
			dealVolume,
			hour, minu);
	};

	bool GetFromFile(FILE* inputFile){
		return (fscanf(inputFile, "%lf %lf %lf %lf %d %d %d:%d\n",
								&openPrice,
								&closePrice,
								&highPrice,
								&lowPrice,
								&totalVolume,
								&dealVolume,
								&hour, &minu) != EOF);
	};
};

struct PositionMsg{
	TUstpFtdcInstrumentIDType	InstrumentID;
	TUstpFtdcDirectionType	Direction;
	TUstpFtdcVolumeType	Position;
	TUstpFtdcPriceType	PositionCost;
	TUstpFtdcVolumeType	YdPosition;
	TUstpFtdcMoneyType	YdPositionCost;
	TUstpFtdcMoneyType	UsedMargin;
	bool setted;

	PositionMsg(){
		setted = false;
	};

	void Set(CUstpFtdcRspInvestorPositionField *pInvestorPosition){
		strcpy(InstrumentID, pInvestorPosition->InstrumentID);
		Direction = pInvestorPosition->Direction;
		Position = pInvestorPosition->Position;
		PositionCost = pInvestorPosition->PositionCost;
		YdPosition = pInvestorPosition->YdPosition;
		YdPositionCost = pInvestorPosition->YdPositionCost;
		UsedMargin = pInvestorPosition->UsedMargin;
		setted = true;
	};

	void PutToFile(FILE* outputFile){
		if (!setted) return;
		fprintf(outputFile, "%s %c %d %lf %d %lf %lf\n",
							InstrumentID,
							Direction,
							Position,
							PositionCost,
							YdPosition,
							YdPositionCost,
							UsedMargin);
	};
};



#endif