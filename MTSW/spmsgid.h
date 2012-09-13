/*******************************************************************************
File Name: spmsgid.h

Description: This file contains the sp macro and all callback function declaration for ARMDD.

Copyright:
Copyright(c) 2011 LinkTop technologies Co., LTD. All rights reserved.

Change History:
2011-03-25  ww    v0.0.0.1  Create
2011-05-06  ww    v0.0.1.0  Check and release after function test
2011-08-31  ww    v1.0.0.1  modify to follow the function defintion of V1.1
2012-02-15  ww    v1.1.0.1  modify to follow the function defintion of V1.2
*******************************************************************************/
#ifndef _SPMSGID_H_
#define _SPMSGID_H_


 #define __packet
 #pragma pack(push) //保存对齐状态 

#pragma pack(1)//设定为1字节对齐 

/*
// #ifdef __cplusplus
// extern "C" {
// #endif
*/
#include "typedef.h"
#include "sysmsgdef.h"
//#include "adrvfirm.h"

/*
// #if  !defined(SLEEP_APART) && !defined(SLEEP_ALL)
// #define SLEEP_APART {SLEEP_APART, SLEEP_ALL}
// #endif
*/

#define SP_VOICE           0
#define SP_DATA_TYPE1      2
#define SP_DATA_TYPE2      3
#define SP_DATA_TYPE3      4
#define SP_STATUSCHECK     7
#define SP_RC              8
#define SP_ALARM           9

#define SP_DATA_TYPE       3         /* 2: DATA1  3: DATA2  4: DATA3*/

#define SP_BUFF_LENGTH    200

#define SP_VOLUME 0
#define SP_VOX 0
#define SP_BATLIFE 1
#define SP_RSSI   2
#define SP_BUSY   3
#define SP_WHISPER  3
#define SP_ALARMNUM  3           /*max alarm index*/

#ifdef LT_PRJNUM_6701
#define SP_AUDIONUM  110          /*max audio index*/
#else
#define SP_AUDIONUM  10          /*max audio index*/
#endif

#define SP_CHANUM  16            /*max channel number*/
#define SP_ZONENUM  16            /*max zone number*/
#define SP_SCANNUM  32            /*max scanlist number*/
#define SP_BUSYNUM  10           /* calculated busy thrd max number*/
#define SP_RSSINUM  5             /* calculated rssi thrd max number*/
#define SP_BTLNUM  4             /* calculated btl number*/
#define SP_CNTNUM  200            /*max contact number*/
#define SP_GRPNUM  32            /*max grouplist number*/
#define SP_TXTNUM  50            /*max text number*/


#define SP_WAIT_CHCHG  100    /* wait for sch chchg cnf*/
#define SP_WAIT_STOP   400     /*wait for ccl stop cnf*/
#define SP_WAIT_VOICE  600    /*wait for ccl voice ind*/

#define SP_RSSI_A_TH    0
#define SP_RSSI_D_TH    0
#define SP_BUSY_D_OPEN     390
#define SP_BUSY_D_CLOSE    472

#ifdef  LT_PRJNUM_6700
#define SP_VOL_LOW    684
#define SP_VOL_HIGH   948
#else
#define SP_VOL_LOW    0
#define SP_VOL_HIGH   1024
#endif

#define SP_FAIL 0
#define SP_SUCCESS 1

#define SP_BUSY_LIMIT       3
#define SP_LOWBAT_LIMIT     7
#define SP_MEM_FAIL         8


#define SP_OFF 0
#define SP_ON 1

#define SP_NORMAL 0
#define SP_PRIORITY 1

#define SP_PC    1
#define SP_MT    2

#define SP_TA_ON  1
#define SP_TA_OFF 2

#define SP_BASED_SERVICE      0
#define SP_BASED_TIME         1

#define SP_IDLE            0
#define SP_RSYNC           1
#define SP_CONECT          2
#define SP_TX              3
#define SP_RX              4

#define SP_PC    1
#define SP_MT    2

