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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <devctl.h>
#include <fcntl.h>

#include <devctl.h>
#include <inttypes.h>

#include <dcmd_smbus.h>

#include "sb_defines.h"

int fd;

smbus_cmd_data_t cmd;

void print_battery_mask(uint8_t mask)
{
    switch(mask)
    {
        case SM_BSS_BATTERY_A:
             fprintf(stdout, "Battery A ");
             break;
        case SM_BSS_BATTERY_B:
             fprintf(stdout, "Battery B ");
             break;
        case SM_BSS_BATTERY_C:
             fprintf(stdout, "Battery C, ");
             break;
        case SM_BSS_BATTERY_D:
             fprintf(stdout, "Battery D ");
             break;
        case SM_BSS_BATTERY_COMPOSITE:
             fprintf(stdout, "All batteries ");
             break;
        case 3:
             fprintf(stdout, "Battery A, Battery B ");
             break;
        case 5:
             fprintf(stdout, "Battery A, Battery C ");
             break;
        case 6:
             fprintf(stdout, "Battery B, Battery C ");
             break;
        case 7:
             fprintf(stdout, "Battery A, Battery B, Battery C ");
             break;
        case 9:
             fprintf(stdout, "Battery A, Battery D ");
             break;
        case 10:
             fprintf(stdout, "Battery B, Battery D ");
             break;
        case 11:
             fprintf(stdout, "Battery A, Battery B, Battery D ");
             break;
        case 12:
             fprintf(stdout, "Battery C, Battery D ");
             break;
        case 13:
             fprintf(stdout, "Battery A, Battery C, Battery D ");
             break;
        case 14:
             fprintf(stdout, "Battery B, Battery C, Battery D ");
             break;
        default:
             fprintf(stdout, "No batteries");
             break;
    }
}

void print_time(uint16_t time)
{
    /* Split days from a time */
    printf("%dd ", time/60/24);
    /* Split hours from a time */
    printf("%dh ", (time - time/60/24*24*60)/60);
    /* Split minutes from a time */
    printf("%dm ", (time - time/60/24*24*60 - (time - time/60/24*24*60)/60*60));
}

void print_date(uint16_t date)
{
    printf("%d ", date & 0x001F);
    switch((date & 0x01E0)>>5)
    {
        case 1:
             fprintf(stdout, "Jan ");
             break;
        case 2:
             fprintf(stdout, "Feb ");
             break;
        case 3:
             fprintf(stdout, "Mar ");
             break;
        case 4:
             fprintf(stdout, "Apr ");
             break;
        case 5:
             fprintf(stdout, "May ");
             break;
        case 6:
             fprintf(stdout, "Jun ");
             break;
        case 7:
             fprintf(stdout, "Jul ");
             break;
        case 8:
             fprintf(stdout, "Aug ");
             break;
        case 9:
             fprintf(stdout, "Sep ");
             break;
        case 10:
             fprintf(stdout, "Oct ");
             break;
        case 11:
             fprintf(stdout, "Nov ");
             break;
        case 12:
             fprintf(stdout, "Dec ");
             break;
        default:
             fprintf(stdout, "Unknown ");
             break;
    }
    printf("%d ", ((date & 0xFE00)>>9)+1980);
}

void print_error(uint8_t error)
{
    switch(error)
    {
         case SB_BS_ERROR_OK:
              fprintf(stdout, "OK");
              break;
         case SB_BS_ERROR_BUSY:
              fprintf(stdout, "BUSY");
              break;
         case SB_BS_ERROR_RESERVED_COMMAND:
              fprintf(stdout, "RESERVED COMMAND");
              break;
         case SB_BS_ERROR_UNSUPPORTED_COMMAND:
              fprintf(stdout, "UNSUPPORTED COMMAND");
              break;
         case SB_BS_ERROR_ACCESS_DENIED:
              fprintf(stdout, "ACCESS DENIED");
              break;
         case SB_BS_ERROR_OVERFLOW_UNDERFLOW:
              fprintf(stdout, "OVERFLOW/UNDERFLOW");
              break;
         case SB_BS_ERROR_BAD_SIZE:
              fprintf(stdout, "BAD SIZE");
              break;
         case SB_BS_ERROR_UNKNOWN_ERROR:
              fprintf(stdout, "UNKNOWN ERRROR");
              break;
         default:
              fprintf(stdout, "UNKNOWN ERROR");
              break;
    }
}

