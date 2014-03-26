/*
 * $QNXLicenseC:
 * Copyright 2011-2012, QNX Software Systems.
 * Copyright 2011-2012, Mike Gorchak.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include <inttypes.h>

/****************************************************************************/
/* Smart Battery devices                                                    */
/****************************************************************************/

/* 7 bit slave addresses of smart battery devices */
#define SB_SMART_CHARGER_ADDRESS                            0x09
#define SB_SMART_SELECTOR_ADDRESS                           0x0A
#define SB_SMART_MANAGER_ADDRESS                            0x0A
#define SB_SMART_BATTERY_ADDRESS                            0x0B

/****************************************************************************/
/* Smart Battery Charger definitions                                        */
/****************************************************************************/

/* READ WORD operation, returns uint16_t */
#define SC_CHARGER_SPEC_INFO                                0x00000011
#define     SC_CSI_CHARGER_SPEC_MASK                        0x0000000F
#define     SC_CSI_CHARGER_SPEC_SHIFT                       0
#define         SC_CSI_CHARGER_VERSION_1_0                  0x00000001
#define         SC_CSI_CHARGER_VERSION_1_1                  0x00000002
#define         SC_CSI_CHARGER_VERSION_1_1_PEC              0x00000003
#define     SC_CSI_SELECTOR_SUPPORT                         0x00000010

/* READ WORD operation, returns uint16_t */
#define SC_CHARGER_STATUS                                   0x00000013
#define    SC_CS_CHARGE_INHIBITED                           0x00000001
#define    SC_CS_POLLING_ENABLED                            0x00000002
#define    SC_CS_VOLTAGE_NOTREG                             0x00000004
#define    SC_CS_CURRENT_NOTREG                             0x00000008
#define    SC_CS_LEVEL_2                                    0x00000010
#define    SC_CS_LEVEL_3                                    0x00000030
#define    SC_CS_CURRENT_OR                                 0x00000040
#define    SC_CS_VOLTAGE_OR                                 0x00000080
#define    SC_CS_RES_OR                                     0x00000100
#define    SC_CS_RES_COLD                                   0x00000200
#define    SC_CS_RES_HOT                                    0x00000400
#define    SC_CS_RES_UR                                     0x00000800
#define    SC_CS_ALARM_INHIBITED                            0x00001000
#define    SC_CS_POWER_FAIL                                 0x00002000
#define    SC_CS_BATTERY_PRESENT                            0x00004000
#define    SC_CS_AC_PRESENT                                 0x00008000

/****************************************************************************/
/* Smart Battery Manager definitions                                        */
/****************************************************************************/

/* READ/WRITE WORD operation, returns/takes uint16_t */
#define SM_BATTERY_SYSTEM_STATE                             0x00000001
#define     SM_BSS_CONNECTED_TO_SMB_MASK                    0x0000F000
#define     SM_BSS_CONNECTED_TO_SMB_SHIFT                   12
#define     SM_BSS_POWERED_BY_MASK                          0x00000F00
#define     SM_BSS_POWERED_BY_SHIFT                         8
#define     SM_BSS_CHARGING_MASK                            0x000000F0
#define     SM_BSS_CHARGING_SHIFT                           4
#define     SM_BSS_PRESENT_MASK                             0x0000000F
#define     SM_BSS_PRESENT_SHIFT                            0
#define     SM_BSS_BATTERY_A                                0x00000001
#define     SM_BSS_BATTERY_B                                0x00000002
#define     SM_BSS_BATTERY_C                                0x00000004
#define     SM_BSS_BATTERY_D                                0x00000008
#define     SM_BSS_BATTERY_COMPOSITE                        0x0000000F

/* READ/WRITE WORD operation, returns/takes uint16_t */
#define SM_BATTERY_SYSTEM_STATE_CONT                        0x00000002
#define     SM_BSSC_AC_PRESENT                              0x00000001
#define     SM_BSSC_POWER_NOT_GOOD                          0x00000002
#define     SM_BSSC_CALIBRATE_REQUEST_SUPPORT               0x00000004
#define     SM_BSSC_CALIBRATE_REQUEST                       0x00000008
#define     SM_BSSC_CHARGING_INHIBIT                        0x00000010
#define     SM_BSSC_CHARGER_POR                             0x00000020
#define     SM_BSSC_CALIBRATE                               0x00000040
#define     SM_BSSC_CALIBRATE_A                             0x00000100
#define     SM_BSSC_CALIBRATE_B                             0x00000200
#define     SM_BSSC_CALIBRATE_C                             0x00000400
#define     SM_BSSC_CALIBRATE_D                             0x00000800

