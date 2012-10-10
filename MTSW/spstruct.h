#ifndef _SPSTRUCT_H_
#define _SPSTRUCT_H_

/*******************************************************************************
File Name: spstruct.h

Description: This file contains the sp global struct definition.

Copyright:
    Copyright(c) 2011 LinkTop technologies Co., LTD. All rights reserved.

Change History:
2011-03-25  ww    v0.0.0.1  Create
2011-05-06  ww    v0.0.1.0  Check and release after function test
2011-08-31  ww    v1.0.0.1  modify to follow the function defintion of V1.1
2012-02-15  ww    v1.1.0.1  modify to follow the function defintion of V1.2
*******************************************************************************/

#include "typedef.h"
#include "spmsgid.h"
/*****************************************************************************
				 Global Struct Definitions
******************************************************************************/
#define    SP_NUM_DATA      160
#pragma pack(push) //保存对齐状态 

#pragma pack(1)//设定为1字节对齐
/*SP General Parameters */
typedef   struct
{
    uint16         sleepstep;
    uint16         LowBtlCout;
    uint8          Numbusy;
    uint8          tbusy;
    uint8          tsleep;
    uint8          tbtl;
    uint8          tvol;
    uint8          trssi;
    uint32         refrcc[16];
} SpGen_S;

/*pl calculate  param for mt*/
typedef   struct
{
    uint32         CalTxFreq;
    uint32         CalRxFreq;
    uint16         CalHighPC;
    uint16         CalLowPC;
    uint16         DigTXMod1Factor;
    uint16         DigTXMod2Factor;
    uint16   	   AnaTXMod1FactorAf;
    uint16         AnaTXMod2FactorAf;
    uint16   	   AnaTXMod1FactorCtc0;
    uint16   	   AnaTXMod2FactorCtc0;
    uint16   	   AnaTXMod1FactorCtc1;
    uint16   	   AnaTXMod2FactorCtc1;
    uint16   	   AnaTXMod1FactorCtc2;
    uint16   	   AnaTXMod2FactorCtc2;
    uint16   	   AnaTXMod1FactorCdc;
    uint16   	   AnaTXMod2FactorCdc;
    uint16         RxCalTV;
    uint16   	   RxNoise1Open;
    uint16   	   Rxnoise1Close;
    uint16   	   RxNoise9Open;
    uint16   	   Rxnoise9Close;
    uint16         Rxrssi1thrd;
    uint16         Rxrssi5thrd;
} PlCal_S;



/*SP mt  thrd Parameters */
typedef   struct
{
    uint16           BATLowThrd;
    uint16           TxVccn;
    uint16           Vox1open;
    uint16           Vox5open;
    uint16           maxvol;
    uint16           freqnum;
} SpMtTh_S;

/*SP mt  Parameters */
typedef   struct
{
    uint16           BATLowThrd;
    uint16           TxVccn;
    uint16           Vox1open;
    uint16           Vox5open;
    uint16           maxvol;
    uint16           freqnum;   /* 2-5*/
    PlCal_S          PlCal[5];
} SpMtFlh_S;


/*SP_DLL_INIT_REQ*/
typedef struct
{
    uint16         ReceiverMID;         /*Receiver MID*/
    uint16         SenderMID;           /*Sender MID*/
    uint16         MsgTYPE;             /*Msg Type*/
    uint16         MsgID;               /*Msg MID*/
    uint16         MsgLen;              /*Msg Length*/
    uint16         preamblelen;
    uint16         CRC;                 /*CRC*/
} DllInitReq_S;

/*SP_SCH_INIT_REQ*/
typedef   struct
{
    uint16         ReceiverMID;         /*Receiver MID*/
    uint16         SenderMID;           /*Sender MID*/
    uint16         MsgTYPE;             /*Msg Type*/
    uint16         MsgID;               /*Msg MID*/
    uint16         MsgLen;              /*Msg Length*/
    uint16         SpkGain;
    uint16         MicGain;
    uint8  	       chAss; 							/* whether is null channel */
    uint8  	       mode;  							/*0: analog  1: digital */
    uint8          ctcq;
    uint8          powerlevel;
    uint32         txfreq;
    uint32         rxfreq;
    uint32         txcc;
    uint32         rxcc;
    uint16         speechenhanen;
    uint8          interv;       /*channel interval    0:12.5KHZ  1：20KHZ    2：25KHZ      */
    uint8          voikeyon;
    uint32         voikey;
    uint8          txanatype;
    uint8          rxanatype;
    uint32         txananum;
    uint32         rxananum;
    uint16         TxVccn;
    uint16         TV;
    uint16         HighPC;
    uint16         LowPC;
    uint16         DigTXMod1Factor;
    uint16         DigTXMod2Factor;
    uint16         AnaTXMod1FactorAf;
    uint16         AnaTXMod2FactorAf;
    uint16   	   AnaTXMod1FactorCtc;
    uint16   	   AnaTXMod2FactorCtc;
    uint16   	   AnaTXMod1FactorCdc;
    uint16   	   AnaTXMod2FactorCdc;
    uint16          CRC;                 /*CRC*/
} SchInitReq_S;


