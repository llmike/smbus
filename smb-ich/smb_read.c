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
#include "hw.h"

int ich_smbus_read(resmgr_context_t* ctp, io_devctl_t* msg, ich_ocb_t* ocb)
{
    ich_dev_t*        dev=ocb->dev;
    smbus_cmd_t*      cmd;
    smbus_cmd_data_t* cmd_out;
    uint8_t           slv_addr;
    uint8_t           length;
    uint8_t           cmd_type;
    uint16_t          data;
    uint8_t           smbus_cmd;
    uint8_t*          buf;
    int               it=0;
    volatile uint8_t  tempval;
    int               timeout;
    struct timespec   ts_ms={0, 100};

    cmd=_DEVCTL_DATA(msg->i);

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

    /* Try to acquire SMBUS */
    timeout=1000000;
    do {
        tempval=read_smbus(dev, ICH_SMBUS_HST_STS);
        if (!(tempval & NSHS_INUSE_STS))
        {
            /* Hardware is free, acquire it */
            break;
        }
        timeout--;
    } while(timeout);

    /* Check if acquiring was done */
    if (timeout==0)
    {
        slogf(_SLOGC_ICH_SMBUS, _SLOG_ERROR, "[smb-ich] Can't acquire SMBUS, timeout occured");
        return EBUSY;
    }

    switch (cmd_type)
    {
        case SMBUS_CMD_QUICK:
             /* Set SMBUS slave address */
             write_smbus(dev, ICH_SMBUS_XMIT_SLVA, (slv_addr<<1) | NSXS_READ);
             /* Clear SMBUS block command */
             write_smbus(dev, ICH_SMBUS_HST_CMD, 0x00000000);
             /* Disable 32 byte buffer in chipset for data transfer */
             write_smbus(dev, ICH_SMBUS_AUX_CTL, 0x00000000);
             /* Reset data registers */
             write_smbus(dev, ICH_SMBUS_HST_D0, 0x00000000);
             write_smbus(dev, ICH_SMBUS_HST_D1, 0x00000000);
             /* Start SMBUS transaction in BYTE mode */
             write_smbus(dev, ICH_SMBUS_HST_CNT, NSHC_SMB_CMD_QUICK | NSHC_START);

             /* wait for the transfer end */
             timeout=100;
             do {
                 tempval=read_smbus(dev, ICH_SMBUS_HST_STS);
                 if ((tempval & NSHS_INTR)==NSHS_INTR)
                 {
                     /* All data has been received */
                     break;
                 }
                 /* Clear SMBUS busy and error status before exit */
                 write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS |
                     NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR | NSHS_INTR);
                 if (tempval & (NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR))
                 {
                     return EIO;
                 }
                 nanosleep(&ts_ms, NULL);
                 timeout--;
             } while(timeout);

             /* Check if transfer has been terminated by timeout */
             if (timeout==0)
             {
                 /* Kill current transaction */
                 write_smbus(dev, ICH_SMBUS_HST_CNT, NSHC_KILL);
                 tempval=read_smbus(dev, ICH_SMBUS_HST_CNT);
                 write_smbus(dev, ICH_SMBUS_HST_CNT, 0x00000000);

                 /* Clear SMBUS busy and error status before exit */
                 write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS |
                     NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR | NSHS_INTR);

                 return ETIMEDOUT;
             }
             break;
        case SMBUS_CMD_BYTE:
             /* Set SMBUS slave address */
             write_smbus(dev, ICH_SMBUS_XMIT_SLVA, (slv_addr<<1) | NSXS_READ);
             /* Clear SMBUS block command */
             write_smbus(dev, ICH_SMBUS_HST_CMD, 0x00000000);
             /* Disable 32 byte buffer in chipset for data transfer */
             write_smbus(dev, ICH_SMBUS_AUX_CTL, 0x00000000);
             /* Reset data registers */
             write_smbus(dev, ICH_SMBUS_HST_D0, 0x00000000);
             write_smbus(dev, ICH_SMBUS_HST_D1, 0x00000000);
             /* Start SMBUS transaction in BYTE mode */
             write_smbus(dev, ICH_SMBUS_HST_CNT, NSHC_SMB_CMD_BYTE | NSHC_START);

             /* wait for the transfer end */
             timeout=100;
             do {
                 tempval=read_smbus(dev, ICH_SMBUS_HST_STS);
                 if ((tempval & NSHS_INTR)==NSHS_INTR)
                 {
                     /* All data has been received */
                     break;
                 }
                 /* Clear SMBUS busy and error status before exit */
                 write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS |
                     NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR | NSHS_INTR);
                 if (tempval & (NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR))
                 {
                     return EIO;
                 }
                 nanosleep(&ts_ms, NULL);
                 timeout--;
             } while(timeout);

             /* Check if transfer has been terminated by timeout */
             if (timeout==0)
             {
                 /* Kill current transaction */
                 write_smbus(dev, ICH_SMBUS_HST_CNT, NSHC_KILL);
                 tempval=read_smbus(dev, ICH_SMBUS_HST_CNT);
                 write_smbus(dev, ICH_SMBUS_HST_CNT, 0x00000000);

                 /* Clear SMBUS busy and error status before exit */
                 write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS |
                     NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR | NSHS_INTR);

                 return ETIMEDOUT;
             }
             else
             {
                 /* Store retrieved 8 bit data */
                 cmd_out->data=read_smbus(dev, ICH_SMBUS_HST_D0);
                 if (length>0)
                 {
                     *buf++=cmd_out->data;
                 }
             }
             break;
        case SMBUS_CMD_BYTE_DATA:
             /* Set SMBUS slave address */
             write_smbus(dev, ICH_SMBUS_XMIT_SLVA, (slv_addr<<1) | NSXS_READ);
             /* Set SMBUS block command */
             write_smbus(dev, ICH_SMBUS_HST_CMD, smbus_cmd);
             /* Disable 32 byte buffer in chipset for data transfer */
             write_smbus(dev, ICH_SMBUS_AUX_CTL, 0x00000000);
             /* Reset data registers */
             write_smbus(dev, ICH_SMBUS_HST_D0, 0x00000000);
             write_smbus(dev, ICH_SMBUS_HST_D1, 0x00000000);
             /* Start SMBUS transaction in BYTE mode */
             write_smbus(dev, ICH_SMBUS_HST_CNT, NSHC_SMB_CMD_BYTE_DATA | NSHC_START);

             /* wait for the transfer end */
             timeout=100;
             do {
                 tempval=read_smbus(dev, ICH_SMBUS_HST_STS);
                 if ((tempval & NSHS_INTR)==NSHS_INTR)
                 {
                     /* All data has been received */
                     break;
                 }
                 /* Clear SMBUS busy and error status before exit */
                 write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS |
                     NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR | NSHS_INTR);
                 if (tempval & (NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR))
                 {
                     return EIO;
                 }
                 nanosleep(&ts_ms, NULL);
                 timeout--;
             } while(timeout);

             /* Check if transfer has been terminated by timeout */
             if (timeout==0)
             {
                 /* Kill current transaction */
                 write_smbus(dev, ICH_SMBUS_HST_CNT, NSHC_KILL);
                 tempval=read_smbus(dev, ICH_SMBUS_HST_CNT);
                 write_smbus(dev, ICH_SMBUS_HST_CNT, 0x00000000);

                 /* Clear SMBUS busy and error status before exit */
                 write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS |
                     NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR | NSHS_INTR);

                 return ETIMEDOUT;
             }
             else
             {
                 /* Store retrieved 8 bit data */
                 cmd_out->data=read_smbus(dev, ICH_SMBUS_HST_D0);
                 if (length>0)
                 {
                     *buf++=cmd_out->data;
                 }
             }
             break;
        case SMBUS_CMD_WORD_DATA:
             /* Set SMBUS slave address */
             write_smbus(dev, ICH_SMBUS_XMIT_SLVA, (slv_addr<<1) | NSXS_READ);
             /* Set SMBUS block command */
             write_smbus(dev, ICH_SMBUS_HST_CMD, smbus_cmd);
             /* Disable 32 byte buffer in chipset for data transfer */
             write_smbus(dev, ICH_SMBUS_AUX_CTL, 0x00000000);
             /* Reset data registers */
             write_smbus(dev, ICH_SMBUS_HST_D0, 0x00000000);
             write_smbus(dev, ICH_SMBUS_HST_D1, 0x00000000);
             /* Start SMBUS transaction in WORD mode */
             write_smbus(dev, ICH_SMBUS_HST_CNT, NSHC_SMB_CMD_WORD_DATA | NSHC_START);

             /* wait for the transfer end */
             timeout=100;
             do {
                 tempval=read_smbus(dev, ICH_SMBUS_HST_STS);
                 if ((tempval & NSHS_INTR)==NSHS_INTR)
                 {
                     /* All data has been received */
                     break;
                 }
                 if (tempval & (NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR))
                 {
                     /* Clear SMBUS busy and error status before exit */
                     write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS |
                         NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR | NSHS_INTR);
                     return EIO;
                 }
                 nanosleep(&ts_ms, NULL);
                 timeout--;
             } while(timeout);

             /* Check if transfer has been terminated by timeout */
             if (timeout==0)
             {
                 /* Kill current transaction */
                 write_smbus(dev, ICH_SMBUS_HST_CNT, NSHC_KILL);
                 tempval=read_smbus(dev, ICH_SMBUS_HST_CNT);
                 write_smbus(dev, ICH_SMBUS_HST_CNT, 0x00000000);

                 /* Clear SMBUS busy and error status before exit */
                 write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS |
                     NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR | NSHS_INTR);

                 return ETIMEDOUT;
             }
             else
             {
                 /* Store retrieved 16 bit data */
                 cmd_out->data=read_smbus(dev, ICH_SMBUS_HST_D0) |
                     (read_smbus(dev, ICH_SMBUS_HST_D1)<<8);
                 if (length>0)
                 {
                     *buf++=cmd_out->data & 0x000000FF;
                 }
                 if (length>1)
                 {
                     *buf++=(cmd_out->data>>8) & 0x000000FF;
                 }
             }
             break;
        case SMBUS_CMD_PROCESS_CALL:
             /* Clear SMBUS busy status before exit */
             write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS | NSHS_INTR);
             return ENOTSUP;
        case SMBUS_CMD_BLOCK:
             /* Set SMBUS slave address */
             write_smbus(dev, ICH_SMBUS_XMIT_SLVA, (slv_addr<<1) | NSXS_READ);
             /* Set SMBUS block command */
             write_smbus(dev, ICH_SMBUS_HST_CMD, smbus_cmd);
             /* Do empty read to reset byte counter */
             tempval=read_smbus(dev, ICH_SMBUS_HST_CNT);
             /* Enable 32 byte buffer in chipset for data transfer */
             write_smbus(dev, ICH_SMBUS_AUX_CTL, NSAC_E32B);
             /* Reset data registers */
             write_smbus(dev, ICH_SMBUS_HST_D0, 0x00000000);
             write_smbus(dev, ICH_SMBUS_HST_D1, 0x00000000);
             /* Start SMBUS transaction in BLOCK mode */
             write_smbus(dev, ICH_SMBUS_HST_CNT, NSHC_SMB_CMD_BLOCK | NSHC_START);

             /* wait for the transfer end */
             timeout=100;
             do {
                 tempval=read_smbus(dev, ICH_SMBUS_HST_STS);
                 if ((tempval & NSHS_INTR)==NSHS_INTR)
                 {
                     /* All data has been received */
                     break;
                 }
                 if (tempval & (NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR))
                 {
                     /* Clear 32 byte buffer at exit */
                     write_smbus(dev, ICH_SMBUS_AUX_CTL, 0x00000000);

                     /* Clear SMBUS busy and error status before exit */
                     write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS |
                         NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR | NSHS_INTR);

                     return EIO;
                 }
                 nanosleep(&ts_ms, NULL);
                 timeout--;
             } while(timeout);

             /* Check if transfer has been terminated by timeout */
             if (timeout==0)
             {
                 /* Kill current transaction */
                 write_smbus(dev, ICH_SMBUS_HST_CNT, NSHC_KILL);
                 tempval=read_smbus(dev, ICH_SMBUS_HST_CNT);
                 write_smbus(dev, ICH_SMBUS_HST_CNT, 0x00000000);

                 /* Clear 32 byte buffer at exit */
                 write_smbus(dev, ICH_SMBUS_AUX_CTL, 0x00000000);

                 /* Clear SMBUS busy and error status before exit */
                 write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS |
                     NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR | NSHS_INTR);

                 return ETIMEDOUT;
             }
             else
             {
                 int block_size;

                 /* Check available block size */
                 block_size=read_smbus(dev, ICH_SMBUS_HST_D0);
                 if (block_size>length)
                 {
                     block_size=length;
                 }
                 cmd_out->length=block_size;
                 for (it=0; it<block_size; it++)
                 {
                     /* Read all available bytes from internal SRAM */
                     *buf++=read_smbus(dev, ICH_SMBUS_HOST_BLOCK_DB);
                 }
             }
             break;
        case SMBUS_CMD_I2C_READ:
             /* Clear SMBUS busy status before exit */
             write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS | NSHS_INTR);
             return ENOTSUP;
        case SMBUS_CMD_BLOCK_PROCESS:
             /* Clear SMBUS busy status before exit */
             write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS | NSHS_INTR);
             return ENOTSUP;
        default:
             slogf(_SLOGC_ICH_SMBUS, _SLOG_ERROR, "[smb-ich] Unknown SMBUS command type: %x", cmd_type);
             return EINVAL;
    }

    /* Clear 32 byte buffer at exit */
    write_smbus(dev, ICH_SMBUS_AUX_CTL, 0x00000000);

    /* Clear SMBUS busy and error status before exit */
    write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS |
        NSHS_FAILED | NSHS_BUS_ERR | NSHS_DEV_ERR | NSHS_INTR);

    msg->o.ret_val=EOK;
    msg->o.nbytes=it + sizeof(smbus_cmd_t);

    return _RESMGR_NPARTS(1);
}
