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

#include <devctl.h>
#include <inttypes.h>

/* 32 bytes is a maximum for SMBUS packet size */
#define SMBUS_MAX_DATA_SIZE                  0x00000020

#pragma pack(1)

typedef struct _smbus_cmd {
    uint8_t     cmd_type;
    uint8_t     slv_addr;   /* 7 bit slave address                     */
    uint8_t     smbus_cmd;  /* SMBUS command id                        */
    uint16_t    data;       /* data size depends on command type       */
    uint8_t     length;     /* amount of available space               */
} smbus_cmd_t;

typedef struct _smbus_cmd_data {
    uint8_t     cmd_type;
    uint8_t     slv_addr;
    uint8_t     smbus_cmd;
    uint16_t    data;
    uint8_t     length;
    uint8_t     rwdata[SMBUS_MAX_DATA_SIZE];
} smbus_cmd_data_t;

#pragma pack(0)

/***********************************************************************/
/* Definitions of command types for cmd_type field                     */
/***********************************************************************/

/* cmd_type = SMBUS_CMD_QUICK,                                         */
/* slv_addr = slave address,                                           */
#define SMBUS_CMD_QUICK                      0x00000000

/* cmd_type = SMBUS_CMD_BYTE,                                          */
/* slv_addr = slave address,                                           */
/* smbus_cmd - must contain SMBUS command,                             */
#define SMBUS_CMD_BYTE                       0x00000001

/* cmd_type = SMBUS_CMD_BYTE_DATA,                                     */
/* slv_addr = slave address,                                           */
/* data - low 8 bit of this field contains byte of data.               */
/* smbus_cmd - must contain SMBUS command,                             */
/* length = total data length                                          */
#define SMBUS_CMD_BYTE_DATA                  0x00000002

/* cmd_type = SMBUS_CMD_WORD_DATA,                                     */
/* slv_addr = slave address,                                           */
/* data - field contains word of data.                                 */
/* smbus_cmd - must contain SMBUS command,                             */
/* length = total data length                                          */
#define SMBUS_CMD_WORD_DATA                  0x00000003

/* cmd_type = SMBUS_CMD_PROCESS_CALL                                   */
/* slv_addr = slave address,                                           */
/* data - field contains word of data.                                 */
/* smbus_cmd - must contain SMBUS command,                             */
/* length = total data length                                          */
#define SMBUS_CMD_PROCESS_CALL               0x00000004

/* cmd_type = SMBUS_CMD_BLOCK,                                         */
/* slv_addr = slave address,                                           */
/* data - field is ignored,                                            */
/* smbus_cmd - must contain SMBUS command,                             */
/* length = total data length                                          */
#define SMBUS_CMD_BLOCK                      0x00000005

/* cmd_type = SMBUS_CMD_I2C_READ,                                      */
#define SMBUS_CMD_I2C_READ                   0x00000006

/* cmd_type = SMBUS_CMD_BLOCK_PROCESS,                                 */
/* slv_addr = slave address,                                           */
/* data - field is ignored,                                            */
/* smbus_cmd - must contain SMBUS command,                             */
/* length = total data length                                          */
#define SMBUS_CMD_BLOCK_PROCESS              0x00000007

/* SMBus read */
#define DCMD_SMBUS_READ   __DIOTF(_DCMD_MISC, 0x01, smbus_cmd_t)
/* SMBus write */
#define DCMD_SMBUS_WRITE  __DIOTF(_DCMD_MISC, 0x02, smbus_cmd_t)