/*SP_SCH_AUDIO_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         repeat;
    uint16         audtype;      /*0: single audio 1:pressed audio without channel
                                          2: pressed audio with channel 3: unpressed audio 4:circle single audio*/
    /*payload:uint16 freq+ uint16 duration*/
} SchAudio_S;

/*SCH_SP_AUDIO_IND*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         optind;     /*0:play finished unnormal 1:play finished normal 2:stop unnormal*/
    uint16         CRC;
} SchIndAudio_S;

/*SP_SCH_VOL_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         SpkGain;    /* 0- 127*/
    uint16         CRC;
} SchVol_S;

/*SP_SCH_ANASIGSET_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint8          txanatype;
    uint8          rxanatype;
    uint32         txananum;
    uint32         rxananum;
    uint16         CRC;
} SchAnaSig_S;

/*SP_SCH_CHCHG_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint8  	       chAss; 							/* whether is null channel */
    uint8  	       mode;  							/*0: analog  1: digital */
    uint8          ctcq;
    uint8          powerlevel;
    uint32         txfreq;
    uint32         rxfreq;
    uint32         txcc;
    uint32         rxcc;
    uint16         speechenhanen;
    uint8          interv;       /*channel interval    0:6.25KHZ  1:12.5KHZ  2:20KHZ  3:25KHZ      */
    uint8          voikeyon;
    uint32         voikey;
    uint8          txanatype;
    uint8          rxanatype;
    uint32         txananum;
    uint32         rxananum;
    uint16         TxVccn;
    uint16         TV;
    uint16         HighPC;
    uint16         LowPC;
    uint16         DigTXMod1Factor;
    uint16         DigTXMod2Factor;
    uint16         AnaTXMod1FactorAf;
    uint16         AnaTXMod2FactorAf;
    uint16   	   AnaTXMod1FactorCtc;
    uint16   	   AnaTXMod2FactorCtc;
    uint16   	   AnaTXMod1FactorCdc;
    uint16   	   AnaTXMod2FactorCdc;
    uint16         CRC;
} SpSchChg_S;

/*SCH_SP_CHCHG_CNF*/
typedef struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         chgready;       /*0:unready 1: ready*/
    uint16         CRC;
} SpSchChgCnf_S;

/*SP_SCH_MIC_IND*/
typedef   struct
{
    uint16         ReceiverMID;         /*Receiver MID*/
    uint16         SenderMID;           /*Sender MID*/
    uint16         MsgTYPE;             /*Msg Type*/
    uint16         MsgID;               /*Msg MID*/
    uint16         MsgLen;              /*Msg Length*/
    uint16         operate;             /* 0: pull in   1:pull out */
    uint16         CRC;                 /*CRC*/
} SchMicInd_S;

/*SP_SCH_MICGAIN_REQ*/
typedef struct
{
    uint16         ReceiverMID;                /*Receiver MID*/
    uint16         SenderMID;                /*Sender MID*/
    uint16         MsgTYPE;               /*Msg Type*/
    uint16         MsgID;                 /*Msg MID*/
    uint16         MsgLen;                /*Msg Length*/
    uint16         micgain;                /*0-63 */
    uint16         CRC;                    /*CRC*/
} SchMicGainReq_S;

/*SCH_SP_SLEEP_CNF/SP_SCH_SLEEP_REQ*/
typedef struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         sleepen;       /*0:nonsleep/dis 1: sleep/en*/
    uint16         CRC;
} SpSchSleep_S;

