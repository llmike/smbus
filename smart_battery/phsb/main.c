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
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include <Ph.h>
#include <Pt.h>
#include <photon/PtProto.h>
#include <photon/PkKeyDef.h>

#include <dcmd_smbus.h>

#include "sb_defines.h"
#include "images.h"

int shutdown=0;

PtWidget_t* phwindow;
PtWidget_t* phoscontainer;
PtWidget_t* phbattery;
PtWidget_t* phprogress;
PtWidget_t* phtemperature;
PtWidget_t* phtime;

PhImage_t* empty_charge_img;
PhImage_t* one_fourth_charge_img;
PhImage_t* half_charge_img;
PhImage_t* three_fourth_charge_img;
PhImage_t* full_charge_img;
PhImage_t* charging_img;

int init_draw()
{
    empty_charge_img=PhCreateImage(NULL, BATTERY_IMAGE_WIDTH, BATTERY_IMAGE_HEIGHT, BATTERY_IMAGE_FORMAT, NULL, 0, 0);
    PhReleaseImage(empty_charge_img);
    empty_charge_img->image=(char*)empty_charge_image;
    empty_charge_img->bpl=BATTERY_IMAGE_WIDTH*3;
    empty_charge_img->flags=0;
    PhMakeTransparent(empty_charge_img, PgRGB(0xFF, 0x00, 0xFF));

    one_fourth_charge_img=PhCreateImage(NULL, BATTERY_IMAGE_WIDTH, BATTERY_IMAGE_HEIGHT, BATTERY_IMAGE_FORMAT, NULL, 0, 0);
    PhReleaseImage(one_fourth_charge_img);
    one_fourth_charge_img->image=(char*)one_fourth_charge_image;
    one_fourth_charge_img->bpl=BATTERY_IMAGE_WIDTH*3;
    one_fourth_charge_img->flags=0;
    PhMakeTransparent(one_fourth_charge_img, PgRGB(0xFF, 0x00, 0xFF));

    half_charge_img=PhCreateImage(NULL, BATTERY_IMAGE_WIDTH, BATTERY_IMAGE_HEIGHT, BATTERY_IMAGE_FORMAT, NULL, 0, 0);
    PhReleaseImage(half_charge_img);
    half_charge_img->image=(char*)half_charge_image;
    half_charge_img->bpl=BATTERY_IMAGE_WIDTH*3;
    half_charge_img->flags=0;
    PhMakeTransparent(half_charge_img, PgRGB(0xFF, 0x00, 0xFF));

    three_fourth_charge_img=PhCreateImage(NULL, BATTERY_IMAGE_WIDTH, BATTERY_IMAGE_HEIGHT, BATTERY_IMAGE_FORMAT, NULL, 0, 0);
    PhReleaseImage(three_fourth_charge_img);
    three_fourth_charge_img->image=(char*)three_fourth_charge_image;
    three_fourth_charge_img->bpl=BATTERY_IMAGE_WIDTH*3;
    three_fourth_charge_img->flags=0;
    PhMakeTransparent(three_fourth_charge_img, PgRGB(0xFF, 0x00, 0xFF));

    full_charge_img=PhCreateImage(NULL, BATTERY_IMAGE_WIDTH, BATTERY_IMAGE_HEIGHT, BATTERY_IMAGE_FORMAT, NULL, 0, 0);
    PhReleaseImage(full_charge_img);
    full_charge_img->image=(char*)full_charge_image;
    full_charge_img->bpl=BATTERY_IMAGE_WIDTH*3;
    full_charge_img->flags=0;
    PhMakeTransparent(full_charge_img, PgRGB(0xFF, 0x00, 0xFF));

    charging_img=PhCreateImage(NULL, BATTERY_IMAGE_WIDTH, BATTERY_IMAGE_HEIGHT, BATTERY_IMAGE_FORMAT, NULL, 0, 0);
    PhReleaseImage(charging_img);
    charging_img->image=(char*)charging_image;
    charging_img->bpl=BATTERY_IMAGE_WIDTH*3;
    charging_img->flags=0;
    PhMakeTransparent(charging_img, PgRGB(0xFF, 0x00, 0xFF));

    return 0;
}

int fini_draw()
{
    free(empty_charge_img);
    free(one_fourth_charge_img);
    free(half_charge_img);
    free(three_fourth_charge_img);
    free(full_charge_img);
    free(charging_img);

    return 0;
}

int draw()
{
    PtFlush();

    return 0;
}

