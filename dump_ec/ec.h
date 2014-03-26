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

#ifndef __EC_H__
#define __EC_H__

#include <stdint.h>

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
/* Function prototypes                                          */
/****************************************************************/

void write_ec_sc(uint8_t data);
uint8_t read_ec_sc();
void write_ec_data(uint8_t data);
uint8_t read_ec_data();

void write_ec(uint8_t reg, uint8_t data);
uint8_t read_ec(uint8_t reg);

#endif /* __SMBUS_EC_H__ */