/*SP_SCH_RXSTART_REQ/SP_SCH_RXSTOP_REQ/SP_SCH_RXRFOPEN_REQ/SCH_SP_RXRFOPEN_CNF*/
typedef struct
{
    uint16                 ReceiverMID;            /*Receiver MID*/
    uint16                 SenderMID;            /*Sender MID*/
    uint16                 MsgTYPE;           /*Msg Type*/
    uint16                 MsgID;             /*Msg ID*/
    uint16                 MsgLen;            /*Msg Length--null msg*/
    uint16                 CRC;               /*CRC*/
} SpSchRxReq_S;

/*SP_SCH_MAXPW_REQ*/
typedef struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         MaxPower;               /* 0: low   1: high */
    uint16         CRC;
} SchMaxPower_S;








/*SP_MMI_HOLD_IND/CCL_SP_HOLD_IND/SP_CCL_ALARM_CANCEL_REQ/MMI_SP_POWEROFF_REQ/MMI_SP_SCAN_CANCEL_REQ/
/CCL_SP_POWEROFF_REQ/SCH_SP_POWEROFF_REQ/SP_MMI_EXIT_REQ/MMI_SP_EXIT_CNF/MMI_SP_HOLD_CANCEL_REQ/
SP_CCL_HOLD_CANCEL_REQ/MMI_SP_BACK_REQ/SP_CCL_BACK_REQ/SP_CCL_PCCONNECT_REQ*/
typedef struct
{
    uint16         ReceiverMID;         /*Receiver MID*/
    uint16         SenderMID;           /*Sender MID*/
    uint16         MsgTYPE;             /*Msg Type*/
    uint16         MsgID;                /*Msg MID*/
    uint16         MsgLen;              /*Msg Length*/
    uint16         CRC;                    /*CRC*/
} SpHold_S, SpAlrmCanl_S, SpPwrOff_S, SpExit_S, SpHoldCanl_S, SpBack_S, SpScanCanl_S, CclSwOn_S;

/*CCL_SP_TOTNOTE_IND/SP_MMI_TOTNOTE_IND*/
typedef struct
{
    uint16         ReceiverMID;         /*Receiver MID*/
    uint16         SenderMID;           /*Sender MID*/
    uint16         MsgTYPE;             /*Msg Type*/
    uint16         MsgID;                /*Msg MID*/
    uint16         MsgLen;              /*Msg Length*/
    uint16         duration;            /*pre--tot*/
    uint16         CRC;                    /*CRC*/
} SPNote_S;



/*PC_SP_CONNECT_REQ/MT_SP_CONNECT_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint32         connect;
    uint16         CRC;
}
SpPcConnect_S, SpMtConnect_S;

/*SP_PC_CONNECT_CNF/SP_MT_CONNECT_CNF*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint32         ready;
    uint16         CRC;
}
SpPcConCnf_S, SpMtConCnf_S;

/*SP_PC_KEY_REQ */
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         keytype;
    uint16         CRC;
} SpPckeyReq_S;

/*PC_SP_KEY_CNF*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         keytype;
    uint16         keyon;
    uint32         key;
    uint16         CRC;
} SpPckeyCnf_S;

/*SP_PC_UPDATEKEY_REQ */
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint32         writekey;
    uint16         keytype;
    uint16         keyon;
    uint32         key;
    uint16         CRC;
} SpPcUpdkeyReq_S;