int create_window(int width, int height)
{
    PhDim_t    winsize;
    PhPoint_t  position;
    PtArg_t    winargs[32];
    uint32_t   winargc = 0;
    int32_t    status;

    PtSetArg(&winargs[winargc++], Pt_ARG_WINDOW_TITLE, "BAT: n/a", 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_BASIC_FLAGS, Pt_TRUE, Pt_BASIC_PREVENT_FILL);
    PtSetArg(&winargs[winargc++], Pt_ARG_WINDOW_MANAGED_FLAGS, Pt_FALSE, Ph_WM_APP_DEF_MANAGED);
    PtSetArg(&winargs[winargc++], Pt_ARG_WINDOW_MANAGED_FLAGS, Pt_TRUE,
             Ph_WM_BACKDROP | Ph_WM_TOFRONT | Ph_WM_COLLAPSE | Ph_WM_FFRONT |
             Ph_WM_FOCUS | Ph_WM_HELP | Ph_WM_HIDE | Ph_WM_MAX |
             Ph_WM_MENU | Ph_WM_MOVE | Ph_WM_RESTORE | Ph_WM_TASKBAR |
             Ph_WM_TOBACK | Ph_WM_RESIZE);
    PtSetArg(&winargs[winargc++], Pt_ARG_WINDOW_NOTIFY_FLAGS, Pt_FALSE,
             Ph_WM_RESIZE | Ph_WM_CLOSE | Ph_WM_HELP);
    PtSetArg(&winargs[winargc++], Pt_ARG_WINDOW_NOTIFY_FLAGS, Pt_TRUE,
             Ph_WM_CLOSE | Ph_WM_COLLAPSE | Ph_WM_FOCUS | Ph_WM_MAX |
             Ph_WM_MOVE | Ph_WM_RESIZE | Ph_WM_RESTORE | Ph_WM_HIDE);
    PtSetArg(&winargs[winargc++], Pt_ARG_WINDOW_RENDER_FLAGS, Pt_FALSE,
             Ph_WM_APP_DEF_RENDER | Ph_WM_RENDER_MAX);
    PtSetArg(&winargs[winargc++], Pt_ARG_WINDOW_RENDER_FLAGS, Pt_TRUE,
             Ph_WM_RENDER_CLOSE | Ph_WM_RENDER_MENU | Ph_WM_RENDER_MIN |
             Ph_WM_RENDER_TITLE | Ph_WM_RENDER_MOVE |
             Ph_WM_RENDER_COLLAPSE |  Ph_WM_RENDER_ASAPP);
    PtSetArg(&winargs[winargc++], Pt_ARG_WINDOW_STATE, Pt_TRUE,
             Ph_WM_STATE_ISFOCUS);
    PtSetArg(&winargs[winargc++], Pt_ARG_WINDOW_MANAGED_FLAGS, Pt_FALSE,
             Ph_WM_MAX | Ph_WM_RESTORE | Ph_WM_RESIZE);

    /* Set window dimension */
    winsize.w = width;
    winsize.h = height;
    PtSetArg(&winargs[winargc++], Pt_ARG_DIM, &winsize, 0);

    /* Finally create the window */
    phwindow = PtCreateWidget(PtWindow, Pt_NO_PARENT, winargc, winargs);
    if (phwindow == NULL)
    {
        return -1;
    }

    /* Create PtOSContainer widget for flicker-free updating */
    winargc=0;
    winsize.w = width;
    winsize.h = height;
    PtSetArg(&winargs[winargc++], Pt_ARG_DIM, &winsize, 0);

    /* Create the PtOSContainer widget */
    phoscontainer = PtCreateWidget(PtOSContainer, phwindow, winargc, winargs);
    if (phoscontainer == NULL)
    {
        return -1;
    }

    /* Create PtLabel widget for battery image */
    winargc=0;
    winsize.w=BATTERY_IMAGE_WIDTH;
    winsize.h=BATTERY_IMAGE_HEIGHT;
    position.x=1;
    position.y=2;
    PtSetArg(&winargs[winargc++], Pt_ARG_DIM, &winsize, 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_POS, &position, 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_LABEL_TYPE, Pt_IMAGE, 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_LABEL_IMAGE, empty_charge_img, 0);

    /* Create the PtLabel widget */
    phbattery = PtCreateWidget(PtLabel, phoscontainer, winargc, winargs);
    if (phbattery == NULL)
    {
        return -1;
    }

    /* Create PtProgress widget for battery state */
    winargc=0;
    winsize.w=200-BATTERY_IMAGE_WIDTH-4-5;
    winsize.h=BATTERY_IMAGE_HEIGHT+2;
    position.x=2+4+BATTERY_IMAGE_WIDTH;
    position.y=2+1;
    PtSetArg(&winargs[winargc++], Pt_ARG_DIM, &winsize, 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_POS, &position, 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_PROGRESS_BAR_COLOR, PgRGB(0x6F, 0x6F, 0x6F), 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_FILL_COLOR, PgRGB(0xFE, 0xB9, 0x27), 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_GAUGE_FLAGS, Pt_TRUE, Pt_GAUGE_SHOW_VALUE);
    PtSetArg(&winargs[winargc++], Pt_ARG_GAUGE_FONT, "TextFont10b", 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_GAUGE_VALUE_SUFFIX, "%", 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_GAUGE_VALUE, 0, 0);

    /* Create the PtProgress widget */
    phprogress = PtCreateWidget(PtProgress, phoscontainer, winargc, winargs);
    if (phprogress == NULL)
    {
        return -1;
    }

    /* Create PtLabel widget for battery temperature */
    winargc=0;
    winsize.w=BATTERY_IMAGE_WIDTH;
    winsize.h=20;
    position.x=4;
    position.y=28;
    PtSetArg(&winargs[winargc++], Pt_ARG_DIM, &winsize, 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_POS, &position, 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_LABEL_TYPE, Pt_Z_STRING, 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_TEXT_STRING, "t n/a", 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_TEXT_FONT, "TextFont10b", 0);

    /* Create the PtLabel widget */
    phtemperature = PtCreateWidget(PtLabel, phoscontainer, winargc, winargs);
    if (phtemperature == NULL)
    {
        return -1;
    }

    /* Create PtLabel widget for battery temperature */
    winargc=0;
    winsize.w=BATTERY_IMAGE_WIDTH;
    winsize.h=20;
    position.x=4+BATTERY_IMAGE_WIDTH+20;
    position.y=28;
    PtSetArg(&winargs[winargc++], Pt_ARG_DIM, &winsize, 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_POS, &position, 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_LABEL_TYPE, Pt_Z_STRING, 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_TEXT_STRING, "REST:    n/a  ", 0);
    PtSetArg(&winargs[winargc++], Pt_ARG_TEXT_FONT, "TextFont10b", 0);

    /* Create the PtLabel widget */
    phtime = PtCreateWidget(PtLabel, phoscontainer, winargc, winargs);
    if (phtime == NULL)
    {
        return -1;
    }

    /* Show widget */
    status = PtRealizeWidget(phwindow);
    if (status != 0)
    {
        PtDestroyWidget(phwindow);
    }

    /* Flush all widget operations */
    PtFlush();

    /* Window has been successfully created */
    return 0;
}

