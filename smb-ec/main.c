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

#include <sys/rsrcdbmgr.h>

#include "proto.h"

static ec_dev_t* ec;

static void exit_signal(int signo)
{
    free(ec);
    exit(EXIT_FAILURE);
}

static void start(ec_dev_t* dev)
{
    char                   name[PATH_MAX];
    struct sigaction       sa;
    resmgr_io_funcs_t      io_funcs;
    resmgr_connect_funcs_t connect_funcs;
    resmgr_attr_t          rattr;
    iofunc_funcs_t         ocb_funcs = {_IOFUNC_NFUNCS, _ec_ocb_calloc, _ec_ocb_free};
    iofunc_mount_t         mount = {0, 0, 0, 0, &ocb_funcs};
    int                    it;

    if ((dev->dpp = dispatch_create()) == NULL)
    {
        perror("dispatch_create() failed");
        exit(EXIT_FAILURE);
    }

    memset(&rattr, 0, sizeof(rattr));

    iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs,
                     _RESMGR_IO_NFUNCS, &io_funcs);
    io_funcs.devctl = ec_devctl;
    iofunc_attr_init(&dev->hdr, S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP |
        S_IWGRP | S_IROTH | S_IWOTH, NULL, NULL);

    dev->hdr.rdev=rsrcdbmgr_devno_attach("smb", -1, 0);
    if (dev->hdr.rdev==-1)
    {
        perror("rsrcdbmgr_devno_attach() failed");
        exit(EXIT_FAILURE);
    }

    dev->hdr.mount = &mount;

    /* Find free device name */
    for (it=0; it<=99; it++)
    {
        snprintf(name, PATH_MAX, "/dev/smb%d", it);

        if (access(name, F_OK)==0)
        {
            continue;
        }

        if (resmgr_attach(dev->dpp, &rattr, name, _FTYPE_ANY, 0,
                          &connect_funcs, &io_funcs, (RESMGR_HANDLE_T*)dev) == -1)
        {
            if (it!=99)
            {
                continue;
            }
            else
            {
                perror("resmgr_attach() failed");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            break;
        }
    }

    if ((dev->ctp = resmgr_context_alloc(dev->dpp)) == NULL)
    {
        perror("resmgr_context_alloc() failed");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGTERM);
    sa.sa_handler = exit_signal;
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, NULL);

    /* Initialize hardware access mutex */
    pthread_mutex_init(&dev->ioaccess, NULL);

    procmgr_daemon(0, PROCMGR_DAEMON_NOCLOSE);

    while (1)
    {
        if (resmgr_block(dev->ctp) != NULL)
        {
            resmgr_handler(dev->ctp);
        }
        else
        {
            /* Deinitialize hardware access mutex */
            pthread_mutex_destroy(&dev->ioaccess);

            free(dev);
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char* argv[])
{
    int         c;
    ec_dev_t* dev;

    if (ThreadCtl(_NTO_TCTL_IO, NULL) == -1)
    {
        return (EPERM);
    }

    ec = dev = calloc(1, sizeof(ec_dev_t));

    if (dev == NULL)
    {
        return -1;
    }

    /* Initialize defaults */
    dev->priority=21;
    dev->smbus_offset=-1;

    while (optind < argc)
    {
        if ((c = getopt(argc, argv, "o:p:v")) == -1)
        {
            optind++;
            continue;
        }
        switch (c)
        {
            case 'p':
                 dev->priority=atoi(optarg);
                 break;
            case 'o':
                 dev->smbus_offset=atoi(optarg);
                 break;
            case 'v':
                 dev->verbose++;
                 break;
        }
    }

    if (ec_init(dev, 0)!=0)
    {
        fprintf(stderr, "[smb-ec] Initialization failed\n");
        free(dev);
        return -1;
    }

    start(dev);

    return 0;
}