/*PC_SP_UPDATEKEY_CNF*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint32         success;
    uint16         keytype;
    uint16         CRC;
} SpPcUpdkeyCnf_S;

 /*MT_SP_READ_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint32         read;
    uint16         CRC;
} SpMtRead_S;

/*MT_SP_ADCAL_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         operate;
    uint16         checkitem;
    uint16         CRC;
} SpMtAd_S;

/*SP_MT_ADCAL_IND*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         checkitem;
    uint16         result;
    uint16         CRC;
} SpMtAdInd_S;

/*MT_SP_MAXVOL_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         operate;
    uint16         volgain;
    uint16         CRC;
}SpMtMaxvol_S ;


/*SP_MT_READ_CNF*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint32         success;
    uint16         BATLowThrd;
    uint16         TxVccn;
    uint16         Vox1open;
    uint16         Vox5open;
    uint16         maxvol;
    uint16         freqnum;
    PlCal_S        PlCal[5];
    uint16         CRC;
} SpMtReadCnf_S;

/*MT_SP_WRITE_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint32         write;
    uint16         BATLowThrd;
    uint16         TxVccn;
    uint16         Vox1open;
    uint16         Vox5open;
    uint16         maxvol;
    uint16         freqnum;
    PlCal_S        PlCal[5];
    uint16         CRC;
} SpMtWrite_S;

 
/*SP_MT_WRITE_CNF/SP_MT_EFFECT_CNF/SP_PC_EFFECT_CNF*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint32         success;
    uint16         CRC;
} SpMtWriteCnf_S, SpEffectCnf_S;

/*PC_SP_EFFECT_REQ/MT_SP_EFFECT_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint32         reset;
    uint16         CRC;
}
SpPcEffect_S, SpMtEffect_S;

/*DEBUG_SP_TEST_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         testlevel;
    uint16         CRC;
} SpDebug_S;

/*SP_DEBUG_GLB_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint8          Tsleep;            /*   sleep minimum time length*/
    uint8          testlev;           /* 0-5*/
    uint16         sleepstep;
    uint32         Tbtl;             /* check batterylife timer-- timer3 */
    uint16         TVol;             /* check volume timer -- timer2*/
    uint16         TRssi;            /*check rssi&busy  timer--timer3*/
    uint8          timer7on;
    uint8          timer8on;
    uint8          timer9on;
    uint8          alaon;
    uint8          pwsOn;            /*0: powersave off 1: powersave on*/
    uint8          pws;              /* whether other modules  can sleep  */
    uint16         freetime;
    uint8          sysready;             /* whether other modules is ready*/
    uint8          exitred;              /* the print level */
    uint8          Rssi;                 /*last  rssi*/
    uint8          Btl;                  /*last batterylife*/
    uint16         Vol;                  /*last volume*/
    uint16         ChanNum;              /*current channel number : 0-15*/
    uint16         chanumback;
    uint16         chanumtep;
    uint16         chanumalive;
    uint16         chanumcall;
    uint8          ScanTime;               /*scan time : 0-16*/
    uint8          ScanOn;                 /*whether scan is on*/
    uint8          ScanCall;
    uint8          ChCalStatus;       /*whether the mt parameter of channel is caculated*/
    uint8          ChaChOn;                /*whether is wait for CCL_SP_STOP_CNF*/
    uint8          schchgon;
    uint16         busyopen[10];
    uint16         busyclose[10];
    uint16         btlTh;                   /*the low level batterylife thrd*/
    uint16         callon;
    uint16         sendchange;
    uint8          waitslp;
    uint8          RcStatus;                /*whether ms is active  0: killed 1: active*/
    uint16         CRC;
} SpGlb_S;

/*DEBUG_SP_RDGLB_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         Glbkey;
    uint16         module;
    uint16         CRC;
} DebugReadGlbReq_S;

/*SP_DEBUG_RDSYSGLB_CNF*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint32         success;
    uint8          Workmode;     /*system mode 0:res 1:digtal 2: analog 3:dual*/
    uint8          RCStatus;     /*whether ms is active*/
    uint8          EnKilled;     /*whether can been killed*/
    uint8          EnActived;    /*whether can been actived*/
    uint32         MtKey;        /*MT key*/
    uint8          MicGainHigh;
    uint8          MicGainLow;
    uint16         Spklev[15];
    uint8          SpkGain[16];
    uint16         BtlLev[4];
    uint16         VoxLev[5];
    uint32         TxCC[16][16];     /*channel tx color code  */
    uint32         RxCC[16][16];    /*channel rx color code  */
    uint8          ChCalStatus;
    uint8          VolLevel;
    uint8          CurZoneNum;
    uint8          CurChanNum;
    uint8          PerSZoneNum;
    uint8          PerSChanNum;
    uint8          Sreseve;
    uint16         BATLowThrd;
    uint16         TxVccn;
    uint16         Vox1open;
    uint16         Vox5open;
    uint16         maxvol;
    uint16         freqnum;
    PlCal_S        PlCal[5];
    uint16         sleepstep;
    uint8          tsleep;
    uint8          tbtl;
    uint8          tvol;
    uint8          trssi;
    uint32         refrcc[16];
    uint8          bTack;              /* T_ACK */
    uint8          btalarm;
    uint16         wTTxwait;          /* Tx_wait:
    										                        0000: 	       No Time
											                    0001:         40ms
												                0010:         80ms
												                0011:         160ms
												                0100:         320ms
												                other:        Reserve
										                      */
    uint8          bNumRep;            /*NUM_REP*/
    uint8          bNumData;           /*NUM_DATA*/
    uint8          bNumStatus;         /*NUM_STATUS*/
    uint8          bNumbusy;
    uint8          bTbusy;
    uint8          bheadcnt;
    uint16         wTRsp;              /*T_RSP*/

    uint16         wTsync;              /*T_SYNC*/
    uint32         Uart2Baudrate;
    uint32         RfVcoRfin;
    uint16         RfVcoRefer;
    uint16         RfVcoPreDiv;
    uint16         RfVcoCurSet;
    uint16         RfVcoMuxOut;
    uint16         RfDelay5TOn;
    uint16         RfDelay5TOff;
    uint16         RfDelay5ROn;
    uint16         RfDelay5ROff;
    uint16         RfDelay5COn;
    uint16         RfDelay5COff;
    uint16         RfDelayTxOn;
    uint16         RfDelayTxOff;
    uint16         RfDelayRxOn;
    uint16         RfDelayRxOff;
    uint16         RfDelayPaOn;
    uint16         RfDelayPaOff;
    uint16         RfDelayPllTROn;
    uint16         RfDelayPllTROff;
    uint16         RfDelayTxVco;
    uint16         RfDelayRxVco;
    uint16         SpchDelaySpkOn;
    uint16         SpchDelaySpkOff;
    uint16         SpchDelayAmpOn;
    uint16         SpchDelayAmpOff;
    uint16         SpchDelayMicOn;
    uint16         SpchDelayMicOff;
    uint16         SpchDelayMuteOn;
    uint16         SpchDelayMuteOff;
    uint16         SpchDelaySleep;
    uint16         SpchDelayWake;
    uint16         adamp;
    uint16         affactr;
    uint32         spechengain;
    uint8          digsegon;
    uint8          anasegon;
    uint16         prembllen;
    uint16         CRC;
} SpReadSysGlbCnf_S;

