/*
 * $QNXLicenseC:
 * Copyright 2011-2012, QNX Software Systems.
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

#ifndef __SMBUS_EC_H_INCLUDED
#define __SMBUS_EC_H_INCLUDED

#include "proto.h"

/****************************************************************/
/* Embedded Controller registers, i/o aperture                  */
/****************************************************************/

#define EC_DATA                                 0x00000062
#define EC_SC                                   0x00000066
#define     EC_SC_SMI_EVT                       0x00000040
#define     EC_SC_SCI_EVT                       0x00000020
#define     EC_SC_BURST                         0x00000010
#define     EC_SC_CMD                           0x00000008
#define     EC_SC_IBF                           0x00000002
#define     EC_SC_OBF                           0x00000001

/****************************************************************/
/* Embedded Controller command set, ACPI specification, ch 12.3 */
/****************************************************************/

/* Read Embedded Controller */
#define RD_EC                                   0x00000080

/* Write Embedded Controller */
#define WR_EC                                   0x00000081

/* Burst Enable Embedded Controller */
#define BE_EC                                   0x00000082

/* Burst Disable Embedded Controller */
#define BD_EC                                   0x00000083

/* Query Embedded Contoller */
#define QR_EC                                   0x00000084

/****************************************************************/
/* SMBus Controller registers, i/o aperture                     */
/****************************************************************/

#define SMB_PRTCL                               0x00000000
#define     SMB_PRTCL_PEC_EN                    0x00000080
#define     SMB_PRTCL_CONTROLLER_NOT_IN_USE     0x00000000
#define     SMB_PRTCL_RESERVED                  0x00000001
#define     SMB_PRTCL_WRITE_QUICK_COMMAND       0x00000002
#define     SMB_PRTCL_READ_QUICK_COMMAND        0x00000003
#define     SMB_PRTCL_SEND_BYTE                 0x00000004
#define     SMB_PRTCL_RECEIVE_BYTE              0x00000005
#define     SMB_PRTCL_WRITE_BYTE                0x00000006
#define     SMB_PRTCL_READ_BYTE                 0x00000007
#define     SMB_PRTCL_WRITE_WORD                0x00000008
#define     SMB_PRTCL_READ_WORD                 0x00000009
#define     SMB_PRTCL_WRITE_BLOCK               0x0000000A
#define     SMB_PRTCL_READ_BLOCK                0x0000000B
#define     SMB_PRTCL_PROCESS_CALL              0x0000000C
#define     SMB_PRTCL_BLOCK_PROCESS_CALL        0x0000000D

#define SMB_STS                                 0x00000001
#define     SMB_STS_DONE                        0x00000080
#define     SMB_STS_ALRM                        0x00000040
#define     SMB_STS_RES                         0x00000020
#define     SMB_STS_STATUS_MASK                 0x0000001F
#define     SMB_STATUS_OK                       0x00000000
#define     SMB_STATUS_UKNOWN_FAILURE           0x00000007
#define     SMB_STATUS_DEVICE_ADDRESS_NOT_ACK   0x00000010
#define     SMB_STATUS_DEVICE_ERROR             0x00000011
#define     SMB_STATUS_DEVICE_CMD_ACCESS_DEN    0x00000012
#define     SMB_STATUS_SMBUS_UNKNOWN_ERROR      0x00000013
#define     SMB_STATUS_DEVICE_ADDR_ACCESS_DEN   0x00000017
#define     SMB_STATUS_SMBUS_TIMEOUT            0x00000018
#define     SMB_STATUS_HOST_UNSUPP_PROTOCOL     0x00000019
#define     SMB_STATUS_SMBUS_BUSY               0x0000001A
#define     SMB_STATUS_SMBUS_PEC_ERROR          0x0000001F

#define SMB_ADDR                                0x00000002

#define SMB_CMD                                 0x00000003

#define SMB_DATA0                               0x00000004
#define SMB_DATA1                               0x00000005
#define SMB_DATA2                               0x00000006
#define SMB_DATA3                               0x00000007
#define SMB_DATA4                               0x00000008
#define SMB_DATA5                               0x00000009
#define SMB_DATA6                               0x0000000A
#define SMB_DATA7                               0x0000000B
#define SMB_DATA8                               0x0000000C
#define SMB_DATA9                               0x0000000D
#define SMB_DATA10                              0x0000000E
#define SMB_DATA11                              0x0000000F
#define SMB_DATA12                              0x00000010
#define SMB_DATA13                              0x00000011
#define SMB_DATA14                              0x00000012
#define SMB_DATA15                              0x00000013
#define SMB_DATA16                              0x00000014
#define SMB_DATA17                              0x00000015
#define SMB_DATA18                              0x00000016
#define SMB_DATA19                              0x00000017
#define SMB_DATA20                              0x00000018
#define SMB_DATA21                              0x00000019
#define SMB_DATA22                              0x0000001A
#define SMB_DATA23                              0x0000001B
#define SMB_DATA24                              0x0000001C
#define SMB_DATA25                              0x0000001D
#define SMB_DATA26                              0x0000001E
#define SMB_DATA27                              0x0000001F
#define SMB_DATA28                              0x00000020
#define SMB_DATA29                              0x00000021
#define SMB_DATA30                              0x00000022
#define SMB_DATA31                              0x00000023

#define SMB_BCNT                                0x00000024

#define SMB_ALRM_ADDR                           0x00000025
#define SMB_ALRM_DATA0                          0x00000026
#define SMB_ALRM_DATA1                          0x00000027

/****************************************************************/
/* Function prototypes                                          */
/****************************************************************/

void write_ec_sc(ec_dev_t* dev, uint8_t data);
uint8_t read_ec_sc(ec_dev_t* dev);
void write_ec_data(ec_dev_t* dev, uint8_t data);
uint8_t read_ec_data(ec_dev_t* dev);

void write_ec(ec_dev_t* dev, uint8_t reg, uint8_t data);
uint8_t read_ec(ec_dev_t* dev, uint8_t reg);

void write_smbus(ec_dev_t* dev, uint8_t reg, uint8_t data);
uint8_t read_smbus(ec_dev_t* dev, uint8_t reg);

#endif /* __SMBUS_EC_H_INCLUDED */