/* READ WORD operation, returns uint16_t */
#define SM_BATTERY_SYSTEM_INFO                              0x00000004
#define     SM_BSI_IPSCALE_MASK                             0x0000F000
#define     SM_BSI_IPSCALE_SHIFT                            12
#define     SM_BSI_VSCALE_MASK                              0x00000F00
#define     SM_BSI_VSCALE_SHIFT                             8
#define     SM_BSI_BATTERY_SYSTEM_REVISION_MASK             0x000000F0
#define     SM_BSI_BATTERY_SYSTEM_REVISION_SHIFT            4
#define     SM_BSI_BATTERY_VERSION_1_0                      0x00000080
#define     SM_BSI_BATTERY_VERSION_1_0_PEC                  0x00000090
#define     SM_BSI_SUPPORTED_BATTERIES_MASK                 0x0000000F
#define     SM_BSI_SUPPORTED_BATTERIES_SHIFT                0

/****************************************************************************/
/* Smart Battery Selector definitions                                       */
/****************************************************************************/

/* READ/WRITE WORD operation, returns/takes uint16_t */
#define SS_SELECTOR_STATE                                   0x00000001
#define     SS_SS_CONNECTED_TO_SMB_MASK                     0x0000F000
#define     SS_SS_CONNECTED_TO_SMB_SHIFT                    12
#define     SS_SS_POWERED_BY_MASK                           0x00000F00
#define     SS_SS_POWERED_BY_SHIFT                          8
#define     SS_SS_CHARGE_MASK                               0x000000F0
#define     SS_SS_CHARGE_SHIFT                              4
#define     SS_SS_PRESENT_MASK                              0x0000000F
#define     SS_SS_PRESENT_SHIFT                             0
#define     SS_SS_BATTERY_A                                 0x00000001
#define     SS_SS_BATTERY_B                                 0x00000002
#define     SS_SS_BATTERY_C                                 0x00000004
#define     SS_SS_BATTERY_D                                 0x00000008
#define     SS_SS_BATTERY_COMPOSITE                         0x0000000F

/* READ/WRITE WORD operation, returns/takes uint16_t */
#define SS_SELECTOR_PRESETS                                 0x00000002
#define     SS_SP_USE_NEXT_MASK                             0x000000F0
#define     SS_SP_USE_NEXT_SHIFT                            4
#define     SS_SP_OK_TO_USE_MASK                            0x0000000F
#define     SS_SP_OK_TO_USE_SHIFT                           0

/* READ WORD operation, returns uint16_t */
#define SS_SELECTOR_INFO                                    0x00000004
#define     SS_SI_CHARGING_INDICATOR                        0x00000100
#define     SS_SI_SELECTOR_REVISION_MASK                    0x000000F0
#define     SS_SI_SELECTOR_REVISION_SHIFT                   4
#define     SS_SI_SELECTOR_VERSION_1_0                      0x00000010
#define     SS_SI_SELECTOR_VERSION_1_1                      0x00000020
#define     SS_SI_SELECTOR_VERSION_1_1_PEC                  0x00000030
#define     SS_SI_SUPPORTED_BATTERIES_MASK                  0x0000000F
#define     SS_SI_SUPPORTED_BATTERIES_SHIFT                 0

/****************************************************************************/
/* Smart Battery definitions                                                */
/****************************************************************************/

/* READ/WRITE WORD operation, returns/takes uint16_t */
#define SB_MANUFACTURER_ACCESS                              0x00000000

/* READ/WRITE WORD operation, returns/takes int16_t */
#define SB_REMAINING_CAPACITY_ALARM                         0x00000001

/* READ/WRITE WORD operation, returns/takes int16_t */
#define SB_REMAINING_TIME_ALARM                             0x00000002

/* READ/WRITE WORD operation, returns/takes uint16_t */
#define SB_BATTERY_MODE                                     0x00000003
#define     SB_BM_INTERNAL_CHARGE_CONTROLLER                0x00000001
#define     SB_BM_PRIMATY_BATTERY_SUPPORT                   0x00000002
#define     SM_BM_CONDITION_FLAG                            0x00000080
#define     SM_BM_CHARGE_CONTROLLER_ENABLED                 0x00000100
#define     SM_BM_PRIMARY_BATTERY                           0x00000200
#define     SM_BM_ALARM_MODE                                0x00002000
#define     SM_BM_CHARGER_MODE                              0x00004000
#define     SM_BM_CAPACITY_MODE                             0x00008000

/* READ/WRITE WORD operation, returns/takes int16_t */
#define SB_AT_RATE                                          0x00000004

/* READ WORD operation, returns uint16_t */
#define SB_AT_RATE_TIME_TO_FULL                             0x00000005

/* READ WORD operation, returns uint16_t */
#define SB_AT_RATE_TIME_TO_EMPTY                            0x00000006

/* READ WORD operation, returns boolean */
#define SB_AT_RATE_OK                                       0x00000007

/* READ WORD operation, returns uint16_t */
#define SB_TEMPERATURE                                      0x00000008

/* READ WORD operation, returns uint16_t */
#define SB_VOLTAGE                                          0x00000009