/*SP_DEBUG_RDCHCALGLB_CNF*/
typedef   struct
{
    uint16                ReceiverMID;
    uint16                SenderMID;
    uint16                MsgTYPE;
    uint16                MsgID;
    uint16                MsgLen;
    uint32                success;
    SpMtChaCalc_S         chalc[16][16];
    uint16                CRC;
} SpReadChlGlbCnf_S;

/*DEBUG_SP_WTSYSGLB_REQ*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint32         glbkey;
    uint8          Workmode;     /*system mode 0:res 1:digtal 2: analog 3:dual*/
    uint8          RCStatus;     /*whether ms is active*/
    uint8          EnKilled;     /*whether can been killed*/
    uint8          EnActived;    /*whether can been actived*/
    uint32         MtKey;        /*MT key*/
    uint8          MicGainHigh;
    uint8          MicGainLow;
    uint16         Spklev[15];
    uint8          SpkGain[16];
    uint16         BtlLev[4];
    uint16         VoxLev[5];
    uint32         TxCC[16][16];     /*channel tx color code  */
    uint32         RxCC[16][16];    /*channel rx color code  */
    uint8          ChCalStatus;
    uint8          VolLevel;
    uint8          CurZoneNum;
    uint8          CurChanNum;
    uint8          PerSZoneNum;
    uint8          PerSChanNum;
    uint8          Sreseve;
    uint16         BATLowThrd;
    uint16         TxVccn;
    uint16         Vox1open;
    uint16         Vox5open;
    uint16         maxvol;
    uint16         freqnum;
    PlCal_S        PlCal[5];
    uint16         sleepstep;
    uint8          tsleep;
    uint8          tbtl;
    uint8          tvol;
    uint8          trssi;
    uint32         refrcc[16];
    uint8          bTack;              /* T_ACK */
    uint8          btalarm;
    uint16         wTTxwait;          /* Tx_wait:
    										                        0000: 	       No Time
											                    0001:         40ms
												                0010:         80ms
												                0011:         160ms
												                0100:         320ms
												                other:        Reserve
										                      */
    uint8          bNumRep;            /*NUM_REP*/
    uint8          bNumData;           /*NUM_DATA*/
    uint8          bNumStatus;         /*NUM_STATUS*/
    uint8          bNumbusy;
    uint8          bTbusy;
    uint8          bheadcnt;
    uint16         wTRsp;              /*T_RSP*/

    uint16         wTsync;              /*T_SYNC*/
    uint32         Uart2Baudrate;
    uint32         RfVcoRfin;
    uint16         RfVcoRefer;
    uint16         RfVcoPreDiv;
    uint16         RfVcoCurSet;
    uint16         RfVcoMuxOut;
    uint16         RfDelay5TOn;
    uint16         RfDelay5TOff;
    uint16         RfDelay5ROn;
    uint16         RfDelay5ROff;
    uint16         RfDelay5COn;
    uint16         RfDelay5COff;
    uint16         RfDelayTxOn;
    uint16         RfDelayTxOff;
    uint16         RfDelayRxOn;
    uint16         RfDelayRxOff;
    uint16         RfDelayPaOn;
    uint16         RfDelayPaOff;
    uint16         RfDelayPllTROn;
    uint16         RfDelayPllTROff;
    uint16         RfDelayTxVco;
    uint16         RfDelayRxVco;
    uint16         SpchDelaySpkOn;
    uint16         SpchDelaySpkOff;
    uint16         SpchDelayAmpOn;
    uint16         SpchDelayAmpOff;
    uint16         SpchDelayMicOn;
    uint16         SpchDelayMicOff;
    uint16         SpchDelayMuteOn;
    uint16         SpchDelayMuteOff;
    uint16         SpchDelaySleep;
    uint16         SpchDelayWake;
    uint16         adamp;
    uint16         affactr;
    uint32         spechengain;
    uint8          digsegon;
    uint8          anasegon;
    uint16         prembllen;
    uint16         CRC;
} DebugWriteSysGlbReq_S;