static int fd=-1;

void update_battery_state()
{
    int              ac_present=0;
    int              battery_charge_percent=0;
    int              status;
    uint16_t         battery_time;
    int16_t          battery_temperature;
    smbus_cmd_data_t cmd;

    if (fd<0)
    {
        int smbus_id;

        /* Go through all available SMBUSes */
        for (smbus_id=0; smbus_id<=99; smbus_id++)
        {
            char smbus_name[PATH_MAX];

            snprintf(smbus_name, PATH_MAX, "/dev/smb%d", smbus_id);

            fd=open(smbus_name, O_RDWR);
            if (fd>=0)
            {
                /* Try to determine if we smart batter is present */
                cmd.cmd_type=SMBUS_CMD_WORD_DATA;
                cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
                cmd.smbus_cmd=SB_BATTERY_STATUS;
                cmd.length=0;
                status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
                if (status!=EOK)
                {
                    fd=-1;
                    /* If battery is not detected, then skip this bus */
                    continue;
                }
                else
                {
                    break;
                }
            }
        }
    }

    /* Check if we have found smbus, then update status */
    if (fd>=0)
    {
        char tempbuff[32];

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
        else
        {
            /* Don't fail in case if smart charger is not responding */
            ac_present=0;
        }

        /* Read relative state of charge */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_RELATIVE_STATE_OF_CHARGE;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fd=-1;
            return;
        }
        else
        {
            battery_charge_percent=cmd.data;
            if (battery_charge_percent>100)
            {
                battery_charge_percent=100;
            }
        }

        if (ac_present)
        {
            /* Read average time to full */
            cmd.cmd_type=SMBUS_CMD_WORD_DATA;
            cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
            cmd.smbus_cmd=SB_AVERAGE_TIME_TO_FULL;
            cmd.length=0;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                fd=-1;
                return;
            }
            else
            {
                battery_time=cmd.data;
            }
        }
        else
        {
            /* Read average time to empty */
            cmd.cmd_type=SMBUS_CMD_WORD_DATA;
            cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
            cmd.smbus_cmd=SB_AVERAGE_TIME_TO_EMPTY;
            cmd.length=0;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                fd=-1;
                return;
            }
            else
            {
                if (cmd.data==65535)
                {
                    /* Read average time to full */
                    cmd.cmd_type=SMBUS_CMD_WORD_DATA;
                    cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
                    cmd.smbus_cmd=SB_AVERAGE_TIME_TO_FULL;
                    cmd.length=0;
                    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
                    if (status!=EOK)
                    {
                        fd=-1;
                        return;
                    }
                    else
                    {
                        /* This is a special case. Some batteries do not provide time to full, */
                        /* but when time to empty and time to full both are 65536, this means  */
                        /* that AC power is connected.                                         */
                        ac_present=1;
                    }
                }
                battery_time=cmd.data;
            }
        }

        /* Read battery temperature */
        cmd.cmd_type=SMBUS_CMD_WORD_DATA;
        cmd.slv_addr=SB_SMART_BATTERY_ADDRESS;
        cmd.smbus_cmd=SB_TEMPERATURE;
        cmd.length=0;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            fd=-1;
            return;
        }
        else
        {
            battery_temperature=cmd.data;
        }

        /* Update window title */
        snprintf(tempbuff, 32, "BAT: %d%%", battery_charge_percent);
        PtSetResource(phwindow, Pt_ARG_WINDOW_TITLE, tempbuff, strlen(tempbuff)+1);

        /* Update progress bar */
        PtSetResource(phprogress, Pt_ARG_GAUGE_VALUE, battery_charge_percent, sizeof(long));
        PtDamageWidget(phprogress);

        /* Update battery time */
        if (battery_time==65535)
        {
            PtSetResource(phtime, Pt_ARG_TEXT_STRING, "REST:    n/a  ", 15);
            PtDamageWidget(phtime);
        }
        else
        {
            snprintf(tempbuff, 32, "REST: %dd %02d:%02d", battery_time/60/24, (battery_time - battery_time/60/24*24*60)/60,
                (battery_time - battery_time/60/24*24*60 - (battery_time - battery_time/60/24*24*60)/60*60));
            PtSetResource(phtime, Pt_ARG_TEXT_STRING, tempbuff, strlen(tempbuff)+1);
            PtDamageWidget(phtime);
        }

        /* Update battery temperature */
        if (battery_temperature<2731)
        {
            snprintf(tempbuff, 32, "t-%d.%dC", abs((battery_temperature-2731))/10,
                abs((battery_temperature-2731))-abs((battery_temperature-2731))/10*10);
        }
        else
        {
            snprintf(tempbuff, 32, "t+%d.%dC", (battery_temperature-2731)/10,
                (battery_temperature-2731)-(battery_temperature-2731)/10*10);
        }
        PtSetResource(phtemperature, Pt_ARG_TEXT_STRING, tempbuff, strlen(tempbuff)+1);
        PtDamageWidget(phtemperature);

        /* Update battery image */
        if (ac_present)
        {
            PtSetResource(phbattery, Pt_ARG_LABEL_IMAGE, charging_img, sizeof(PhImage_t*));
        }
        else
        {
            if ((battery_charge_percent>=0)&&(battery_charge_percent<12))
            {
                PtSetResource(phbattery, Pt_ARG_LABEL_IMAGE, empty_charge_img, sizeof(PhImage_t*));
            }
            else
            {
                if ((battery_charge_percent>=12)&&(battery_charge_percent<37))
                {
                    PtSetResource(phbattery, Pt_ARG_LABEL_IMAGE, one_fourth_charge_img, sizeof(PhImage_t*));
                }
                else
                {
                    if ((battery_charge_percent>=37)&&(battery_charge_percent<63))
                    {
                        PtSetResource(phbattery, Pt_ARG_LABEL_IMAGE, half_charge_img, sizeof(PhImage_t*));
                    }
                    else
                    {
                        if ((battery_charge_percent>=63)&&(battery_charge_percent<87))
                        {
                            PtSetResource(phbattery, Pt_ARG_LABEL_IMAGE, three_fourth_charge_img, sizeof(PhImage_t*));
                        }
                        else
                        {
                            if ((battery_charge_percent>=87)&&(battery_charge_percent<=100))
                            {
                                PtSetResource(phbattery, Pt_ARG_LABEL_IMAGE, full_charge_img, sizeof(PhImage_t*));
                            }
                        }
                    }
                }
            }
        }
        PtDamageWidget(phbattery);

        PtFlush();
    }
    else
    {
        /* Reset resources */
        PtSetResource(phbattery, Pt_ARG_LABEL_IMAGE, empty_charge_img, sizeof(PhImage_t*));
        PtDamageWidget(phbattery);
        PtSetResource(phtemperature, Pt_ARG_TEXT_STRING, "t n/a", 7);
        PtDamageWidget(phtemperature);
        PtSetResource(phtime, Pt_ARG_TEXT_STRING, "REST:    n/a  ", 15);
        PtDamageWidget(phtime);
        PtSetResource(phprogress, Pt_ARG_GAUGE_VALUE, 0, sizeof(long));
        PtDamageWidget(phprogress);
        PtSetResource(phwindow, Pt_ARG_WINDOW_TITLE, "BAT: n/a", 8);

        PtFlush();
    }
}

