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

#ifndef __SMBUS_ICH_H__
#define __SMBUS_ICH_H__

/****************************************************************/
/* PCI configuration space                                      */
/****************************************************************/

#define ICH_SMBUS_BAR0                                  0x00000010
#define ICH_SMBUS_BAR1                                  0x00000014
#define ICH_SMBUS_BASE                                  0x00000020
#define ICH_SMBUS_HOSTC                                 0x00000040
#define     ICH_SMBUS_HOSTC_SMBUS_HOST_EN               0x00000001
#define     ICH_SMBUS_HOSTC_SMBUS_SMI_EN                0x00000002
#define     ICH_SMBUS_HOSTC_I2C_EN                      0x00000004

/****************************************************************/
/* SMBus registers, i/o aperture                                */
/****************************************************************/

/* Host status */
#define ICH_SMBUS_HST_STS                               0x00000000
#define     NSHS_BYTE_DONE_STATUS                       0x00000080
#define     NSHS_INUSE_STS                              0x00000040
#define     NSHS_SMBALERT_STS                           0x00000020
#define     NSHS_FAILED                                 0x00000010
#define     NSHS_BUS_ERR                                0x00000008
#define     NSHS_DEV_ERR                                0x00000004
#define     NSHS_INTR                                   0x00000002
#define     NSHS_HOST_BUSY                              0x00000001

/* Host control */
#define ICH_SMBUS_HST_CNT                               0x00000002
#define     NSHC_PEC_EN                                 0x00000080
#define     NSHC_START                                  0x00000040
#define     NSHC_LAST_BYTE                              0x00000020
#define     NSHC_SMB_CMD_MASK                           0x0000001C
#define         NSHC_SMB_CMD_QUICK                      0x00000000
#define         NSHC_SMB_CMD_BYTE                       0x00000004
#define         NSHC_SMB_CMD_BYTE_DATA                  0x00000008
#define         NSHC_SMB_CMD_WORD_DATA                  0x0000000C
#define         NSHC_SMB_CMD_PROCESS_CALL               0x00000010
#define         NSHC_SMB_CMD_BLOCK                      0x00000014
#define         NSHC_SMB_CMD_I2C_READ                   0x00000018
#define         NSHC_SMB_CMD_BLOCK_PROCESS              0x0000001C
#define     NSHC_KILL                                   0x00000002
#define     NSHC_INTREN                                 0x00000001

/* Host command */
#define ICH_SMBUS_HST_CMD                               0x00000003

/* Transmit slave address */
#define ICH_SMBUS_XMIT_SLVA                             0x00000004
#define     NSXS_WRITE                                  0x00000000
#define     NSXS_READ                                   0x00000001

/* Host data 0 */
#define ICH_SMBUS_HST_D0                                0x00000005

/* Host data 1 */
#define ICH_SMBUS_HST_D1                                0x00000006

/* Host block data byte */
#define ICH_SMBUS_HOST_BLOCK_DB                         0x00000007

/* Packet error check */
#define ICH_SMBUS_PEC                                   0x00000008

/* Receive slave address */
#define ICH_SMBUS_RCV_SLVA                              0x00000009

/* Receive slave data */
#define ICH_SMBUS_SLV_DATA                              0x0000000A

/* Auxiliary status */
#define ICH_SMBUS_AUX_STS                               0x0000000C
#define    NSAS_SMBUS_TCO_MODE                          0x00000002
#define    NSAS_CRC_ERROR                               0x00000001

/* Auxiliary control */
#define ICH_SMBUS_AUX_CTL                               0x0000000D
#define     NSAC_E32B                                   0x00000002
#define     NSAC_AAC                                    0x00000001

/* SMLink pin control (TCO compatible mode) */
#define ICH_SMBUS_SMLINK_PIN_CTL                        0x0000000E
#define     NSSPC_SMLINK_CLK_CTL                        0x00000004
#define     NSSPC_SMLINK1_CUR_STS                       0x00000002
#define     NSSPC_SMLINK0_CUR_STS                       0x00000001

/* SMBus pin control */
#define ICH_SMBUS_PIN_CTL                               0x0000000F
#define     NSPC_SMBCLK_CTL                             0x00000004
#define     NSPC_SMBDATA_CUR_STS                        0x00000002
#define     NSPC_SMBCLK_CUR_STS                         0x00000001

/* Slave status */
#define ICH_SMBUS_SLV_STS                               0x00000010

/* Slave command */
#define ICH_SMBUS_SLV_CMD                               0x00000011
#define     NSSC_SMBALERT_DIS                           0x00000004
#define     NSSC_HOST_NOTIFY_WKEN                       0x00000002
#define     NSSC_HOST_NOTIFY_INTREN                     0x00000001

/* Notify device address */
#define ICH_SMBUS_NOTIFY_DADDR                          0x00000014

/* Notify data low byte */
#define ICH_SMBUS_NOTIFY_DLOW                           0x00000016

/* Notify data high byte */
#define ICH_SMBUS_NOTIFY_DHIGH                          0x00000017

#endif /* __SMBUS_ICH_H__ */
