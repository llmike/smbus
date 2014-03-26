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

#include <sys/slog.h>
#include <sys/mman.h>

#include "proto.h"
#include "smbus_ec.h"

int ec_smbus_read(resmgr_context_t* ctp, io_devctl_t* msg, ec_ocb_t* ocb)
{
    ec_dev_t*         dev=ocb->dev;
    smbus_cmd_t*      cmd;
    smbus_cmd_data_t* cmd_out;
    uint8_t           slv_addr;
    uint8_t           length;
    uint8_t           cmd_type;
    uint16_t          data;
    uint8_t           smbus_cmd;
    uint8_t*          buf;
    uint8_t*          buf_in;
    int               it=0;
    volatile uint8_t  tempval;
    int               timeout;
    struct timespec   ts_ns={0, 1};

    cmd=_DEVCTL_DATA(msg->i);
    buf_in=(uint8_t*)cmd + sizeof(smbus_cmd_t);

    /* Store input arguments */
    slv_addr=cmd->slv_addr;
    data=cmd->data;
    length=cmd->length;
    cmd_type=cmd->cmd_type;
    smbus_cmd=cmd->smbus_cmd;

    SETIOV(&ctp->iov[0], &msg->o, sizeof(msg->o) + length + sizeof(smbus_cmd_t));
    memset(&msg->o, 0x00, sizeof(msg->o) + sizeof(smbus_cmd_t));
    cmd_out=_DEVCTL_DATA(msg->o);
    buf=_DEVCTL_DATA(msg->o);
    buf+=sizeof(smbus_cmd_t);

    /* Copy input arguments to output */
    cmd_out->slv_addr=slv_addr;
    cmd_out->data=data;
    cmd_out->length=length;
    cmd_out->cmd_type=cmd_type;
    cmd_out->smbus_cmd=smbus_cmd;

    /* Check if length less than zero, return empty message */
    if (length<0)
    {
        memset(&msg->o, 0, sizeof(msg->o));
        return _RESMGR_PTR(ctp, &msg->o, sizeof(msg->o));
    }

    switch (cmd_type)
    {
        case SMBUS_CMD_QUICK:
             write_smbus(dev, SMB_ADDR, slv_addr<<1);
             write_smbus(dev, SMB_PRTCL, SMB_PRTCL_READ_QUICK_COMMAND);

             /* Wait for the command complete */
             timeout=100;
             do {
                 tempval=read_smbus(dev, SMB_PRTCL);
                 if (tempval==SMB_PRTCL_CONTROLLER_NOT_IN_USE)
                 {
                     /* Command has been completed */
                     break;
                 }
                 timeout--;
                 if (timeout==0)
                 {
                     break;
                 }
                 nanosleep(&ts_ns, NULL);
             } while(1);

             /* Check for the timeout status */
             if (timeout==0)
             {
                 return ETIMEDOUT;
             }

             /* Check if command has been completed successfully */
             tempval=read_smbus(dev, SMB_STS);
             if ((tempval & SMB_STS_STATUS_MASK)!=SMB_STATUS_OK)
             {
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_TIMEOUT)
                 {
                     return ETIMEDOUT;
                 }
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_BUSY)
                 {
                     return EBUSY;
                 }

                 return EIO;
             }
             break;
        case SMBUS_CMD_BYTE:
             write_smbus(dev, SMB_ADDR, slv_addr<<1);
             write_smbus(dev, SMB_PRTCL, SMB_PRTCL_RECEIVE_BYTE);

             /* Wait for the command complete */
             timeout=100;
             do {
                 tempval=read_smbus(dev, SMB_PRTCL);
                 if (tempval==SMB_PRTCL_CONTROLLER_NOT_IN_USE)
                 {
                     /* Command has been completed */
                     break;
                 }
                 timeout--;
                 if (timeout==0)
                 {
                     break;
                 }
                 nanosleep(&ts_ns, NULL);
             } while(1);

             /* Check for the timeout status */
             if (timeout==0)
             {
                 return ETIMEDOUT;
             }

             /* Check if command has been completed successfully */
             tempval=read_smbus(dev, SMB_STS);
             if ((tempval & SMB_STS_STATUS_MASK)!=SMB_STATUS_OK)
             {
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_TIMEOUT)
                 {
                     return ETIMEDOUT;
                 }
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_BUSY)
                 {
                     return EBUSY;
                 }

                 return EIO;
             }

             /* Store retrieved 8 bit data */
             cmd_out->data=read_smbus(dev, SMB_DATA0);
             if (length>0)
             {
                 *buf++=cmd_out->data & 0x000000FF;
             }
             break;
        case SMBUS_CMD_BYTE_DATA:
             write_smbus(dev, SMB_ADDR, slv_addr<<1);
             write_smbus(dev, SMB_CMD, smbus_cmd);
             write_smbus(dev, SMB_PRTCL, SMB_PRTCL_READ_BYTE);

             /* Wait for the command complete */
             timeout=100;
             do {
                 tempval=read_smbus(dev, SMB_PRTCL);
                 if (tempval==SMB_PRTCL_CONTROLLER_NOT_IN_USE)
                 {
                     /* Command has been completed */
                     break;
                 }
                 timeout--;
                 if (timeout==0)
                 {
                     break;
                 }
                 nanosleep(&ts_ns, NULL);
             } while(1);

             /* Check for the timeout status */
             if (timeout==0)
             {
                 return ETIMEDOUT;
             }

             /* Check if command has been completed successfully */
             tempval=read_smbus(dev, SMB_STS);
             if ((tempval & SMB_STS_STATUS_MASK)!=SMB_STATUS_OK)
             {
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_TIMEOUT)
                 {
                     return ETIMEDOUT;
                 }
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_BUSY)
                 {
                     return EBUSY;
                 }

                 return EIO;
             }

             /* Store retrieved 8 bit data */
             cmd_out->data=read_smbus(dev, SMB_DATA0);
             if (length>0)
             {
                 *buf++=cmd_out->data & 0x000000FF;
             }
             break;
        case SMBUS_CMD_WORD_DATA:
             write_smbus(dev, SMB_ADDR, slv_addr<<1);
             write_smbus(dev, SMB_CMD, smbus_cmd);
             write_smbus(dev, SMB_PRTCL, SMB_PRTCL_READ_WORD);

             /* Wait for the command complete */
             timeout=100;
             do {
                 tempval=read_smbus(dev, SMB_PRTCL);
                 if (tempval==SMB_PRTCL_CONTROLLER_NOT_IN_USE)
                 {
                     /* Command has been completed */
                     break;
                 }
                 timeout--;
                 if (timeout==0)
                 {
                     break;
                 }
                 nanosleep(&ts_ns, NULL);
             } while(1);

             /* Check for the timeout status */
             if (timeout==0)
             {
                 return ETIMEDOUT;
             }

             /* Check if command has been completed successfully */
             tempval=read_smbus(dev, SMB_STS);
             if ((tempval & SMB_STS_STATUS_MASK)!=SMB_STATUS_OK)
             {
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_TIMEOUT)
                 {
                     return ETIMEDOUT;
                 }
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_BUSY)
                 {
                     return EBUSY;
                 }

                 return EIO;
             }

             /* Store retrieved 16 bit data */
             cmd_out->data=read_smbus(dev, SMB_DATA0) | (read_smbus(dev, SMB_DATA1)<<8);
             if (length>0)
             {
                 *buf++=cmd_out->data & 0x000000FF;
             }
             if (length>1)
             {
                 *buf++=(cmd_out->data>>8) & 0x000000FF;
             }
             break;
        case SMBUS_CMD_PROCESS_CALL:
             write_smbus(dev, SMB_ADDR, slv_addr<<1);
             write_smbus(dev, SMB_CMD, smbus_cmd);
             write_smbus(dev, SMB_DATA0, data & 0x000000FF);
             write_smbus(dev, SMB_DATA1, (data>>8) & 0x000000FF);
             write_smbus(dev, SMB_PRTCL, SMB_PRTCL_WRITE_WORD);

             /* Wait for the command complete */
             timeout=100;
             do {
                 tempval=read_smbus(dev, SMB_PRTCL);
                 if (tempval==SMB_PRTCL_CONTROLLER_NOT_IN_USE)
                 {
                     /* Command has been completed */
                     break;
                 }
                 timeout--;
                 if (timeout==0)
                 {
                     break;
                 }
                 nanosleep(&ts_ns, NULL);
             } while(1);

             /* Check for the timeout status */
             if (timeout==0)
             {
                 return ETIMEDOUT;
             }

             /* Check if command has been completed successfully */
             tempval=read_smbus(dev, SMB_STS);
             if ((tempval & SMB_STS_STATUS_MASK)!=SMB_STATUS_OK)
             {
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_TIMEOUT)
                 {
                     return ETIMEDOUT;
                 }
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_BUSY)
                 {
                     return EBUSY;
                 }

                 return EIO;
             }

             /* Store retrieved 16 bit data */
             cmd_out->data=read_smbus(dev, SMB_DATA0) | (read_smbus(dev, SMB_DATA1)<<8);
             if (length>0)
             {
                 *buf++=cmd_out->data & 0x000000FF;
             }
             if (length>1)
             {
                 *buf++=(cmd_out->data>>8) & 0x000000FF;
             }
             break;
        case SMBUS_CMD_BLOCK:
             write_smbus(dev, SMB_ADDR, slv_addr<<1);
             write_smbus(dev, SMB_CMD, smbus_cmd);
             write_smbus(dev, SMB_PRTCL, SMB_PRTCL_READ_BLOCK);

             /* Wait for the command complete */
             timeout=100;
             do {
                 tempval=read_smbus(dev, SMB_PRTCL);
                 if (tempval==SMB_PRTCL_CONTROLLER_NOT_IN_USE)
                 {
                     /* Command has been completed */
                     break;
                 }
                 timeout--;
                 if (timeout==0)
                 {
                     break;
                 }
                 nanosleep(&ts_ns, NULL);
             } while(1);

             /* Check for the timeout status */
             if (timeout==0)
             {
                 return ETIMEDOUT;
             }

             /* Check if command has been completed successfully */
             tempval=read_smbus(dev, SMB_STS);
             if ((tempval & SMB_STS_STATUS_MASK)!=SMB_STATUS_OK)
             {
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_TIMEOUT)
                 {
                     return ETIMEDOUT;
                 }
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_BUSY)
                 {
                     return EBUSY;
                 }

                 return EIO;
             }

             /* Store retrieved data */
             cmd_out->length=read_smbus(dev, SMB_BCNT);
             if (cmd_out->length>length)
             {
                 cmd_out->length=length;
             }
             for (it=0; it<cmd_out->length; it++)
             {
                 /* Read all available bytes from data registers */
                 *buf++=read_smbus(dev, SMB_DATA0 + it);
             }
             break;
        case SMBUS_CMD_I2C_READ:
             /* EC doesn't have such command */
             return ENOTSUP;
        case SMBUS_CMD_BLOCK_PROCESS:
             write_smbus(dev, SMB_ADDR, slv_addr<<1);
             write_smbus(dev, SMB_CMD, smbus_cmd);
             write_smbus(dev, SMB_PRTCL, SMB_PRTCL_WRITE_BLOCK);

             /* Upload BLOCK of data */
             if (length>SMBUS_MAX_DATA_SIZE)
             {
                 length=SMBUS_MAX_DATA_SIZE;
             }
             write_smbus(dev, SMB_BCNT, length);
             for (it=0; it<length; it++)
             {
                 /* Write all available bytes to data registers */
                 write_smbus(dev, SMB_DATA0 + it, *buf_in++);
             }

             /* Wait for the command complete */
             timeout=100;
             do {
                 tempval=read_smbus(dev, SMB_PRTCL);
                 if (tempval==SMB_PRTCL_CONTROLLER_NOT_IN_USE)
                 {
                     /* Command has been completed */
                     break;
                 }
                 timeout--;
                 if (timeout==0)
                 {
                     break;
                 }
                 nanosleep(&ts_ns, NULL);
             } while(1);

             /* Check for the timeout status */
             if (timeout==0)
             {
                 return ETIMEDOUT;
             }

             /* Check if command has been completed successfully */
             tempval=read_smbus(dev, SMB_STS);
             if ((tempval & SMB_STS_STATUS_MASK)!=SMB_STATUS_OK)
             {
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_TIMEOUT)
                 {
                     return ETIMEDOUT;
                 }
                 if ((tempval & SMB_STS_STATUS_MASK)==SMB_STATUS_SMBUS_BUSY)
                 {
                     return EBUSY;
                 }

                 return EIO;
             }

             /* Store retrieved data */
             cmd_out->length=read_smbus(dev, SMB_BCNT);
             if (cmd_out->length>length)
             {
                 cmd_out->length=length;
             }
             for (it=0; it<cmd_out->length; it++)
             {
                 /* Read all available bytes from data registers */
                 *buf++=read_smbus(dev, SMB_DATA0 + it);
             }
             break;
        default:
             slogf(_SLOGC_EC_SMBUS, _SLOG_ERROR, "[smb-ec] Unknown SMBUS command type: %x", cmd_type);
             return EINVAL;
    }

    msg->o.ret_val=EOK;
    msg->o.nbytes=it + sizeof(smbus_cmd_t);

    return _RESMGR_NPARTS(1);
}
