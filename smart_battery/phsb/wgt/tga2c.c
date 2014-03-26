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
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <inttypes.h>

#pragma pack(1)
typedef struct
{
    uint8_t  identsize;          // size of ID field that follows 18 uint8_t header (0 usually)
    uint8_t  colourmaptype;      // type of colour map 0=none, 1=has palette
    uint8_t  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    uint16_t colourmapstart;     // first colour map entry in palette
    uint16_t colourmaplength;    // number of colours in palette
    uint8_t  colourmapbits;      // number of bits per palette entry 15,16,24,32

    uint16_t xstart;             // image x origin
    uint16_t ystart;             // image y origin
    uint16_t width;              // image width in pixels
    uint16_t height;             // image height in pixels
    uint8_t  bits;               // image bits per pixel 8,16,24,32
    uint8_t  descriptor;         // image descriptor bits (vh flip bits)

    // pixel data follows header

} TGA_HEADER;
#pragma pack(0)

uint8_t buffer[16384];

int main(int argc, char* argv[])
{
    int fd;
    TGA_HEADER hdr;
    int it, jt;

    if (argc!=2)
    {
        fprintf(stderr, "Usage: tga2c filename.tga\n");
        return -1;
    }

    fd=open(argv[1], O_RDONLY);
    if (fd==-1)
    {
        fprintf(stderr, "Can't open %s file\n", argv[1]);
        return -1;
    }

    if (read(fd, &hdr, 18)!=18)
    {
        close(fd);
        fprintf(stderr, "Can't read %s file, too short\n", argv[1]);
        return -1;
    }

    fprintf(stderr, "Image width %d pixels\n", hdr.width);
    fprintf(stderr, "Image height %d pixels\n", hdr.height);
    fprintf(stderr, "Image color bits %d\n", hdr.bits);
    if ((hdr.imagetype & 0x03)!=2)
    {
        fprintf(stderr, "Non-RGB images are not supported!\n");
        return -1;
    }
    if (hdr.imagetype & 0x08)
    {
        fprintf(stderr, "RLE packed images are not supported\n");
        return -1;
    }

    fprintf(stdout, "unsigned char image[%d][%d*%d]=\n", hdr.height, hdr.width, hdr.bits/8);
    fprintf(stdout, "{\n");
    for (jt=0; jt<hdr.height; jt++)
    {
        fprintf(stdout, "    {");
        if (read(fd, buffer, hdr.width*hdr.bits/8)!=hdr.width*hdr.bits/8)
        {
            close(fd);
            fprintf(stderr, "Can't read %s file, too short\n", argv[1]);
            return -1;
        }
        for (it=0; it<hdr.width*hdr.bits/8; it++)
        {
            if ((it%12==0)&&(it!=0))
            {
                fprintf(stdout, "\n");
                fprintf(stdout, "     ");
            }
            if (it==hdr.width*hdr.bits/8-1)
            {
                fprintf(stdout, "0x%02X", buffer[it]);
            }
            else
            {
                if ((it+1)%12==0)
                {
                    fprintf(stdout, "0x%02X,", buffer[it]);
                }
                else
                {
                    fprintf(stdout, "0x%02X, ", buffer[it]);
                }
            }
        }
        fprintf(stdout, "},\n");
    }
    fprintf(stdout, "};\n");

    close(fd);

    return 0;
}
