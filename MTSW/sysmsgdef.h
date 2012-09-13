
/*******************************************************************************
File Name: sysmsgdef.h

Description: This file contains the message definition.

Copyright:
    Copyright(c) 2011 LinkTop technologies Co., LTD. All rights reserved.

Change History:
2011-03-25  wangjl    v0.0.0.1  Create
2011-05-06  wangjl    v0.0.1.0  Check and release after function test
2011-09-05  wangjl    v1.0.0.1  modify to follow the function defintion of V1.1
2012-02-15  wengt     v1.1.0.1  modify to follow the function defintion of V1.2

PS: only reserve useful msgdefinitions
*******************************************************************************/
#ifndef _MESSAGEDEF_H
#define _MESSAGEDEF_H

/**************************************************************************
 *                        Signaling Message ID                         *
 **************************************************************************/

/*******SP&PC****************/
#define     PC_SP_CONNECT_REQ                       4495
#define     SP_PC_CONNECT_CNF                       4496

#define     PC_SP_KEY_REQ                           4497
#define     SP_PC_KEY_CNF                           4498

#define     PC_SP_UPDATEKEY_REQ                     4499
#define     SP_PC_UPDATEKEY_CNF                     4500

#define     PC_SP_EFFECT_REQ                        4501
#define     SP_PC_EFFECT_CNF                        4502

#define     PC_SP_WTSYSSET_REQ                      4503
#define     PC_SP_WTZONE_REQ                        4504
#define     PC_SP_WTCHANNEL_REQ                     4505
#define     PC_SP_WTSCAN_REQ                        4506
#define     PC_SP_WTCONTACT_REQ                     4507
#define     PC_SP_WTGROUP_REQ                       4508
#define     PC_SP_WTTEXT_REQ                        4509
#define     SP_PC_WRITE_CNF                         4510

#define     PC_SP_READ_REQ                          4511
#define     SP_PC_RDSYSSET_CNF                      4512
#define     SP_PC_RDZONE_CNF                        4513
#define     SP_PC_RDCHANNEL_CNF                     4514
#define     SP_PC_RDSCAN_CNF                        4515
#define     SP_PC_RDCONTACT_CNF                     4516
#define     SP_PC_RDGROUP_CNF                       4517
#define     SP_PC_RDTEXT_CNF                        4518

/*******SP&MT****************/
#define     MT_SP_CONNECT_REQ                       4550
#define     SP_MT_CONNECT_CNF                       4551

#define     MT_SP_READ_REQ                          4552
#define     SP_MT_READ_CNF                          4553

#define     MT_SP_WRITE_REQ                         4554
#define     SP_MT_WRITE_CNF                         4555

#define     MT_SP_ADCAL_REQ                         4556
#define     SP_MT_ADCAL_IND                         4557

#define     MT_SP_MAXVOL_REQ                        4558

#define     MT_SP_EFFECT_REQ                        4559
#define     SP_MT_EFFECT_CNF                        4560

/*******SCH&MT****************/
#define     MT_SCH_ANA_FREQCAL_REQ                  4600
#define     MT_SCH_ANA_FREQCAL_STOP                 4601

#define     MT_SCH_ANA_RXCAL_REQ                    4602
#define     MT_SCH_ANA_RXCAL_STOP                   4603

#define     MT_SCH_DIG_TXCAL_REQ                    4604
#define     MT_SCH_DIG_TXCAL_STOP                   4605

#define     MT_SCH_ANA_TXCAL_REQ                    4606
#define     MT_SCH_ANA_TXCAL_STOP                   4607

#define     MT_SCH_DIG_RXCAL_REQ                    4608
#define     MT_SCH_DIG_RXCAL_STOP                   4609
#define     SCH_MT_DIG_RXCAL_RSP                    4610

/*******MMI&PC****************/
#define     PC_MMI_CHCHG_REQ                        6000
#define     MMI_PC_CHCHG_CNF                        6001

#define     PC_MMI_PTT_REQ                          6002
#define     MMI_PC_PTT_CNF                          6003

/**************************************************************************
 *                        Test Message ID                                  *
 **************************************************************************/
/*******SP&DEBUG****************/
#define      DEBUG_SP_TEST_REQ                      8200
#define      SP_DEBUG_GLB_REQ                       8201

#define      DEBUG_SP_RDGLB_REQ                     8202
#define      SP_DEBUG_RDSYSGLB_CNF                  8203
#define      SP_DEBUG_RDCHLCALGLB_CNF               8204


#define      DEBUG_SP_WTSYSGLB_REQ                  8205
#define      DEBUG_SP_WTCHLCALGLB_REQ               8206
#define      SP_DEBUG_WTGLB_CNF                     8207


/*******CCL&DEBUG****************/
#define     DEBUG_CCL_TEST_REQ                      8250
#define     CCL_DEBUG_STAT_IND                      8251
#define     CCL_DEBUG_CALL_IND                      8252
#define     CCL_DEBUG_SYSMSG_IND                    8253

/*******DLL&DEBUG****************/
#define     DEBUG_DLL_TEST_REQ                      8300
#define     DLL_DEBUG_GLOBALVAR_IND                 8301
#define     DLL_DEBUG_DATAGET_IND                   8302

/*******PL&DEBUG****************/
#define     DEBUG_PL_TEST_REQ                       8350
#define     PL_DEBUG_TEST1_IND                      8351
#define     PL_DEBUG_TEST2_IND                      8352


/*******SCH&DEBUG****************/
#define     DEBUG_SCH_TEST_REQ                      8450
#define     SCH_DEBUG_TEST_INFO                     8451

#define     DEBUG_SCH_CPU_RATE_SET                  8452
#define     DEBUG_SCH_CPU_RATE_REQ                  8453
#define     SCH_DEBUG_CPU_RATE_RSP                  8454

#define     DEBUG_SCH_MEM_STA_REQ                   8455
#define     SCH_DEBUG_MEM_STA_RSP                   8456


/*******ZSPDD&DEBUG****************/
#define     DEBUG_ZSPDD_DUMP_REQ                    8505
#define     ZSPDD_DEBUG_DUMP_IND                    8506

#define     DEBUG_ZSPDD_TEST_REQ                    8507
#define     ZSPDD_DEBUG_TEST_DATA                   8508


/*******ARMDD&DEBUG****************/
#define     DEBUG_ARMDD_READREG_REQ                 8560
#define     ARMDD_DEBUG_READREG_RSP                 8561

/*******OSE&DEBUG****************/
#define     DEBUG_OSE_TEST_REQ                      8570

/*******ZSPDD&ARMDD****************/
#define     ZSPDD_ARMDD_MSG_FAIL_IND                8600

/**************************************************************************
 *                        MODULE ID                        *
 **************************************************************************/

#define SYS_MODULE_ARMDD             1
#define SYS_MODULE_OSE               2
#define SYS_MODULE_SP                3
#define SYS_MODULE_MMI               4
#define SYS_MODULE_CCL               5
#define SYS_MODULE_ZSPDD             21
#define SYS_MODULE_SCH               22
#define SYS_MODULE_PL                23
#define SYS_MODULE_DLL               24
#define SYS_MODULE_VOCODER           25
#define SYS_MODULE_MTSW              41
#define SYS_MODULE_DEBUGSW           42
#define SYS_MODULE_PCSW              43

/**************************************************************************
 *                        Message Type                        *
 **************************************************************************/
#define SIGNALING         1
#define MAINTAINANCE      2
#define TESTING           3


#endif