/* READ WORD operation, returns int16_t */
#define SB_CURRENT                                          0x0000000A

/* READ WORD operation, returns int16_t */
#define SB_AVERAGE_CURRENT                                  0x0000000B

/* READ WORD operation, returns uint16_t */
#define SB_MAX_ERROR                                        0x0000000C

/* READ WORD operation, returns uint16_t */
#define SB_RELATIVE_STATE_OF_CHARGE                         0x0000000D

/* READ WORD operation, returns uint16_t */
#define SB_ABSOLUTE_STATE_OF_CHARGE                         0x0000000E

/* READ WORD operation, returns uint16_t */
#define SB_REMAINING_CAPACITY                               0x0000000F

/* READ WORD operation, returns uint16_t */
#define SB_FULL_CHARGE_CAPACITY                             0x00000010

/* READ WORD operation, returns uint16_t */
#define SB_RUN_TIME_TO_EMPTY                                0x00000011

/* READ WORD operation, returns uint16_t */
#define SB_AVERAGE_TIME_TO_EMPTY                            0x00000012

/* READ WORD operation, returns uint16_t */
#define SB_AVERAGE_TIME_TO_FULL                             0x00000013

/* READ WORD operation, returns uint16_t */
#define SB_BATTERY_STATUS                                   0x00000016
#define     SB_BS_ALARM_MASK                                0x0000FF00
#define     SB_BS_OVER_CHARGED_ALARM                        0x00008000
#define     SB_BS_TERMINATE_CHARGE_ALARM                    0x00004000
#define     SB_BS_OVER_TEMP_ALARM                           0x00001000
#define     SB_BS_TERMINATE_DISCHARGE_ALARM                 0x00000800
#define     SB_BS_REMAINING_CAPACITY_ALARM                  0x00000200
#define     SB_BS_REMAINING_TIME_ALARM                      0x00000100
#define     SB_BS_STATUS_MASK                               0x000000F0
#define     SB_BS_INITIALIZED                               0x00000080
#define     SB_BS_DISCHARGING                               0x00000040
#define     SB_BS_FULLY_CHARGED                             0x00000020
#define     SB_BS_FULLY_DISCHARGED                          0x00000010
#define     SB_BS_ERROR_CODE_MASK                           0x0000000F
#define     SB_BS_ERROR_CODE_SHIFT                          0
#define     SB_BS_ERROR_OK                                  0x00000000
#define     SB_BS_ERROR_BUSY                                0x00000001
#define     SB_BS_ERROR_RESERVED_COMMAND                    0x00000002
#define     SB_BS_ERROR_UNSUPPORTED_COMMAND                 0x00000003
#define     SB_BS_ERROR_ACCESS_DENIED                       0x00000004
#define     SB_BS_ERROR_OVERFLOW_UNDERFLOW                  0x00000005
#define     SB_BS_ERROR_BAD_SIZE                            0x00000006
#define     SB_BS_ERROR_UNKNOWN_ERROR                       0x00000007

/* READ WORD operation, returns uint16_t */
#define SB_CYCLE_COUNT                                      0x00000017

/* READ WORD operation, returns uint16_t */
#define SB_DESIGN_CAPACITY                                  0x00000018

/* READ WORD operation, returns uint16_t */
#define SB_DESIGN_VOLTAGE                                   0x00000019

/* READ WORD operation, returns uint16_t */
#define SB_SPECIFICATION_INFO                               0x0000001A
#define     SB_SI_REVISION_MASK                             0x0000000F
#define     SB_SI_REVISION_SHIFT                            0
#define     SB_SI_VERSION_MASK                              0x000000F0
#define     SB_SI_VERSION_SHIFT                             4
#define     SB_SI_VERSION_1_0                               0x00000010
#define     SB_SI_VERSION_1_1                               0x00000020
#define     SB_SI_VERSION_1_1_PEC                           0x00000030
#define     SB_SI_VSCALE_MASK                               0x00000F00
#define     SB_SI_VSCALE_SHIFT                              8
#define     SB_SI_IPSCALE_MASK                              0x0000F000
#define     SB_SI_IPSCALE_SHIFT                             12

/* READ WORD operation, returns uint16_t */
#define SB_MANUFACTURE_DATE                                 0x0000001B

/* READ WORD operation, returns uint16_t */
#define SB_SERIAL_NUMBER                                    0x0000001C

/* READ BLOCK operation, returns ASCIIZ string */
#define SB_MANUFACTURER_NAME                                0x00000020

/* READ BLOCK operation, returns ASCIIZ string */
#define SB_DEVICE_NAME                                      0x00000021

/* READ BLOCK operation, returns ASCIIZ string */
#define SB_DEVICE_CHEMISTRY                                 0x00000022

/* READ BLOCK operation, returns ASCIIZ string */
#define SB_MANUFACTURER_DATA                                0x00000023