#define SP_TA_ON  1
#define SP_TA_OFF 2

#define SP_BASED_SERVICE      0
#define SP_BASED_TIME         1

#define SP_DELETE 0
#define SP_ADD 1

#define SP_IMPOLITE       0
#define SP_POLITE         1
#define SP_POLITE_CC      2
#define SP_POLITE_GRP     3
#define SP_UNDEF             255

#define SP_UNSYNC        1
#define SP_SYNC          2

#define SP_SERVICE        1
#define SP_PREFRAME       2

#define SP_SYSGLB     0
#define SP_CHALCALGLB 1

#define SP_CONNECT      1
#define SP_READ         2
#define SP_WRITE        3

#define SP_SYS_ON_READY       63
#define SP_SYS_OFF_READY      56
#define SP_SYS_EXIT_READY     59
#define SP_SYS_SLEEP_READY  33

#define SP_MMI_READY      32
#define SP_CCL_READY       16
#define SP_SCH_READY      8
#define SP_ZSPDD_READY  4
#define SP_DLL_READY       2
#define SP_PL_READY         1

#define SP_LCD_READY       2
#define SP_KEY_READY       1

#define SP_SLEEP_READY    1

#define SP_UNCALC  0
#define SP_CALC    1

#define SP_CALLED   0
#define SP_CALLING 1

#define SP_UNASS   0
#define SP_ASS     1
#define SP_DIG   1
#define SP_ANA  2
#define SP_DIG_ANA   3

#define SP_DIRECT_MODE   0
#define SP_REPEATER_MODE 1

#define SP_RX_START      0
#define SP_RX_STOP       1
#define SP_RX_RFOPEN     2

#define SP_SCAN_ALL   1
#define SP_SCAN_LIST  2

#define SP_T_DELAY  10

#define SP_PEER_TO_PEER   1
#define SP_GROUP_CALL     2
    /*System setting WF Parameters*/