/*DEBUG_SP_WTCHLCALGLB_REQ*/
typedef   struct
{
    uint16               ReceiverMID;
    uint16               SenderMID;
    uint16               MsgTYPE;
    uint16               MsgID;
    uint16               MsgLen;
    uint32               glbkey;
    SpMtChaCalc_S        chalc[16][16];
    uint16               CRC;
} DebugWriteChlGlbReq_S;

/*SP_DEBUG_WTGLB_CNF*/
typedef   struct
{
    uint16         ReceiverMID;
    uint16         SenderMID;
    uint16         MsgTYPE;
    uint16         MsgID;
    uint16         MsgLen;
    uint16         success;
    uint16         module;
    uint16         CRC;
} SpWriteGlbCnf_S;

/*sysglb parameter*/
typedef   struct
{
    uint8          Workmode;     /*system mode 0:res 1:digtal 2: analog 3:dual*/
    uint8          RCStatus;     /*whether ms is active*/
    uint8          EnKilled;     /*whether can been killed*/
    uint8          EnActived;    /*whether can been actived*/
    uint32         MtKey;        /*MT key*/
    uint8          MicGainHigh;
    uint8          MicGainLow;
    uint16         Spklev[15];
    uint8          SpkGain[16];
    uint16         BtlLev[4];
    uint16         VoxLev[5];
    uint32         TxCC[16][16];     /*channel tx color code  */
    uint32         RxCC[16][16];    /*channel rx color code  */
    uint8          ChCalStatus;
    uint8          VolLevel;
    uint8          CurZoneNum;
    uint8          CurChanNum;
    uint8          PerSZoneNum;
    uint8          PerSChanNum;
    uint8          Sreseve;
    uint16         BATLowThrd;
    uint16         TxVccn;
    uint16         Vox1open;
    uint16         Vox5open;
    uint16         maxvol;
    uint16         freqnum;
    PlCal_S        PlCal[5];
    uint16         sleepstep;
    uint8          tsleep;
    uint8          tbtl;
    uint8          tvol;
    uint8          trssi;
    uint32         refrcc[16];
    uint8          bTack;              /* T_ACK */
    uint8          btalarm;
    uint16         wTTxwait;          /* Tx_wait:
    										                        0000: 	       No Time
											                    0001:         40ms
												                0010:         80ms
												                0011:         160ms
												                0100:         320ms
												                other:        Reserve
										                      */
    uint8          bNumRep;            /*NUM_REP*/
    uint8          bNumData;           /*NUM_DATA*/
    uint8          bNumStatus;         /*NUM_STATUS*/
    uint8          bNumbusy;
    uint8          bTbusy;
    uint8          bheadcnt;
    uint16         wTRsp;              /*T_RSP*/

    uint16         wTsync;              /*T_SYNC*/
    uint32         Uart2Baudrate;
    uint32         RfVcoRfin;
    uint16         RfVcoRefer;
    uint16         RfVcoPreDiv;
    uint16         RfVcoCurSet;
    uint16         RfVcoMuxOut;
    uint16         RfDelay5TOn;
    uint16         RfDelay5TOff;
    uint16         RfDelay5ROn;
    uint16         RfDelay5ROff;
    uint16         RfDelay5COn;
    uint16         RfDelay5COff;
    uint16         RfDelayTxOn;
    uint16         RfDelayTxOff;
    uint16         RfDelayRxOn;
    uint16         RfDelayRxOff;
    uint16         RfDelayPaOn;
    uint16         RfDelayPaOff;
    uint16         RfDelayPllTROn;
    uint16         RfDelayPllTROff;
    uint16         RfDelayTxVco;
    uint16         RfDelayRxVco;
    uint16         SpchDelaySpkOn;
    uint16         SpchDelaySpkOff;
    uint16         SpchDelayAmpOn;
    uint16         SpchDelayAmpOff;
    uint16         SpchDelayMicOn;
    uint16         SpchDelayMicOff;
    uint16         SpchDelayMuteOn;
    uint16         SpchDelayMuteOff;
    uint16         SpchDelaySleep;
    uint16         SpchDelayWake;
    uint16         adamp;
    uint16         affactr;
    uint32         spechengain;
    uint8          digsegon;
    uint8          anasegon;
    uint16         prembllen;
} SysGlbReq_S;