void handle_window_event()
{
    uint8_t    eventbuffer[8192];
    PhEvent_t* event=(PhEvent_t *) eventbuffer;
    int32_t    status;
    uint32_t   finish=0;

    do {
        status = PhEventPeek(event, 8192);
        switch (status)
        {
            case Ph_RESIZE_MSG:
                 {
                     fprintf(stderr, "Photon: Event size too large for buffer\n");
                     return;
                 }
                 break;
            case Ph_EVENT_MSG:
                 {
                     /* Pass event to Widgets Toolkit */
                     PtEventHandler(event);

                     /* Event is ready */
                     switch (event->type)
                     {
                         case Ph_EV_EXPOSE:
                              {
                                  draw();
                              }
                              break;
                         case Ph_EV_KEY:
                              {
                                  PhKeyEvent_t *keyevent = NULL;
                                  int pressed = 0;

                                  keyevent = PhGetData(event);
                                  if (keyevent == NULL)
                                  {
                                      break;
                                  }

                                  /* Check if key is repeated */
                                  if ((keyevent->key_flags & Pk_KF_Key_Repeat) == Pk_KF_Key_Repeat)
                                  {
                                      /* Ignore such events */
                                      break;
                                  }

                        /* Check if key has its own scancode */
                        if ((keyevent->key_flags & Pk_KF_Scan_Valid) ==
                            Pk_KF_Scan_Valid)
                        {
                            if ((keyevent->key_flags & Pk_KF_Key_Down) ==
                                Pk_KF_Key_Down)
                            {
                                pressed = 1;
                            }
                            else
                            {
                                pressed = 0;
                            }

                            /* ESC is pressed */
                            if (keyevent->key_scan==1)
                            {
                                shutdown=1;
                            }
                        }
                    }
                    break;
                case Ph_EV_WM:
                    {
                        PhWindowEvent_t *wmevent = NULL;

                        /* Get associated event data */
                        wmevent = PhGetData(event);
                        if (wmevent == NULL) {
                            break;
                        }

                        switch (wmevent->event_f) {
                        case Ph_WM_CLOSE:
                            {
                                shutdown=1;
                            }
                            break;
                        }
                    }
                    break;
                }
                PtEventHandler(event);
            }
            break;
        case 0:
            {
                /* All events are read */
                finish = 1;
                break;
            }
        case -1:
            {
                /* Error occured in event reading */
                fprintf(stderr, "Photon: Can't read event\n");
                return;
            }
            break;
        }
        if (finish != 0)
        {
            break;
        }
    } while (1);
}

int main()
{
    struct timespec ts={0, 1};
    struct timeval  time_old;
    struct timeval  time_new;
    int             first_run=1;

    if (PtInit(NULL)==-1)
    {
        fprintf(stderr, "Can't connect to Photon\n");
        return 1;
    }

    if (init_draw()==-1)
    {
        fprintf(stderr, "Can't initialize images\n");
        return 1;
    }

    create_window(200, 50);

    draw();

    gettimeofday(&time_old, NULL);

    do {
        handle_window_event();
        gettimeofday(&time_new, NULL);
        if ((time_new.tv_sec-time_old.tv_sec>=3)||(first_run)||(time_old.tv_sec>-time_new.tv_sec))
        {
            time_old=time_new;
            first_run=0;
            update_battery_state();
        }
        nanosleep(&ts, NULL);
        if (shutdown==1)
        {
            break;
        }
    } while(1);

    if (fini_draw()==-1)
    {
        fprintf(stderr, "Can't deinitialize images\n");
        return 1;
    }

    return 0;
}