typedef   struct
{
        uint8           serialnum[16];    /*product serial number*/
        uint8           protype[8];         /*product type*/
        uint16         freqscope;         /*freqency scope
                                                       1:136-150
                                                       2:150-174
                                                       3:400-470
                                                       4:400-425
                                                   */
        uint32         swver;              /*software version*/
        uint32         dataver;           /*data version*/
        uint8           wkeyon;            /*whether write wf key is on  0:off 1:on*/
        uint8           rkeyon;              /*whether read wf key is on 0:off 1:on*/
        uint32         wkey;                 /* write wf key */
        uint32         rkey;                    /* read wf key */
        uint8           pwson;                /*whether the powersave is on  0:off 1:on*/
        uint8           pws;                   /* the powersave time scale
                                                     0:invalid
                                                     1: 1:1
                                                     2: 1:2
                                                     3: 1:4
                                                    */
        uint8           tcomm;               /*this timer timeup,call is forbidden*/
        uint8           pretot;                /*pre-alert when tcomm is close*/
        uint8           trekey;               /*this timer timeup,key can been pressed again
                                                      after call is forbidden*/
        uint8           treset;                /*this timer timeup,system is reset after call is forbidden */
        uint8           whisperen;             /*whether the whisper is on  0:off 1:on */
        uint8           noisethd;               /* noise thrd*/
        uint8          voxlel;                     /*vox function level  0:off   1-5 : 1-5*/
        uint8          voxtime;                   /*vox delay time */
        uint8          voxcancel;                /*whether the vox is canceled after call which is actived by PTT finished*/
        uint8          voxforbid;                  /*whether the vox is forbidden when receiving*/
        uint8          keylock;                     /*whether the keylock is on   0:off 1-6:5-30s*/
        uint8          chanlock;                    /*whether the channellock function is on */
        uint8          vibrate;                     /*whether vibrate is on */
        uint8          toneen;                     /*whether audio is on */
        uint8          callouttone;              /*whether audio is on when make calls*/
        uint8          callintone;                /*whether audio is on when receive call*/
        uint8          smstone;                     /*whether audio is on when receive sms*/
        uint8          lowbattone;            /*whether audio is on when battery is low-power*/
        uint8          keytone;                 /*whether audio is on when key is pressed*/
        uint8          lightnoteen;	          /*led status*/
        uint8          backlightnote;          /*backlight status*/
        uint8          backlightdelay;         /*backlight delay*/
        uint8          idledisp;                   /* idle display*/
        uint8          dateen;                    /*whether date&clock is showed */
        uint32        msid;                        /*MS ID*/
        uint8          wildcarden;              /*whether *is supported*/
        uint8          manualdialen;          /*whether manual dial is supported*/
        uint8          pretxdelay;             /*delay before tx*/
        uint8          grphold;                 /*hold time when group call*/
        uint8          peerhold;               /*hold time when peer call*/
        uint8          kilen;                     /*whether can kill others*/
        uint8          activen;                 /*whether can active others*/
        uint8          rcen;                     /*whether can be killed&actived*/
        uint8          callallen;              /*whether can call all*/
        uint8          callgrpen;             /*whether can call group*/
        uint8          crtconten;            /*whether can create new contact*/
        uint8          editsmsen;           /*whether can edit sms*/
        uint8          emename[16];       /* emegency system name*/
        uint8          emetype;              /*emegency type 0: mute 1: alert 2:normal 3:secret without receive 4:secret with receive*/
        uint8          ememode;           /*emegency mode 0:eme  code 1:code + call 2: auto call*/
        uint16        emechannel;      /*change to the channel when make emegency call*/
        uint8          alarmtime;            /*alarm time*/
        uint8          emecalltime;            /*emegency call time*/
        uint8          michold;              /*duration of autocall  when mode is 2*/
        uint8          revhold;               /*duration of receive  when call is finished*/
        uint32        emeid;                  /*eme call ID*/
        uint8          powlelmenu;       /*whether powerlevel  menu is on */
        uint8          keylockmenu;       /*whether keylock  menu is on */
        uint8          laumenu;              /*whether language setting menu is on */
        uint8          backlightmenu;      /*whether backlight setting menu is on */
        uint8          ledmenu;                 /*whether led setting menu is on */
        uint8          tonemenu;                /*whether audio setting menu is on */
        uint8          lumcmenu;               /*whether luminace menu is on*/
        uint8          smsmenu;                /*whether sms menu is on */
        uint8          noisemenu;              /*whether noise setting menu is on */
        uint8          pbdelmenu;              /*whether delete contact in phone book is on */
        uint8          scanmenu;                /*whether scan menu is on*/
        uint8          scancheckmenu;              /*whether check channel in scan list menu is on */
        uint8          scanaddtmpmenu;              /*whether add channel temp in scan list menu is on */
        uint8          scandeltmpmenu;              /*whether delete channel temp in scan list menu is on */
        uint8          scanaddmenu;              /*whether add channel in scan list menu is on */
        uint8          scandelmenu;              /*whether delete channel in scan list menu is on */
        uint8          voxmenu;                    /*whether vox setting menu is on */
        uint8          whispermenu;              /*whether whisper setting menu is on */
        uint8          vibratemenu;              /*whether vibrate setting menu is on */
        uint8          datemenu;                   /*whether date setting menu is on */
        uint8          mainmenuen;              /*whether main menu setting menu is on */
        uint8          pwsmenu;                   /*whether powersave setting menu is on */
        uint8          talkaroundmenu;        /*whether talk around setting menu is on*/
        uint8          speechenhanmenu;        /*whether speech enhance setting menu is on*/
        uint8          voicekeymenu;        /*whether voice key setting menu is on*/
        uint8          pf1funcs;             /*PF1's function when short pressed*/
        uint8          pf1funcl;             /*PF1's function when long pressed*/
        uint8          pf2funcs;             /*PF2's function when short pressed*/
        uint8          pf2funcl;             /*PF2's function when long pressed*/
        uint8          topfuncs;            /*TOP's function when short pressed*/
        uint8          topfuncl;            /*TOP's function when long pressed*/
        uint8          menufunc;         /*Menu's function when long pressed*/
        uint8          backfunc;          /*back's function when long pressed*/
        uint8          inputfuncs;         /*input's function when short pressed*/
        uint8          inputfuncl;         /*input's function when long pressed*/
        uint8          switchfunc;         /*on&off's function when short pressed*/
    } gSYSSet_S;

    /*System zone WF  Parameters*/