typedef		struct	 
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	SpMtConnect_S	SpMtConnect;
	uint16	EndFrame;
}SpMtConnectReq;
typedef		struct	
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	SpMtRead_S	SpMtRead;
	uint16	EndFrame;
}SpMtReadReq;

typedef		struct
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	SpMtEffect_S	SpMtEffect;
	uint16	EndFrame;
}SpMtEffectReq;

typedef		struct
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	SpMtMaxvol_S	SpMtMaxvol;
	uint16	EndFrame;
}SpMtMaxvolReq;
/*MT_SCH_ANA_TXCAL_REQ*/
typedef		struct
{
	uint16					HeaderFrame;
	uint16					MsgLen;
	uint16               ReceiverMID;
	uint16               SenderMID;
	uint16               MsgTYPE;
	uint16               MsgID;
	uint16               MsgContLen;
	uint32				   txananum;
	uint16               PC;
	uint16               Interval;	//信道间隔
	uint16               SigType;	//模拟信令类型
	uint32               SigNum;		//模拟信令号码
	uint16               MOD1;		//
	uint16               MOD2;		//
 	uint16               CRC;
	uint16               EndFrame;
}MtSchAnaTXCalReq;

/*MT_SCH_ANA_TXCAL_REQ*/
typedef		struct
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	uint16               ReceiverMID;
	uint16               SenderMID;
	uint16               MsgTYPE;
	uint16               MsgID;
	uint16               MsgContLen;
	uint16               CRC;
	uint16               EndFrame;
}MtSchAnaTXCalStop;

/*MT_SCH_ANA_RXCAL_REQ*/
typedef		struct
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	uint16               ReceiverMID;
	uint16               SenderMID;
	uint16               MsgTYPE;
	uint16               MsgID;
	uint16               MsgContLen;
	uint32				   txananum;
	uint16               TV;
	uint16               CRC;
	uint16               EndFrame;
}MtSchAnaRXCalReq;

/*MT_SCH_ANA_RXCAL_STOP*/
typedef		struct
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	uint16               ReceiverMID;
	uint16               SenderMID;
	uint16               MsgTYPE;
	uint16               MsgID;
	uint16               MsgContLen;
	uint16               CRC;
	uint16               EndFrame;
}MtSchCalStop;

/*MT_SCH_DIG_TXCAL_REQ */
typedef		struct
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	uint16               ReceiverMID;
	uint16               SenderMID;
	uint16               MsgTYPE;
	uint16               MsgID;
	uint16               MsgContLen;
	uint32				   txananum;	//发射频率
	uint16               PC;			//低功率时的校准值
	uint16               Interval;
	uint16               MOD1;		//频偏
	uint16               MOD2;		//平衡
	uint16               CRC;
	uint16               EndFrame;
}MtSchDitTXCalReq;


/*MT_SCH_DIG_RXCAL_REQ */
typedef		struct
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	uint16               ReceiverMID;
	uint16               SenderMID;
	uint16               MsgTYPE;
	uint16               MsgID;
	uint16               MsgContLen;
	uint32				   txananum;
	uint16               TV;
	uint16               Interval;
	uint16               Period;
	uint16               CRC;
	uint16               EndFrame;
}SpSchDitRXCalReq;
/*SCH_MT_DIG_RXCAL_RSP */
typedef		struct
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	uint16               ReceiverMID;
	uint16               SenderMID;
	uint16               MsgTYPE;
	uint16               MsgID;
	uint16               MsgContLen;
	uint32				   txananum;
	uint16               TV;
	uint16               TotalBitNum;
	uint16               ErrorBitNum;
	uint16               CRC;
	uint16               EndFrame;
}SpSchDitRXCalRsp;