int main(int argc, char *argv[])
{
    int      status;
    int      it;
    uint16_t battery_mode;
    int      smbus_id;
    int      found_smbus=0;
    int      ac_present=0;

    /* Go through all available SMBUSes */
    for (smbus_id=0; smbus_id<=99; smbus_id++)
    {
        char smbus_name[PATH_MAX];

        snprintf(smbus_name, PATH_MAX, "/dev/smb%d", smbus_id);

        fd=open(smbus_name, O_RDWR);
        if (fd<0)
        {
            if (smbus_id==99)
            {
                if (!found_smbus)
                {
                    fprintf(stderr, "Can't find any SMBus device!\n");
                }
                exit(1);
            }
            continue;
        }
        else
        {
            found_smbus=1;
        }
        fprintf(stdout, "Found SMBUS%d\n", smbus_id);

        /* Try to determine if we smart batter is present */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_BATTERY_STATUS;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            /* If battery is not detected, then skip this bus */
            continue;
        }

        /* Try to determine if charger is present */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_CHARGER_ADDRESS;
        cmd.smbus_cmd=SC_CHARGER_SPEC_INFO;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status==EOK)
        {
            fprintf(stdout, "Smart battery charger\n");
            fprintf(stdout, "    Smart battery charger version : ");
            if ((cmd.data & SC_CSI_CHARGER_SPEC_MASK)==SC_CSI_CHARGER_VERSION_1_0)
            {
                fprintf(stdout, "Version 1.0\n");
            }
            else
            {
                if ((cmd.data & SC_CSI_CHARGER_SPEC_MASK)==SC_CSI_CHARGER_VERSION_1_1)
                {
                    fprintf(stdout, "Version 1.1\n");
                }
                else
                {
                    if ((cmd.data & SC_CSI_CHARGER_SPEC_MASK)==SC_CSI_CHARGER_VERSION_1_1_PEC)
                    {
                        fprintf(stdout, "Version 1.1 with optional PEC\n");
                    }
                    else
                    {
                        fprintf(stdout, "Unknown version 0x%02X\n", cmd.data & SC_CSI_CHARGER_SPEC_MASK);
                    }
                }
            }

            fprintf(stdout, "    Selector interface w/ charger : ");
            if ((cmd.data & SC_CSI_SELECTOR_SUPPORT)==SC_CSI_SELECTOR_SUPPORT)
            {
                fprintf(stdout, "Supported\n");
            }
            else
            {
                fprintf(stdout, "Not supported\n");
            }

            cmd.cmd_type=SMBUS_CMD_WORD_DATA;
            cmd.slv_addr=SB_SMART_CHARGER_ADDRESS;
            cmd.smbus_cmd=SC_CHARGER_STATUS;
            cmd.length=0;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status==EOK)
            {
                fprintf(stdout, "    Is battery connected to charg.: ");
                if ((cmd.data & SC_CS_BATTERY_PRESENT)==SC_CS_BATTERY_PRESENT)
                {
                    fprintf(stdout, "Yes\n");
                }
                else
                {
                    fprintf(stdout, "No\n");
                }

                fprintf(stdout, "    Is AC power connected         : ");
                if ((cmd.data & SC_CS_AC_PRESENT)==SC_CS_AC_PRESENT)
                {
                    fprintf(stdout, "Yes\n");
                }
                else
                {
                    fprintf(stdout, "No\n");
                }
            }
            else
            {
                fprintf(stderr, "    Is battery connected to charg.: Communication error: %s\n", strerror(status));
            }
        }
        else
        {
            fprintf(stdout, "Smart manager/selector was not found\n");
        }

        /* Try to determine AC power connection */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_CHARGER_ADDRESS;
        cmd.smbus_cmd=SC_CHARGER_STATUS;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status==EOK)
        {
            if ((cmd.data & SC_CS_AC_PRESENT)==SC_CS_AC_PRESENT)
            {
                ac_present=1;
            }
        }

        /* Try to determine if we are talking with manager or selector */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_MANAGER_ADDRESS;
        cmd.smbus_cmd=SM_BATTERY_SYSTEM_INFO;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status==EOK)
        {

            /* Check if fourth bit of nibble is set, then we are talking with manager */
            if ((cmd.data & 0x80)==0x80)
            {
                fprintf(stdout, "Smart battery manager\n");

                /* Read batteries configuration */
                cmd.cmd_type=SMBUS_CMD_WORD_DATA;
                cmd.slv_addr=SB_SMART_MANAGER_ADDRESS;
                cmd.smbus_cmd=SM_BATTERY_SYSTEM_STATE;
                cmd.length=0;
                status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
                if (status!=EOK)
                {
                    fprintf(stderr, "    Battery(s) connected to SMBus : Communication error: %s\n", strerror(status));
                }
                else
                {
                    /* Parse the answer */
                    fprintf(stdout, "    Battery(s) connected to SMBus : ");
                    print_battery_mask((cmd.data & SM_BSS_CONNECTED_TO_SMB_MASK)>>SM_BSS_CONNECTED_TO_SMB_SHIFT);
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Battery(s) powering the system: ");
                    print_battery_mask((cmd.data & SM_BSS_POWERED_BY_MASK)>>SM_BSS_POWERED_BY_SHIFT);
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Battery(s) charging           : ");
                    print_battery_mask((cmd.data & SM_BSS_CHARGING_MASK)>>SM_BSS_CHARGING_SHIFT);
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Battery(s) present            : ");
                    print_battery_mask((cmd.data & SM_BSS_PRESENT_MASK)>>SM_BSS_PRESENT_SHIFT);
                    fprintf(stdout, "\n");
                }

                /* Read batteries information */
                cmd.cmd_type=SMBUS_CMD_WORD_DATA;
                cmd.slv_addr=SB_SMART_MANAGER_ADDRESS;
                cmd.smbus_cmd=SM_BATTERY_SYSTEM_INFO;
                cmd.length=0;
                status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
                if (status!=EOK)
                {
                    fprintf(stderr, "    Maximum batteries supported   : Communication error: %s\n", strerror(status));
                }
                else
                {
                    fprintf(stdout, "    Maximum batteries supported   : ");
                    print_battery_mask((cmd.data & SM_BSI_SUPPORTED_BATTERIES_MASK)>>SM_BSI_SUPPORTED_BATTERIES_SHIFT);
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Battery system revision       : ");
                    if ((cmd.data & SM_BSI_BATTERY_SYSTEM_REVISION_MASK)==SM_BSI_BATTERY_VERSION_1_0)
                    {
                        fprintf(stdout, "Version 1.0\n");
                    }
                    else
                    {
                        if ((cmd.data & SM_BSI_BATTERY_SYSTEM_REVISION_MASK)==SM_BSI_BATTERY_VERSION_1_0_PEC)
                        {
                            fprintf(stdout, "Version 1.0 with PEC\n");
                        }
                        else
                        {
                            fprintf(stdout, "Version unknown (%01X)\n",
                                (cmd.data & SM_BSI_BATTERY_SYSTEM_REVISION_MASK)>>SM_BSI_BATTERY_SYSTEM_REVISION_SHIFT);
                        }
                    }

                    fprintf(stdout, "    Battery current scale         : ");
                    fprintf(stdout, "10^%d\n", (cmd.data & SM_BSI_IPSCALE_MASK) >> SM_BSI_IPSCALE_SHIFT);
                    fprintf(stdout, "    Battery voltage scale         : ");
                    fprintf(stdout, "10^%d\n", (cmd.data & SM_BSI_VSCALE_MASK) >> SM_BSI_VSCALE_SHIFT);
                }

                /* Read batteries information */
                cmd.cmd_type=SMBUS_CMD_WORD_DATA;
                cmd.slv_addr=SB_SMART_MANAGER_ADDRESS;
                cmd.smbus_cmd=SM_BATTERY_SYSTEM_STATE_CONT;
                cmd.length=0;
                status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
                if (status!=EOK)
                {
                    fprintf(stderr, "    Status of AC power connection : Communication error: %s\n", strerror(status));
                }
                else
                {
                    fprintf(stdout, "    Status of AC power connection : ");
                    if ((cmd.data & SM_BSSC_AC_PRESENT)==SM_BSSC_AC_PRESENT)
                    {
                        fprintf(stdout, "Connected");
                    }
                    else
                    {
                        fprintf(stdout, "Not connected");
                    }
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Quality of AC power connection: ");
                    if ((cmd.data & SM_BSSC_POWER_NOT_GOOD)==SM_BSSC_POWER_NOT_GOOD)
                    {
                        fprintf(stdout, "Not good");
                    }
                    else
                    {
                        fprintf(stdout, "Good");
                    }
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Charging inhibition status    : ");
                    if ((cmd.data & SM_BSSC_CHARGING_INHIBIT)==SM_BSSC_CHARGING_INHIBIT)
                    {
                        fprintf(stdout, "Charging is inhibited");
                    }
                    else
                    {
                        fprintf(stdout, "Charging is allowed");
                    }
                    fprintf(stdout, "\n");
                }
            }
            else
            {
                /* We are talking with selector */
                fprintf(stdout, "Smart battery selector\n");

                /* Read batteries configuration */
                cmd.cmd_type=SMBUS_CMD_WORD_DATA;
                cmd.slv_addr=SB_SMART_SELECTOR_ADDRESS;
                cmd.smbus_cmd=SS_SELECTOR_STATE;
                cmd.length=0;
                status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
                if (status!=EOK)
                {
                    fprintf(stderr, "    Battery(s) connected to SMBus : Communication error: %s\n", strerror(status));
                }
                else
                {
                    /* Parse the answer */
                    fprintf(stdout, "    Battery(s) connected to SMBus : ");
                    print_battery_mask((cmd.data & SM_BSS_CONNECTED_TO_SMB_MASK)>>SM_BSS_CONNECTED_TO_SMB_SHIFT);
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Battery(s) powering the system: ");
                    if (ac_present)
                    {
                        print_battery_mask(((~cmd.data) & SM_BSS_POWERED_BY_MASK)>>SM_BSS_POWERED_BY_SHIFT);
                    }
                    else
                    {
                        print_battery_mask((cmd.data & SM_BSS_POWERED_BY_MASK)>>SM_BSS_POWERED_BY_SHIFT);
                    }
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Battery(s) charging           : ");
                    if (ac_present)
                    {
                        print_battery_mask(((~cmd.data) & SM_BSS_CHARGING_MASK)>>SM_BSS_CHARGING_SHIFT);
                    }
                    else
                    {
                        print_battery_mask((cmd.data & SM_BSS_CHARGING_MASK)>>SM_BSS_CHARGING_SHIFT);
                    }
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Battery(s) present            : ");
                    print_battery_mask((cmd.data & SM_BSS_PRESENT_MASK)>>SM_BSS_PRESENT_SHIFT);
                    fprintf(stdout, "\n");
                }

                /* Read selector configuration */
                cmd.cmd_type=SMBUS_CMD_WORD_DATA;
                cmd.slv_addr=SB_SMART_SELECTOR_ADDRESS;
                cmd.smbus_cmd=SS_SELECTOR_PRESETS;
                cmd.length=0;
                status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
                if (status!=EOK)
                {
                    fprintf(stderr, "    Battery(s) will be used next  : Communication error: %s\n", strerror(status));
                }
                else
                {
                    /* Parse the answer */
                    fprintf(stdout, "    Battery(s) will be used next  : ");
                    print_battery_mask((cmd.data & SS_SP_USE_NEXT_MASK)>>SS_SP_USE_NEXT_SHIFT);
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Available battery(s) for usage: ");
                    print_battery_mask((cmd.data & SS_SP_OK_TO_USE_MASK)>>SS_SP_OK_TO_USE_SHIFT);
                    fprintf(stdout, "\n");
                }

                /* Read batteries configuration */
                cmd.cmd_type=SMBUS_CMD_WORD_DATA;
                cmd.slv_addr=SB_SMART_SELECTOR_ADDRESS;
                cmd.smbus_cmd=SS_SELECTOR_INFO;
                cmd.length=0;
                status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
                if (status!=EOK)
                {
                    fprintf(stderr, "    AC power connection status    : Communication error: %s\n", strerror(status));
                }
                else
                {
                    /* Read AC power connection status */
                    fprintf(stdout, "    AC power connection status    : ");
                    if ((cmd.data & SS_SI_CHARGING_INDICATOR)==SS_SI_CHARGING_INDICATOR)
                    {
                        fprintf(stdout, "Supported");
                    }
                    else
                    {
                        fprintf(stdout, "Not supported");
                    }
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Selector supported battery(s) : ");
                    print_battery_mask((cmd.data & SS_SI_SUPPORTED_BATTERIES_MASK)>>SS_SI_SUPPORTED_BATTERIES_SHIFT);
                    fprintf(stdout, "\n");

                    fprintf(stdout, "    Selector system revision      : ");
                    if ((cmd.data & SS_SI_SELECTOR_REVISION_MASK)==SS_SI_SELECTOR_VERSION_1_0)
                    {
                        fprintf(stdout, "Version 1.0\n");
                    }
                    else
                    {
                        if ((cmd.data & SS_SI_SELECTOR_REVISION_MASK)==SS_SI_SELECTOR_VERSION_1_1)
                        {
                            fprintf(stdout, "Version 1.1\n");
                        }
                        else
                        {
                            if ((cmd.data & SS_SI_SELECTOR_REVISION_MASK)==SS_SI_SELECTOR_VERSION_1_1_PEC)
                            {
                                fprintf(stdout, "Version 1.1 with optional PEC\n");
                            }
                            else
                            {
                                fprintf(stdout, "Version unknown (%01X)\n",
                                    (cmd.data & SS_SI_SELECTOR_REVISION_MASK)>>SS_SI_SELECTOR_REVISION_SHIFT);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            fprintf(stdout, "Smart manager/selector was not found\n");
        }

        fprintf(stdout, "Smart battery\n");

        /* Read battery manufacturer */
        cmd.cmd_type=SMBUS_CMD_BLOCK;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_MANUFACTURER_NAME;
        cmd.length=SMBUS_MAX_DATA_SIZE;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Manufacturer Name             : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Manufacturer Name             : ");
            for (it=0; it<cmd.length; it++)
            {
                fprintf(stdout, "%c", cmd.rwdata[it]);
            }
            fprintf(stdout, "\n");
        }

        /* Read battery name */
        cmd.cmd_type=SMBUS_CMD_BLOCK;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_DEVICE_NAME;
        cmd.length=SMBUS_MAX_DATA_SIZE;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Device name                   : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Device name                   : ");
            for (it=0; it<cmd.length; it++)
            {
                fprintf(stdout, "%c", cmd.rwdata[it]);
            }
            fprintf(stdout, "\n");
        }

        /* Read battery chemistry */
        cmd.cmd_type=SMBUS_CMD_BLOCK;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_DEVICE_CHEMISTRY;
        cmd.length=SMBUS_MAX_DATA_SIZE;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Device chemistry              : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Device chemistry              : ");
            for (it=0; it<cmd.length; it++)
            {
                fprintf(stdout, "%c", cmd.rwdata[it]);
            }
            fprintf(stdout, "\n");
        }

        /* Read serial number */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_SERIAL_NUMBER;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Battery serial number         : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Battery serial number         : ");
            fprintf(stdout, "%04X ", cmd.data);
            fprintf(stdout, "\n");
        }

        /* Read manufacture date */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_MANUFACTURE_DATE;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Battery manufacture date      : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Battery manufacture date      : ");
            print_date(cmd.data);
            fprintf(stdout, "\n");
        }

        /* Read manufacturer data */
        cmd.cmd_type=SMBUS_CMD_BLOCK;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_MANUFACTURER_DATA;
        cmd.length=SMBUS_MAX_DATA_SIZE;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Manufacturer data             : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Manufacturer data             : ");
            for (it=0; it<cmd.length; it++)
            {
                fprintf(stdout, "%02X ", cmd.rwdata[it]);
            }
            fprintf(stdout, "\n");
        }

        /* Read battery manufacturer access */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_MANUFACTURER_ACCESS;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Manufacturer access word      : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Manufacturer access word      : %04X\n", cmd.data);
        }

        /* Read battery mode */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_BATTERY_MODE;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        battery_mode=cmd.data;

        /* Switch to the mAh units */
        battery_mode&=~(SM_BM_CAPACITY_MODE);
        cmd.data=battery_mode;
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_BATTERY_MODE;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_WRITE, &cmd,  sizeof(cmd), NULL);

        /* Read battery remaining capacity alarm */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_REMAINING_CAPACITY_ALARM;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Remaining capacity alarm at   : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Remaining capacity alarm at   : ");
            if ((battery_mode & SM_BM_CAPACITY_MODE)==SM_BM_CAPACITY_MODE)
            {
                fprintf(stdout, "%d mWh", cmd.data*10);
            }
            else
            {
                fprintf(stdout, "%d mAh", cmd.data);
            }
            fprintf(stdout, "\n");
        }

        /* Read battery remaining time alarm */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_REMAINING_TIME_ALARM;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Remaining time alarm at       : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Remaining time alarm at       : ");
            print_time(cmd.data);
            fprintf(stdout, "\n");
        }

        /* Print battery mode information */
        fprintf(stdout, "    Internal charge controller    : ");
        if ((battery_mode & SB_BM_INTERNAL_CHARGE_CONTROLLER)==SB_BM_INTERNAL_CHARGE_CONTROLLER)
        {
            if ((battery_mode & SM_BM_CHARGE_CONTROLLER_ENABLED)==SM_BM_CHARGE_CONTROLLER_ENABLED)
            {
                fprintf(stdout, "Supported / Enabled");
            }
            else
            {
                fprintf(stdout, "Supported / Not enabled");
            }
        }
        else
        {
            fprintf(stdout, "Not supported");
        }
        fprintf(stdout, "\n");

        fprintf(stdout, "    Primary/sec batteries support : ");
        if ((battery_mode & SB_BM_PRIMATY_BATTERY_SUPPORT)==SB_BM_PRIMATY_BATTERY_SUPPORT)
        {
            if ((battery_mode & SM_BM_PRIMARY_BATTERY)==SM_BM_PRIMARY_BATTERY)
            {
                fprintf(stdout, "Supported / Enabled");
            }
            else
            {
                fprintf(stdout, "Supported / Not enabled");
            }
        }
        else
        {
            fprintf(stdout, "Not supported");
        }
        fprintf(stdout, "\n");

        fprintf(stdout, "    Battery conditioning mode     : ");
        if ((battery_mode & SM_BM_CONDITION_FLAG)==SM_BM_CONDITION_FLAG)
        {
            fprintf(stdout, "Battery in conditioning mode (WARNING!)");
        }
        else
        {
            fprintf(stdout, "Battery in normal mode");
        }
        fprintf(stdout, "\n");

        fprintf(stdout, "    SMBus host alarm notifications: ");
        if ((battery_mode & SM_BM_ALARM_MODE)==SM_BM_ALARM_MODE)
        {
            fprintf(stdout, "Enabled");
        }
        else
        {
            fprintf(stdout, "Disabled");
        }
        fprintf(stdout, "\n");

        fprintf(stdout, "    Charger notifications         : ");
        if ((battery_mode & SM_BM_CHARGER_MODE)==SM_BM_CHARGER_MODE)
        {
            fprintf(stdout, "Disabled");
        }
        else
        {
            fprintf(stdout, "Enabled");
        }
        fprintf(stdout, "\n");

        fprintf(stdout, "    Capacity mode units           : ");
        if ((battery_mode & SM_BM_CAPACITY_MODE)==SM_BM_CAPACITY_MODE)
        {
            fprintf(stdout, "10*mWh");
        }
        else
        {
            fprintf(stdout, "mAh");
        }
        fprintf(stdout, "\n");

        /* Read battery rate */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_AT_RATE;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Charging/discharging rate     : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Charging/discharging rate     : ");
            if (cmd.data==0)
            {
                if ((battery_mode & SM_BM_CAPACITY_MODE)==SM_BM_CAPACITY_MODE)
                {
                    fprintf(stdout, "%d mWh (probably unsupported)", ((int)cmd.data)*10);
                }
                else
                {
                    fprintf(stdout, "%d mAh (probably unsupported)", (int)cmd.data);
                }
            }
            else
            {
                if ((battery_mode & SM_BM_CAPACITY_MODE)==SM_BM_CAPACITY_MODE)
                {
                    fprintf(stdout, "%d mWh", ((int)cmd.data)*10);
                }
                else
                {
                    fprintf(stdout, "%d mAh", (int)cmd.data);
                }
            }
            fprintf(stdout, "\n");
        }

        /* Read battery rate to full */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_AT_RATE_TIME_TO_FULL;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Time to charge full battery   : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Time to charge full battery   : ");
            print_time(cmd.data);
            if (cmd.data==65535)
            {
                fprintf(stdout, " (probably unsupported)");
            }
            fprintf(stdout, "\n");
        }

        /* Read battery rate to empty */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_AT_RATE_TIME_TO_EMPTY;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Remaining time for a battery  : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Remaining time for a battery  : ");
            print_time(cmd.data);
            if (cmd.data==65535)
            {
                fprintf(stdout, " (probably unsupported)");
            }
            fprintf(stdout, "\n");
        }

        /* Read battery rate for a 10 seconds */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_AT_RATE_OK;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Is 10 seconds available       : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Is 10 seconds available       : ");
            if (cmd.data)
            {
                fprintf(stdout, "Yes");
            }
            else
            {
                fprintf(stdout, "No");
            }
            fprintf(stdout, "\n");
        }

        /* Read battery temperature */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_TEMPERATURE;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Current battery temperature   : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Current battery temperature   : ");
            fprintf(stdout, "%d.%dK, ", cmd.data/10, cmd.data-cmd.data/10*10);
            fprintf(stdout, "%d.%dC, ", (cmd.data-2731)/10, (cmd.data-2731)-(cmd.data-2731)/10*10);
            fprintf(stdout, "%d.%dF ", (cmd.data*9/5-4596)/10, (cmd.data*9/5-4596)-(cmd.data*9/5-4596)/10*10);
            fprintf(stdout, "\n");
        }

        /* Read battery voltage */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_VOLTAGE;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Current battery voltage       : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Current battery voltage       : ");
            fprintf(stdout, "%d mV ", cmd.data);
            fprintf(stdout, "\n");
        }

        /* Read battery current */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_CURRENT;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Current battery current       : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Current battery current       : ");
            fprintf(stdout, "%d mA ", (int16_t)cmd.data);
            fprintf(stdout, "\n");
        }

        /* Read average battery current */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_AVERAGE_CURRENT;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Average battery current       : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Average battery current       : ");
            fprintf(stdout, "%d mA ", (int16_t)cmd.data);
            fprintf(stdout, "\n");
        }

        /* Read maximum error */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_MAX_ERROR;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Maximum error of data         : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Maximum error of data         : ");
            fprintf(stdout, "%d%%", cmd.data);
            fprintf(stdout, "\n");
        }

        /* Read relative state of charge */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_RELATIVE_STATE_OF_CHARGE;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Relative state of charge      : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Relative state of charge      : ");
            fprintf(stdout, "%d%%", cmd.data);
            fprintf(stdout, "\n");
        }

        /* Read absolute state of charge */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_ABSOLUTE_STATE_OF_CHARGE;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Absolute state of charge      : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Absolute state of charge      : ");
            fprintf(stdout, "%d%%", cmd.data);
            fprintf(stdout, "\n");
        }

        /* Read remaining capacity */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_REMAINING_CAPACITY;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Remaining capacity of battery : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Remaining capacity of battery : ");
            if ((battery_mode & SM_BM_CAPACITY_MODE)==SM_BM_CAPACITY_MODE)
            {
                fprintf(stdout, "%d mWh", (cmd.data)*10);
            }
            else
            {
               fprintf(stdout, "%d mAh", cmd.data);
            }
            fprintf(stdout, "\n");
        }

        /* Read full charge capacity */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_FULL_CHARGE_CAPACITY;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Full charge battery capacity  : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Full charge battery capacity  : ");
            if ((battery_mode & SM_BM_CAPACITY_MODE)==SM_BM_CAPACITY_MODE)
            {
                fprintf(stdout, "%d mWh", (cmd.data)*10);
            }
            else
            {
               fprintf(stdout, "%d mAh", cmd.data);
            }
            fprintf(stdout, "\n");
        }

        /* Read run time to empty */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_RUN_TIME_TO_EMPTY;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Run time to empty             : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Run time to empty             : ");
            print_time(cmd.data);
            if (cmd.data==65535)
            {
                fprintf(stdout, " (not applicable)");
            }
            fprintf(stdout, "\n");
        }

        /* Read average time to empty */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_AVERAGE_TIME_TO_EMPTY;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Average time to empty         : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Average time to empty         : ");
            print_time(cmd.data);
            if (cmd.data==65535)
            {
                fprintf(stdout, " (not applicable)");
            }
            fprintf(stdout, "\n");
        }

        /* Read average time to full */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_AVERAGE_TIME_TO_FULL;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Average time to full          : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Average time to full          : ");
            print_time(cmd.data);
            if (cmd.data==65535)
            {
               fprintf(stdout, " (not applicable) ");
            }
            fprintf(stdout, "\n");
        }

        /* Read battery status */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_BATTERY_STATUS;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Battery status                : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Battery status                : \n");
            if ((cmd.data & SB_BS_ALARM_MASK)!=0)
            {
                if ((cmd.data & SB_BS_OVER_CHARGED_ALARM)==SB_BS_OVER_CHARGED_ALARM)
                {
                    fprintf(stdout, "                                    ALARM: OVER CHARGED\n");
                }
                if ((cmd.data & SB_BS_TERMINATE_CHARGE_ALARM)==SB_BS_TERMINATE_CHARGE_ALARM)
                {
                    fprintf(stdout, "                                    ALARM: TERMINATE CHARGE\n");
                }
                if ((cmd.data & SB_BS_OVER_TEMP_ALARM)==SB_BS_OVER_TEMP_ALARM)
                {
                    fprintf(stdout, "                                    ALARM: OVER TEMP\n");
                }
                if ((cmd.data & SB_BS_TERMINATE_DISCHARGE_ALARM)==SB_BS_TERMINATE_DISCHARGE_ALARM)
                {
                    fprintf(stdout, "                                    ALARM: TERMINATE DISCHARGE\n");
                }
                if ((cmd.data & SB_BS_REMAINING_CAPACITY_ALARM)==SB_BS_REMAINING_CAPACITY_ALARM)
                {
                    fprintf(stdout, "                                    ALARM: REMAINING_CAPACITY\n");
                }
                if ((cmd.data & SB_BS_REMAINING_TIME_ALARM)==SB_BS_REMAINING_TIME_ALARM)
                {
                    fprintf(stdout, "                                    ALARM: REMAINING_TIME\n");
                }
            }
            else
            {
                fprintf(stdout, "                                    <No ALARMs>\n");
            }
            if ((cmd.data & SB_BS_INITIALIZED)==SB_BS_INITIALIZED)
            {
                fprintf(stdout, "                                    STATUS: INITIALIZED\n");
            }
            if ((cmd.data & SB_BS_DISCHARGING)==SB_BS_DISCHARGING)
            {
                fprintf(stdout, "                                    STATUS: DISCHARGING\n");
            }
            if ((cmd.data & SB_BS_FULLY_CHARGED)==SB_BS_FULLY_CHARGED)
            {
                fprintf(stdout, "                                    STATUS: FULLY CHARGED\n");
            }
            if ((cmd.data & SB_BS_FULLY_DISCHARGED)==SB_BS_FULLY_DISCHARGED)
            {
                fprintf(stdout, "                                    STATUS: FULLY DISCHARGED\n");
            }
            fprintf(stdout, "                                    ERROR: ");
            print_error((cmd.data & SB_BS_ERROR_CODE_MASK)>>SB_BS_ERROR_CODE_SHIFT);
            fprintf(stdout, "\n");
        }

        /* Read battery cycle count */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_CYCLE_COUNT;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Battery cycle count           : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Battery cycle count           : ");
            if (cmd.data==65535)
            {
               fprintf(stdout, "65535 or more");
            }
            else
            {
               fprintf(stdout, "%d", cmd.data);
            }
            fprintf(stdout, "\n");
        }

        /* Read battery design capacity */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_DESIGN_CAPACITY;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Design capacity               : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Design capacity               : ");
            if ((battery_mode & SM_BM_CAPACITY_MODE)==SM_BM_CAPACITY_MODE)
            {
                fprintf(stdout, "%d mWh", cmd.data*10);
            }
            else
            {
                fprintf(stdout, "%d mAh", cmd.data);
            }
            fprintf(stdout, "\n");
        }

        /* Read design voltage */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_DESIGN_VOLTAGE;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Design voltage                : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Design voltage                : ");
            fprintf(stdout, "%d mV", cmd.data);
            fprintf(stdout, "\n");
        }

        /* Read batteries information */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_SPECIFICATION_INFO;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fprintf(stderr, "    Smart battery revision        : Communication error: %s\n", strerror(status));
        }
        else
        {
            fprintf(stdout, "    Smart Battery revision        : ");
            fprintf(stdout, "%04X", ((cmd.data & SB_SI_REVISION_MASK)>>SB_SI_REVISION_SHIFT));
            fprintf(stdout, "\n");

            fprintf(stdout, "    Smart Battery version         : ");
            if ((cmd.data & SB_SI_VERSION_MASK)==SB_SI_VERSION_1_0)
            {
                fprintf(stdout, "Version 1.0\n");
            }
            else
            {
                if ((cmd.data & SB_SI_VERSION_MASK)==SB_SI_VERSION_1_1)
                {
                    fprintf(stdout, "Version 1.1\n");
                }
                else
                {
                    if ((cmd.data & SB_SI_VERSION_MASK)==SB_SI_VERSION_1_1_PEC)
                    {
                        fprintf(stdout, "Version 1.1 with optional PEC\n");
                    }
                    else
                    {
                        fprintf(stdout, "Version unknown (%01X)\n",
                            (cmd.data & SB_SI_VERSION_MASK)>>SB_SI_VERSION_SHIFT);
                    }
                }
            }

            fprintf(stdout, "    Battery current scale         : ");
            fprintf(stdout, "10^%d\n", (cmd.data & SB_SI_IPSCALE_MASK) >> SB_SI_IPSCALE_SHIFT);
            fprintf(stdout, "    Battery voltage scale         : ");
            fprintf(stdout, "10^%d\n", (cmd.data & SB_SI_VSCALE_MASK) >> SB_SI_VSCALE_SHIFT);
        }
    }

    close(fd);

    exit(0);
}