typedef   struct
{
    uint8           zonename[16];    /*channel name */
    uint8           zonechnum;      /*channel annalog signal number when send */
    uint8           crev;                /*reserve*/
} gZONE_S;

/*System channel WF  Parameters*/
typedef   struct
{
    uint8           chass;            /*whether channel is assgined*/
    uint8           scanlist;         /*current channel's scanlist*/
    uint8           chmode;          /*channel mode 0: direct mode 1:repeater mode*/
    uint8           chstatus;         /*channel status 0: forbid talk around 1:allow talk around*/
    uint8           chname[16];    /*channel name */
    uint32          txfreq;             /*channel freqency when send */
    uint32          rxfreq;             /*channel freqency when receive */
    uint8           powerlel;         /*channel power level */
    uint8           interval;           /*channel interval */
    uint16          speechenhanen;/*speech enhance enable 0: off 1-4*/
    uint8           ctcphase;         /* phase change in analog signal  for tail flag */
    uint8           busylock;         /* whether channel should been locked when busy */
    uint8           txanatype;      /*channel annalog signal type when send */
    uint8           txananum;      /*channel annalog signal number when send */
    uint8           rxanatype;      /*channel annalog signal type when receive  */
    uint8           rxananum;      /*channel annalog signal number when receive  */
    uint8           revgrp;          /*assigned receive group*/
    uint8           resid;           /*reserve contact when send*/
    uint8           polite;           /*polite mannner */
    uint8           voikeyon;       /*whether the voice encrypted is on */
    uint32         voikey;          /* the voice encrypted key */
} gCHANNEL_S;

/*System scan WF  Parameters*/
typedef   struct
{
    uint8           listnum;           /*scan list number */
    uint8           listass;            /* whether the scanlist is assigned*/
    uint8           scaname[16];       /*scanlist name */
    uint8           scanmode;           /*scan mode */
    uint8           scanchnum;          /*the number of channel  included in scanlist */
    uint8           chflag[32];            /*channel  included in scanlist  */
    uint8           tscanon;            /*scan hold on timer*/
    uint8           txmode;            /* channel change mode when ptt is pressed */
    uint16          txchannel;      /*channel nuber when channel change mode is assigned*/
    uint8           revdelay;        /*duration of receive  call ,when timer timeup ,scan continue*/
    uint8           senddelay;     /*duration of make  call ,when timer timeup ,scan continue*/
    uint8           callback;      /*whether callback is on current channel*/
    uint8           nres;          /*reserve*/
} gSCAN_S;

/*System contact WF  Parameters*/
typedef   struct
{
    uint8          contctnum;      /*contact number */
    uint8          type;               /*contact type  0: null 1: peer 2: group call 3: call all*/
    uint8          coname[16];     /*contact name*/
    uint32        callid;          /*contact call ID*/
} gCONTACT_S;

/*System group WF  Parameters*/
typedef   struct
{
    uint8          grpnum;        /*group number*/
    uint8          memnum;       /*number of group member */
    uint8          grpname[16];   /*group name */
    uint8          connum[16];    /*contact included grouplist*/
} gGROUP_S;

/*System text WF  Parameters*/
typedef   struct
{
    uint8          txtnum;        /*text number*/
    uint8          txtlength;     /*text length 0:null */
    uint8          txtcontent[160];    /*text content*/
} gTEXT_S;