typedef		struct
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	SpMtAd_S	SpMtAd;
	uint16               EndFrame;
}SpMtAdReq;

/*MT_SCH_ANA_FREQCAL_REQ*/
typedef		struct
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	uint16               ReceiverMID;
	uint16               SenderMID;
	uint16               MsgTYPE;
	uint16               MsgID;
	uint16               MsgContLen;
	uint32				   txananum;
	uint16               VCCN;
	uint16               CRC;
	uint16               EndFrame;
}MtSchAnaFreqCalReq;

/*MT_SCH_ANA_FREQCAL_STOP */
typedef		struct
{
	uint16	HeaderFrame;
	uint16	MsgLen;
	uint16               ReceiverMID;
	uint16               SenderMID;
	uint16               MsgTYPE;
	uint16               MsgID;
	uint16               MsgContLen;
	uint16               CRC;
	uint16               EndFrame;
}MtSchAnaFreqCalStop;

/*******************************************************************************
                Function Declarations
*******************************************************************************/

/*******************************************************************************
                                Global Variables
*******************************************************************************/

extern uint8     gSPTSleep;
extern uint8     gSPAlaOn;
extern uint16    gSPSleepStep;
extern uint32    gSPTbtl;
extern uint16    gSPTVol;
extern uint16    gSPTRssi;
extern uint16    gSPTBusy;
extern uint8     gSPTimer7On;
extern uint8     gSPTimer8On;
extern uint8     gSPTimer9On;
extern uint8     gSPTimer12On;
extern uint8     gSPPws;
extern uint16    gSPFree;
extern uint8     gSPSysready;
extern uint8     gSPSysExit;
extern uint8     gSPTestLev;
extern uint8     gSPRssi;
extern uint8     gSPBtl;
extern uint8     gSPTempZone;
extern uint8     gSPAliveZone;
extern uint8     gSPCallZone;
extern uint8     gSPTempChan;
extern uint8     gSPAliveChan;
extern uint8     gSPCallChan;
extern uint8     gSPScanTime;
extern uint8     gSPScanCall;
extern uint8     gSPReChgOn;
extern uint8     gSPChaChOn;
extern uint8     gSPSchChgOn;
extern uint8     gSPScanlist[SP_SCANNUM];
extern uint8     gSPScanlistnum;
extern uint8     gSPTalkArouOn;
extern uint8     gSPWaitSlp;
extern uint8     gSPCallBackCh;
extern uint16    gSPbtlTh;
extern uint16    gSPVccn;
extern uint8     gSPChlOptOn;
extern uint8     gSPSaveOn;
extern uint16    gSPPerBtl[SP_BTLNUM];
extern uint16    gSPTxlowBtl;
extern uint8     gSPLowBtlCout;
extern uint8     gSPLowBtlTime;
extern uint8     gSPLowBtlTxTime;
extern uint8     gSPTimer11On;
extern uint8     gSPState;
extern uint8     gSPMode;
extern uint8     gSPRstOn;
extern uint8     gSPSwType;
extern uint8     gSPChBusy;
extern uint8     gSPRxImdInd;
extern uint8     gSPBusyCloseNum;
extern uint8     gSPBusyNUM;
extern uint8     gSPStartInd;
extern uint8     gSPBusyNUMTh;
extern uint8     gSPTBusyIND;
extern uint8     gSPSendAllow;
extern uint8     gSPSendOverInd;
extern uint8     gSPRFOpenOn;
extern uint8     gSPRFInd;
extern uint8     gSPEP;
extern uint8     gSPMsgBuff[SP_BUFF_LENGTH];
extern uint8     gSPMsgBuffOn;
extern uint8     gSPFirstLowBtl;
extern uint16    gSPSEBackup;
extern uint8     gSPCallOn;
extern uint8     gSPVoiceKeyOnBackup;
extern uint32    gSPVoiceKeyBackup;
extern uint16    gSPCallType;
extern uint32     gSPAppVer;
#pragma pack(pop)//恢复对齐状态
#endif  /* _SPSTRUCT_H_ */

