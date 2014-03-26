/*
 * $QNXLicenseC:
 * Copyright 2012, QNX Software Systems.
 * Copyright 2012, Mike Gorchak.
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

#ifndef __G781_H__
#define __G781_H__

#include <stdint.h>

#define G781_SLAVE_ADDRESS                                    0x4C
#define G781_SLAVE_ADDRESS2                                   0x4D

/* Read local temperature. It returns latest temperature */
#define G781_RLTS                                             0x00000000

/* Read remote temperature. It returns latest temperature */
#define G781_RRTE                                             0x00000001

/* Read status byte (flags, busy signal) */
#define G781_RSL                                              0x00000002
#define     G781_STATUS_ADC_BUSY                              0x00000080
#define     G781_STATUS_LOCAL_HIGH                            0x00000040
#define     G781_STATUS_LOCAL_LOW                             0x00000020
#define     G781_STATUS_REMOTE_HIGH                           0x00000010
#define     G781_STATUS_REMOTE_LOW                            0x00000008
#define     G781_STATUS_OPEN_CIRCUIT                          0x00000004
#define     G781_STATUS_REMOTE_THERM                          0x00000002
#define     G781_STATUS_LOCAL_THERM                           0x00000001

/* Read configuration byte */
#define G781_RCL                                              0x00000003
#define     G781_CONFIG_MASK                                  0x00000080
#define     G781_CONFIG_STOP                                  0x00000040

/* Read conversion rate byte */
#define G781_RCRA                                             0x00000004
#define     G781_RCRA_0_0625_HZ                               0x00000000
#define     G781_RCRA_0_125_HZ                                0x00000001
#define     G781_RCRA_0_25_HZ                                 0x00000002
#define     G781_RCRA_0_5_HZ                                  0x00000003
#define     G781_RCRA_1_HZ                                    0x00000004
#define     G781_RCRA_2_HZ                                    0x00000005
#define     G781_RCRA_4_HZ                                    0x00000006
#define     G781_RCRA_8_HZ                                    0x00000007
#define     G781_RCRA_16_HZ                                   0x00000008

/* Read local T_high limit */
#define G781_RLHN                                             0x00000005

/* Real local T_low limit */
#define G781_RLLI                                             0x00000006

/* Read remote T_high limit */
#define G781_RRHI                                             0x00000007

/* Read remote T_low limit */
#define G781_RRLS                                             0x00000008

/* Write configuration byte */
#define G781_WCA                                              0x00000009

/* Write conversion rate byte */
#define G781_WCRW                                             0x0000000A

/* Write local T_high limit */
#define G781_WLHO                                             0x0000000B

/* Write local T_low limit */
#define G781_WLLM                                             0x0000000C

/* Write remote T_high limit */
#define G781_WRHA                                             0x0000000D

/* Write remote T_low limit */
#define G781_WRLN                                             0x0000000E

/* One-short command (use send byte format) */
#define G781_OSHT                                             0x0000000F

/* Remote temperature extended byte */
#define G781_RTEXT                                            0x00000010
#define     G781_RTEXT_MASK                                   0x000000E0

/* Remote temperature offset high byte */
#define G781_RTOFS                                            0x00000011

/* Remote temperature offset extended byte */
#define G781_RTOFSEXT                                         0x00000012
#define     G781_RTOFSEXT_MASK                                0x000000E0

/* Remote T_high limit extended byte */
#define G781_RLEXT                                            0x00000013

/* Remote T_low limit extended byte */
#define G781_RHEXT                                            0x00000014

/* Remote temperature THERM limit */
#define G781_RTTHERM                                          0x00000019

/* Local temperature THERM */
#define G781_LTTHERM                                          0x00000020

/* THERM hysteresis */
#define G781_THERMHYST                                        0x00000021

/* ALERT fault queue code */
#define G781_ALERTFQ                                          0x00000022

/* Manufacturer ID */
#define G781_MFGIO                                            0x000000FE
#define     G781_MFGIO_GMT                                    0x00000047

/* Device ID */
#define G781_DEVID                                            0x000000FF
#define     G781_DEVID_G781                                   0x00000001
#define     G781_DEVID_G7811                                  0x00000003

#endif /* __G781_H__ */