/*System  all WF Parameters*/
typedef   struct
{
    gSYSSet_S             gSYSSetWf;              /*system setting wf parameter*/
    gZONE_S               gZONEWf[16];
    gCHANNEL_S          gCHANNELWf[16][16];     /*channel wf parameter */
    gSCAN_S                gSCANWf[32];            /*scan wf parameter*/
    gCONTACT_S          gCONTACTWf[200];    /*contact wf parameter*/
    gGROUP_S              gGROUPWf[32];       /*group wf parameter*/
    gTEXT_S                 gTEXTWf[50];         /*text wf parameter*/
} gSYSWf_S;

/*System  global  Parameters*/
typedef   struct
{
    uint8          gSYSWorkmode;     /*system mode 0:res 1:digtal 2: analog 3:dual*/
    uint8          gSYSRCStatus;     /*whether ms is active*/
    uint32        gSYSMtKey;        /*MT key*/
    uint8          gSYSMicGainHigh;
    uint8          gSYSMicGainLow;
    uint16        gSYSSpklev[15];
    uint8          gSYSSpkGain[16];
    uint16        gSYSBtlLev[4];
    uint16        gSYSBtlDiff;
    uint16        gSYSVoxLev[5];
    uint32        gSYSTxCC[16][16];     /*channel tx color code  */
    uint32        gSYSRxCC[16][16];    /*channel rx color code  */
    uint8          gSYSChCalStatus;
    uint8          gSYSVolLevel;
    uint8          gSYSCurZoneNum;
    uint8          gSYSCurChanNum;
    uint8          gSYSPerSZoneNum;
    uint8          gSYSPerSChanNum;
    uint8          gSYSZoneNum;
    uint8          gSYSreseve;
} gSYSGlb_S;

/*SP mt channel  Parameters */
typedef   struct
{
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
    uint16         RxTV;
    uint16         NoiseThrOpen[SP_BUSYNUM];   /*NoiseThrOpen[0] is null */
    uint16         NoiseThrclose[SP_BUSYNUM];   /*NoiseThrclose[0] is null */
    uint16         RssiThr[5];
} SpMtChaCalc_S;

/*System  all channel calc Parameters*/
typedef   struct
{
    SpMtChaCalc_S          gCHCalc[16][16];     /*channel wf parameter */
} gSYSChCalc_S;

    extern gSYSWf_S      gSYSWf;
    extern gSYSGlb_S     gSYSGlb;
    extern gSYSChCalc_S   gSYSChCalc;
    extern uint8         gSPScanOn;

    extern void32 SPChannelChangeBack(uint8 chanum);
    extern void32 SPMicChangeBack(uint32 micstatus);
    extern void32 SPChargeBack(uint32 btlstatus);
    extern void32 SPStopBack(void32);
    extern uint32 SPGetCheckTBtl(void32);

#define SP_MODULE_ARMDD             (SYS_MODULE_ARMDD << 8)
#define SP_MODULE_OSE               (SYS_MODULE_OSE << 8)
#define SP_MODULE_SP                (SYS_MODULE_SP << 8)
#define SP_MODULE_MMI               (SYS_MODULE_MMI << 8)
#define SP_MODULE_CCL               (SYS_MODULE_CCL << 8)
#define SP_MODULE_ZSPDD             (SYS_MODULE_ZSPDD << 8)
#define SP_MODULE_SCH               (SYS_MODULE_SCH << 8)
#define SP_MODULE_PL                (SYS_MODULE_PL << 8)
#define SP_MODULE_DLL               (SYS_MODULE_DLL << 8)
#define SP_MODULE_VOCODER           (SYS_MODULE_VOCODER << 8)
#define SP_MODULE_MTSW              (SYS_MODULE_MTSW<< 8)
#define SP_MODULE_DEBUGSW           (SYS_MODULE_DEBUGSW << 8)
#define SP_MODULE_PCSW              (SYS_MODULE_PCSW << 8)
#pragma pack(pop)//恢复对齐状态
#endif
