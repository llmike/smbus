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

/* All definitions are made using JEDEC STANDARD JEP106AH, January 2012.    */
/* Standard Manufacturer's Identidication Code. (Last update May 30, 2012)  */

#include "spd_manufacturer.h"

const char* jedec_manufacturer[8][128]=
{
    /* bank 1 */
    {
        "Pad",                                                      /* 0x00 */
        "AMD",                                                      /* 0x01 */
        "AMI",                                                      /* 0x02 */
        "Fairchild",                                                /* 0x03 */
        "Fujitsu",                                                  /* 0x04 */
        "GTE",                                                      /* 0x05 */
        "Harris",                                                   /* 0x06 */
        "Hitachi",                                                  /* 0x07 */
        "Inmos",                                                    /* 0x08 */
        "Intel",                                                    /* 0x09 */
        "I.T.T.",                                                   /* 0x0A */
        "Intersil",                                                 /* 0x0B */
        "Monolythic Memories",                                      /* 0x0C */
        "Mostek",                                                   /* 0x0D */
        "Freescale (Motorola)",                                     /* 0x0E */
        "National",                                                 /* 0x0F */
        "NEC",                                                      /* 0x10 */
        "RCA",                                                      /* 0x11 */
        "Raytheon",                                                 /* 0x12 */
        "Conexant (Rockwell)",                                      /* 0x13 */
        "Seeq",                                                     /* 0x14 */
        "NXP (Philips)",                                            /* 0x15 */
        "Synertek",                                                 /* 0x16 */
        "Texas Instruments",                                        /* 0x17 */
        "Toshiba",                                                  /* 0x18 */
        "Xicor",                                                    /* 0x19 */
        "Zilog",                                                    /* 0x1A */
        "Eurotechnique",                                            /* 0x1B */
        "Mitsubishi",                                               /* 0x1C */
        "Lucent (AT&T)",                                            /* 0x1D */
        "Exel",                                                     /* 0x1E */
        "Atmel",                                                    /* 0x1F */
        "SGS/Thomson",                                              /* 0x20 */
        "Lattice Semi.",                                            /* 0x21 */
        "NCR",                                                      /* 0x22 */
        "Wafer Scale Integration",                                  /* 0x23 */
        "IBM",                                                      /* 0x24 */
        "Tristar",                                                  /* 0x25 */
        "Visic",                                                    /* 0x26 */
        "Intl. CMOS Technology",                                    /* 0x27 */
        "SSSI",                                                     /* 0x28 */
        "MicrochipTechnology",                                      /* 0x29 */
        "Ricoh Ltd.",                                               /* 0x2A */
        "VLSI",                                                     /* 0x2B */
        "Micron Technology",                                        /* 0x2C */
        "Hynix Semiconductor (Hyundai Electronics)",                /* 0x2D */
        "OKI Semiconductor",                                        /* 0x2E */
        "ACTEL",                                                    /* 0x2F */
        "Sharp",                                                    /* 0x30 */
        "Catalyst",                                                 /* 0x31 */
        "Panasonic",                                                /* 0x32 */
        "IDT",                                                      /* 0x33 */
        "Cypress",                                                  /* 0x34 */
        "DEC",                                                      /* 0x35 */
        "LSI Logic",                                                /* 0x36 */
        "Zarlink (Plessey)",                                        /* 0x37 */
        "UTMC",                                                     /* 0x38 */
        "Thinking Machine",                                         /* 0x39 */
        "Thomson CSF",                                              /* 0x3A */
        "Integrated CMOS (Vertex)",                                 /* 0x3B */
        "Honeywell",                                                /* 0x3C */
        "Tektronix",                                                /* 0x3D */
        "Oracle Corporation",                                       /* 0x3E */
        "Silicon Storage Technology",                               /* 0x3F */
        "ProMos/Mosel Vitelic",                                     /* 0x40 */
        "Infineon (Siemens)",                                       /* 0x41 */
        "Macronix",                                                 /* 0x42 */
        "Xerox",                                                    /* 0x43 */
        "Plus Logic",                                               /* 0x44 */
        "SanDisk Corporation",                                      /* 0x45 */
        "Elan Circuit Tech.",                                       /* 0x46 */
        "European Silicon Str.",                                    /* 0x47 */
        "Apple Computer",                                           /* 0x48 */
        "Xilinx",                                                   /* 0x49 */
        "Compaq",                                                   /* 0x4A */
        "Protocol Engines",                                         /* 0x4B */
        "SCI",                                                      /* 0x4C */
        "Seiko Instruments",                                        /* 0x4D */
        "Samsung",                                                  /* 0x4E */
        "I3 Design Systems",                                        /* 0x4F */
        "Klic",                                                     /* 0x50 */
        "Crosspoint Solutions",                                     /* 0x51 */
        "Alliance Semiconductor",                                   /* 0x52 */
        "Tandem",                                                   /* 0x53 */
        "Hewlet-Packard",                                           /* 0x54 */
        "Integrated Silicon Solutions",                             /* 0x55 */
        "Brooktree",                                                /* 0x56 */
        "New Media",                                                /* 0x57 */
        "MHS Electronic",                                           /* 0x58 */
        "Performance Semi.",                                        /* 0x59 */
        "Winbond Electronic",                                       /* 0x5A */
        "Kawasaki Steel",                                           /* 0x5B */
        "Bright Micro",                                             /* 0x5C */
        "TECMAR",                                                   /* 0x5D */
        "Exar",                                                     /* 0x5E */
        "PCMCIA",                                                   /* 0x5F */
        "LG Semi (Goldstar)",                                       /* 0x60 */
        "Northern Telecom",                                         /* 0x61 */
        "Sanyo",                                                    /* 0x62 */
        "Array Microsystems",                                       /* 0x63 */
        "Crystal Semiconductor",                                    /* 0x64 */
        "Analog Devices",                                           /* 0x65 */
        "PMC-Sierra",                                               /* 0x66 */
        "Asparix",                                                  /* 0x67 */
        "Convex Computer",                                          /* 0x68 */
        "Quality Semiconductor",                                    /* 0x69 */
        "Nimbus Technology",                                        /* 0x6A */
        "Transwitch",                                               /* 0x6B */
        "Micronas (ITT Intermetall)",                               /* 0x6C */
        "Cannon",                                                   /* 0x6D */
        "Altera",                                                   /* 0x6E */
        "NEXCOM",                                                   /* 0x6F */
        "QUALCOMM",                                                 /* 0x70 */
        "Sony",                                                     /* 0x71 */
        "Cray Research",                                            /* 0x72 */
        "AMS (Austria Micro)",                                      /* 0x73 */
        "Vitesse",                                                  /* 0x74 */
        "Aster Electronic",                                         /* 0x75 */
        "Bay Networks (Synoptic)",                                  /* 0x76 */
        "Zentrum/ZMD",                                              /* 0x77 */
        "TRW",                                                      /* 0x78 */
        "Thesis",                                                   /* 0x79 */
        "Solbourne Computer",                                       /* 0x7A */
        "Allied-Signal",                                            /* 0x7B */
        "Digital Semiconductor",                                    /* 0x7C */
        "Media Vision",                                             /* 0x7D */
        "Numonyx Corporation",                                      /* 0x7E */
        "Continue",                                                 /* 0x7F */
    },

    /* bank 2 */
    {
        "Pad",                                                      /* 0x00 */
        "Cirrus Logic",                                             /* 0x01 */
        "National Instruments",                                     /* 0x02 */
        "ILC Data Device",                                          /* 0x03 */
        "Alcatel Mietec",                                           /* 0x04 */
        "Micro Linear",                                             /* 0x05 */
        "Univ. of NC",                                              /* 0x06 */
        "JTAG Technologies",                                        /* 0x07 */
        "BAE Systems",                                              /* 0x08 */
        "Nchip",                                                    /* 0x09 */
        "Galileo Tech",                                             /* 0x0A */
        "Bestlink Systems",                                         /* 0x0B */
        "Graychip",                                                 /* 0x0C */
        "GENNUM",                                                   /* 0x0D */
        "Video Logic",                                              /* 0x0E */
        "Robert Bosch",                                             /* 0x0F */
        "Chip Express",                                             /* 0x10 */
        "DATARAM",                                                  /* 0x11 */
        "United Microelectronics Corp.",                            /* 0x12 */
        "TCSI",                                                     /* 0x13 */
        "Smart Modular",                                            /* 0x14 */
        "Hughes Aircraft",                                          /* 0x15 */
        "Lanstar Semiconductor",                                    /* 0x16 */
        "Qlogic",                                                   /* 0x17 */
        "Kingston",                                                 /* 0x18 */
        "Music Semic",                                              /* 0x19 */
        "Ericsson Components",                                      /* 0x1A */
        "SpaSE",                                                    /* 0x1B */
        "Eon Silicon Devices",                                      /* 0x1C */
        "Programmable Micro Corp.",                                 /* 0x1D */
        "DoD",                                                      /* 0x1E */
        "Integ. Memories Tech.",                                    /* 0x1F */
        "Corollary Inc.",                                           /* 0x20 */
        "Dallas Semiconductor",                                     /* 0x21 */
        "Omnivision",                                               /* 0x22 */
        "EIV (Switzerland)",                                        /* 0x23 */
        "Novatel Wireless",                                         /* 0x24 */
        "Zarlink (Mitel)",                                          /* 0x25 */
        "Clearpoint",                                               /* 0x26 */
        "Cabletron",                                                /* 0x27 */
        "STEC (Silicon Tech)",                                      /* 0x28 */
        "Vanguard",                                                 /* 0x29 */
        "Hagiwara Sys-Com",                                         /* 0x2A */
        "Vantis",                                                   /* 0x2B */
        "Celestica",                                                /* 0x2C */
        "Century",                                                  /* 0x2D */
        "Hal Computers",                                            /* 0x2E */
        "Rohm Company Ltd",                                         /* 0x2F */
        "Juniper Networks",                                         /* 0x30 */
        "Labit Signal Processing",                                  /* 0x31 */
        "Mushkin Enhanced Memory",                                  /* 0x32 */
        "Tundra Semiconductor",                                     /* 0x33 */
        "Adaptec Inc.",                                             /* 0x34 */
        "LightSpeed Semi.",                                         /* 0x35 */
        "ZSP Corp.",                                                /* 0x36 */
        "AMIC Technology",                                          /* 0x37 */
        "Adobe Systems",                                            /* 0x38 */
        "Dynachip",                                                 /* 0x39 */
        "PNY Electronics",                                          /* 0x3A */
        "Newport Digital",                                          /* 0x3B */
        "MMC Networks",                                             /* 0x3C */
        "T Square",                                                 /* 0x3D */
        "Seiko Epson",                                              /* 0x3E */
        "Broadcom",                                                 /* 0x3F */
        "Viking Components",                                        /* 0x40 */
        "V3 Semiconductor",                                         /* 0x41 */
        "Flextronics (Orbit Semiconductor)",                        /* 0x42 */
        "Suwa Electronics",                                         /* 0x43 */
        "Transmeta",                                                /* 0x44 */
        "Micron CMS",                                               /* 0x45 */
        "American Computer & Digital Components Inc",               /* 0x46 */
        "Enhance 3000 Inc",                                         /* 0x47 */
        "Tower Semiconductor",                                      /* 0x48 */
        "CPU Design",                                               /* 0x49 */
        "Price Point",                                              /* 0x4A */
        "Maxim Integrated Product",                                 /* 0x4B */
        "Tellabs",                                                  /* 0x4C */
        "Centaur Technology",                                       /* 0x4D */
        "Unigen Corporation",                                       /* 0x4E */
        "Transcend Information",                                    /* 0x4F */
        "Memory Card Technology",                                   /* 0x50 */
        "CKD Corporation Ltd.",                                     /* 0x51 */
        "Capital Instruments, Inc.",                                /* 0x52 */
        "Aica Kogyo, Ltd.",                                         /* 0x53 */
        "Linvex Technology",                                        /* 0x54 */
        "MSC Vertriebs GmbH",                                       /* 0x55 */
        "AKM Company, Ltd.",                                        /* 0x56 */
        "Dynamem, Inc.",                                            /* 0x57 */
        "NERA ASA",                                                 /* 0x58 */
        "GSI Technology",                                           /* 0x59 */
        "Dane-Elec (C Memory)",                                     /* 0x5A */
        "Acorn Computers",                                          /* 0x5B */
        "Lara Technology",                                          /* 0x5C */
        "Oak Technology, Inc.",                                     /* 0x5D */
        "Itec Memory",                                              /* 0x5E */
        "Tanisys Technology",                                       /* 0x5F */
        "Truevision",                                               /* 0x60 */
        "Wintec Industries",                                        /* 0x61 */
        "Super PC Memory",                                          /* 0x62 */
        "MGV Memory",                                               /* 0x63 */
        "Galvantech",                                               /* 0x64 */
        "Gadzoox Networks",                                         /* 0x65 */
        "Multi Dimensional Cons.",                                  /* 0x66 */
        "GateField",                                                /* 0x67 */
        "Integrated Memory System",                                 /* 0x68 */
        "Triscend",                                                 /* 0x69 */
        "XaQti",                                                    /* 0x6A */
        "Goldenram",                                                /* 0x6B */
        "Clear Logic",                                              /* 0x6C */
        "Cimaron Communications",                                   /* 0x6D */
        "Nippon Steel Semi. Corp.",                                 /* 0x6E */
        "Advantage Memory",                                         /* 0x6F */
        "AMCC",                                                     /* 0x70 */
        "LeCroy",                                                   /* 0x71 */
        "Yamaha Corporation",                                       /* 0x72 */
        "Digital Microwave",                                        /* 0x73 */
        "NetLogic Microsystems",                                    /* 0x74 */
        "MIMOS Semiconductor",                                      /* 0x75 */
        "Advanced Fibre",                                           /* 0x76 */
        "BF Goodrich Data",                                         /* 0x77 */
        "Epigram",                                                  /* 0x78 */
        "Acbel Polytech Inc.",                                      /* 0x79 */
        "Apacer Technology",                                        /* 0x7A */
        "Admor Memory",                                             /* 0x7B */
        "FOXCONN",                                                  /* 0x7C */
        "Quadratics Superconductor",                                /* 0x7D */
        "3COM",                                                     /* 0x7E */
        "Continue",                                                 /* 0x7F */
    },

    /* bank 3 */
    {
        "Pad",                                                      /* 0x00 */
        "Camintonn Corporation",                                    /* 0x01 */
        "ISOA Incorporated",                                        /* 0x02 */
        "Agate Semiconductor",                                      /* 0x03 */
        "ADMtek Incorporated",                                      /* 0x04 */
        "HYPERTEC",                                                 /* 0x05 */
        "Adhoc Technologies",                                       /* 0x06 */
        "MOSAID Technologies",                                      /* 0x07 */
        "Ardent Technologies",                                      /* 0x08 */
        "Switchcore",                                               /* 0x09 */
        "Cisco Systems, Inc.",                                      /* 0x0A */
        "Allayer Technologies",                                     /* 0x0B */
        "WorkX AG (Wichman)",                                       /* 0x0C */
        "Oasis Semiconductor",                                      /* 0x0D */
        "Novanet Semiconductor",                                    /* 0x0E */
        "E-M Solutions",                                            /* 0x0F */
        "Power General",                                            /* 0x10 */
        "Advanced Hardware Arch.",                                  /* 0x11 */
        "Inova Semiconductors GmbH",                                /* 0x12 */
        "Telocity",                                                 /* 0x13 */
        "Delkin Devices",                                           /* 0x14 */
        "Symagery Microsystems",                                    /* 0x15 */
        "C-Port Corporation",                                       /* 0x16 */
        "CiberCore Technologies",                                   /* 0x17 */
        "Southland Microsystems",                                   /* 0x18 */
        "Malleable Technologies",                                   /* 0x19 */
        "Kendin Communications",                                    /* 0x1A */
        "Great Technology Microcomputer",                           /* 0x1B */
        "Sanmina Corporation",                                      /* 0x1C */
        "HADCO Corporation",                                        /* 0x1D */
        "Corsair",                                                  /* 0x1E */
        "Actrans System Inc.",                                      /* 0x1F */
        "ALPHA Technologies",                                       /* 0x20 */
        "Silicon Laborotaries, Inc. (Cygnal)",                      /* 0x21 */
        "Artesyn Technologies",                                     /* 0x22 */
        "Align Manufacturing",                                      /* 0x23 */
        "Peregrine Semiconductor",                                  /* 0x24 */
        "Chamelion Systems",                                        /* 0x25 */
        "Aplus Flash Technology",                                   /* 0x26 */
        "MIPS Technologies",                                        /* 0x27 */
        "Chrysalis ITS",                                            /* 0x28 */
        "ADTEC Corporation",                                        /* 0x29 */
        "Kentron Technologies",                                     /* 0x2A */
        "Win Technologies",                                         /* 0x2B */
        "Tachyon Semiconductor (ASIC)",                             /* 0x2C */
        "Extreme Packet Devices",                                   /* 0x2D */
        "RF Micro Devices",                                         /* 0x2E */
        "Siemens AG",                                               /* 0x2F */
        "Sarnoff Corporation",                                      /* 0x30 */
        "Itautec SA",                                               /* 0x31 */
        "Radiata Inc.",                                             /* 0x32 */
        "Benchmark Elect.",                                         /* 0x33 */
        "Legend",                                                   /* 0x34 */
        "SpecTek Incorporated",                                     /* 0x35 */
        "Hi/fn",                                                    /* 0x36 */
        "Enikia Incorporated",                                      /* 0x37 */
        "SwitchOn Networks",                                        /* 0x38 */
        "AANetcom Incorporated",                                    /* 0x39 */
        "Micro Memory Bank",                                        /* 0x3A */
        "ESS Technology",                                           /* 0x3B */
        "Virata Corporation",                                       /* 0x3C */
        "Excess Bandwidth",                                         /* 0x3D */
        "West Bay Semiconductor",                                   /* 0x3E */
        "DSP Group",                                                /* 0x3F */
        "Newport Communications",                                   /* 0x40 */
        "Chip2Chip Incorporated",                                   /* 0x41 */
        "Phobos Corporation",                                       /* 0x42 */
        "Intellitech Corporation",                                  /* 0x43 */
        "Nordic VLSI ASA",                                          /* 0x44 */
        "Ishoni Networks",                                          /* 0x45 */
        "Silicon Spice",                                            /* 0x46 */
        "Alchemy Semiconductor",                                    /* 0x47 */
        "Agilent Technologies",                                     /* 0x48 */
        "Centillium Communications",                                /* 0x49 */
        "W.L. Gore",                                                /* 0x4A */
        "HanBit Electronics",                                       /* 0x4B */
        "GlobeSpan",                                                /* 0x4C */
        "Element 14",                                               /* 0x4D */
        "Pycon",                                                    /* 0x4E */
        "Saifun Semiconductors",                                    /* 0x4F */
        "Sibyte, Incorporated",                                     /* 0x50 */
        "MetaLink Technologies",                                    /* 0x51 */
        "Feiya Technology",                                         /* 0x52 */
        "I & C Technology",                                         /* 0x53 */
        "Shikatronics",                                             /* 0x54 */
        "Elektrobit",                                               /* 0x55 */
        "Megic",                                                    /* 0x56 */
        "Com-Tier",                                                 /* 0x57 */
        "Malaysia Micro Solutions",                                 /* 0x58 */
        "Hyperchip",                                                /* 0x59 */
        "Gemstone Communications",                                  /* 0x5A */
        "Anadigm (Anadyne)",                                        /* 0x5B */
        "3ParData",                                                 /* 0x5C */
        "Mellanox Technologies",                                    /* 0x5D */
        "Tenx Technologies",                                        /* 0x5E */
        "Helix AG",                                                 /* 0x5F */
        "Domosys",                                                  /* 0x60 */
        "Skyup Technology",                                         /* 0x61 */
        "HiNT Corporation",                                         /* 0x62 */
        "Chiaro",                                                   /* 0x63 */
        "MDT Technologies GmbH",                                    /* 0x64 */
        "Exbit Technology A/S",                                     /* 0x65 */
        "Integrated Technology Express",                            /* 0x66 */
        "AVED Memory",                                              /* 0x67 */
        "Legerity",                                                 /* 0x68 */
        "Jasmine Networks",                                         /* 0x69 */
        "Caspian Networks",                                         /* 0x6A */
        "nCUBE",                                                    /* 0x6B */
        "Silicon Access Networks",                                  /* 0x6C */
        "FDK Corporation",                                          /* 0x6D */
        "High Bandwidth Access",                                    /* 0x6E */
        "MultiLink Technology",                                     /* 0x6F */
        "BRECIS",                                                   /* 0x70 */
        "World Wide Packets",                                       /* 0x71 */
        "APW",                                                      /* 0x72 */
        "Chicory Systems",                                          /* 0x73 */
        "Xstream Logic",                                            /* 0x74 */
        "Fast-Chip",                                                /* 0x75 */
        "Zucotto Wireless",                                         /* 0x76 */
        "Realchip",                                                 /* 0x77 */
        "Galaxy Power",                                             /* 0x78 */
        "eSilicon",                                                 /* 0x79 */
        "Morphics Technology",                                      /* 0x7A */
        "Accelerant Networks",                                      /* 0x7B */
        "Silicon Wave",                                             /* 0x7C */
        "SandCraft",                                                /* 0x7D */
        "Elpida",                                                   /* 0x7E */
        "Continue"                                                  /* 0x7F */
    },

    /* bank 4 */
    {
        "Pad",                                                      /* 0x00 */
        "Solectron",                                                /* 0x01 */
        "Optosys Technologies",                                     /* 0x02 */
        "Buffalo (Formerly Melco)",                                 /* 0x03 */
        "TriMedia Technologies",                                    /* 0x04 */
        "Cyan Technologies",                                        /* 0x05 */
        "Global Locate",                                            /* 0x06 */
        "Optillion",                                                /* 0x07 */
        "Terago Communications",                                    /* 0x08 */
        "Ikanos Communications",                                    /* 0x09 */
        "Princeton Technology",                                     /* 0x0A */
        "Nanya Technology",                                         /* 0x0B */
        "Elite Flash Storage",                                      /* 0x0C */
        "Mysticom",                                                 /* 0x0D */
        "LightSand Communications",                                 /* 0x0E */
        "ATI Technologies",                                         /* 0x0F */
        "Agere Systems",                                            /* 0x10 */
        "NeoMagic",                                                 /* 0x11 */
        "AuroraNetics",                                             /* 0x12 */
        "Golden Empire",                                            /* 0x13 */
        "Mushkin",                                                  /* 0x14 */
        "Tioga Technologies",                                       /* 0x15 */
        "Netlist",                                                  /* 0x16 */
        "TeraLogic",                                                /* 0x17 */
        "Cicada Semiconductor",                                     /* 0x18 */
        "Centon Electronics",                                       /* 0x19 */
        "Tyco Electronics",                                         /* 0x1A */
        "Magis Works",                                              /* 0x1B */
        "Zettacom",                                                 /* 0x1C */
        "Cogency Semiconductor",                                    /* 0x1D */
        "Chipcon AS",                                               /* 0x1E */
        "Aspex Technology",                                         /* 0x1F */
        "F5 Networks",                                              /* 0x20 */
        "Programmable Silicon Solutions",                           /* 0x21 */
        "ChipWrights",                                              /* 0x22 */
        "Acorn Networks",                                           /* 0x23 */
        "Quicklogic",                                               /* 0x24 */
        "Kingmax Semiconductor",                                    /* 0x25 */
        "BOPS",                                                     /* 0x26 */
        "Flasys",                                                   /* 0x27 */
        "BitBlitz Communications",                                  /* 0x28 */
        "eMemory Technology",                                       /* 0x29 */
        "Procket Networks",                                         /* 0x2A */
        "Purple Ray",                                               /* 0x2B */
        "Trebia Networks",                                          /* 0x2C */
        "Delta Electronics",                                        /* 0x2D */
        "Onex Communications",                                      /* 0x2E */
        "Ample Communications",                                     /* 0x2F */
        "Memory Experts Intl",                                      /* 0x30 */
        "Astute Networks",                                          /* 0x31 */
        "Azanda Network Devices",                                   /* 0x32 */
        "Dibcom",                                                   /* 0x33 */
        "Tekmos",                                                   /* 0x34 */
        "API NetWorks",                                             /* 0x35 */
        "Bay Microsystems",                                         /* 0x36 */
        "Firecron Ltd",                                             /* 0x37 */
        "Resonext Communications",                                  /* 0x38 */
        "Tachys Technologies",                                      /* 0x39 */
        "Equator Technology",                                       /* 0x3A */
        "Concept Computer",                                         /* 0x3B */
        "SILCOM",                                                   /* 0x3C */
        "3Dlabs",                                                   /* 0x3D */
        "c't Magazine",                                             /* 0x3E */
        "Sanera Systems",                                           /* 0x3F */
        "Silicon Packets",                                          /* 0x40 */
        "Viasystems Group",                                         /* 0x41 */
        "Simtek",                                                   /* 0x42 */
        "Semicon Devices Singapore",                                /* 0x43 */
        "Satron Handelsges",                                        /* 0x44 */
        "Improv Systems",                                           /* 0x45 */
        "INDUSYS GmbH",                                             /* 0x46 */
        "Corrent",                                                  /* 0x47 */
        "Infrant Technologies",                                     /* 0x48 */
        "Ritek Corp",                                               /* 0x49 */
        "empowerTel Networks",                                      /* 0x4A */
        "Hypertec",                                                 /* 0x4B */
        "Cavium Networks",                                          /* 0x4C */
        "PLX Technology",                                           /* 0x4D */
        "Massana Design",                                           /* 0x4E */
        "Intrinsity",                                               /* 0x4F */
        "Valence Semiconductor",                                    /* 0x50 */
        "Terawave Communications",                                  /* 0x51 */
        "IceFyre Semiconductor",                                    /* 0x52 */
        "Primarion",                                                /* 0x53 */
        "Picochip Designs Ltd",                                     /* 0x54 */
        "Silverback Systems",                                       /* 0x55 */
        "Jade Star Technologies",                                   /* 0x56 */
        "Pijnenburg Securealink",                                   /* 0x57 */
        "takeMS International AG",                                  /* 0x58 */
        "Cambridge Silicon Radio",                                  /* 0x59 */
        "Swissbit",                                                 /* 0x5A */
        "Nazomi Communications",                                    /* 0x5B */
        "eWave System",                                             /* 0x5C */
        "Rockwell Collins",                                         /* 0x5D */
        "Picocel Co. Ltd. (Paion)",                                 /* 0x5E */
        "Alphamosaic Ltd",                                          /* 0x5F */
        "Sandburst",                                                /* 0x60 */
        "SiCon Video",                                              /* 0x61 */
        "NanoAmp Solutions",                                        /* 0x62 */
        "Ericsson Technology",                                      /* 0x63 */
        "PrairieComm",                                              /* 0x64 */
        "Mitac International",                                      /* 0x65 */
        "Layer N Networks",                                         /* 0x66 */
        "MtekVision (Atsana)",                                      /* 0x67 */
        "Allegro Networks",                                         /* 0x68 */
        "Marvell Semiconductors",                                   /* 0x69 */
        "Netergy Microelectronic",                                  /* 0x6A */
        "NVIDIA",                                                   /* 0x6B */
        "Internet Machines",                                        /* 0x6C */
        "Peak Electronics",                                         /* 0x6D */
        "Litchfield Communication",                                 /* 0x6E */
        "Accton Technology",                                        /* 0x6F */
        "Teradiant Networks",                                       /* 0x70 */
        "Scaleo Chip",                                              /* 0x71 */
        "Cortina Systems",                                          /* 0x72 */
        "RAM Components",                                           /* 0x73 */
        "Raqia Networks",                                           /* 0x74 */
        "ClearSpeed",                                               /* 0x75 */
        "Matsushita Battery",                                       /* 0x76 */
        "Xelerated",                                                /* 0x77 */
        "SimpleTech",                                               /* 0x78 */
        "Utron Technology",                                         /* 0x79 */
        "Astec International",                                      /* 0x7A */
        "AVM gmbH",                                                 /* 0x7B */
        "Redux Communications",                                     /* 0x7C */
        "Dot Hill Systems",                                         /* 0x7D */
        "TeraChip",                                                 /* 0x7E */
        "Continue"                                                  /* 0x7F */
    },

    /* bank 5 */
    {
        "Pad",                                                      /* 0x00 */
        "T-RAM Incorporated",                                       /* 0x01 */
        "Innovics Wireless",                                        /* 0x02 */
        "Teknovus",                                                 /* 0x03 */
        "KeyEye Communications",                                    /* 0x04 */
        "Runcom Technologies",                                      /* 0x05 */
        "RedSwitch",                                                /* 0x06 */
        "Dotcast",                                                  /* 0x07 */
        "Silicon Mountain Memory",                                  /* 0x08 */
        "Signia Technologies",                                      /* 0x09 */
        "Pixim",                                                    /* 0x0A */
        "Galazar Networks",                                         /* 0x0B */
        "White Electronic Designs",                                 /* 0x0C */
        "Patriot Scientific",                                       /* 0x0D */
        "Neoaxiom Corporation",                                     /* 0x0E */
        "3Y Power Technology",                                      /* 0x0F */
        "Scaleo Chip",                                              /* 0x10 */
        "Potentia Power Systems",                                   /* 0x11 */
        "C-guys Incorporated",                                      /* 0x12 */
        "Digital Communications Technology Incorporateed",          /* 0x13 */
        "Silicon-Based Technology",                                 /* 0x14 */
        "Fulcrum Microsystems",                                     /* 0x15 */
        "Positivo Informatica Ltd",                                 /* 0x16 */
        "XIOtech Corporation",                                      /* 0x17 */
        "PortalPlayer",                                             /* 0x18 */
        "Zhiying Software",                                         /* 0x19 */
        "ParkerVision, Inc.",                                       /* 0x1A */
        "Phonex Broadband",                                         /* 0x1B */
        "Skyworks Solutions",                                       /* 0x1C */
        "Entropic Communications",                                  /* 0x1D */
        "Pacific Force Technology",                                 /* 0x1E */
        "Zensys A/S",                                               /* 0x1F */
        "Legend Silicon Corp.",                                     /* 0x20 */
        "Sci-worx GmbH",                                            /* 0x21 */
        "SMSC (Standard Microsystems)",                             /* 0x22 */
        "Renesas Electronics",                                      /* 0x23 */
        "Raza Microelectronics",                                    /* 0x24 */
        "Phyworks",                                                 /* 0x25 */
        "MediaTek",                                                 /* 0x26 */
        "Non-cents Productions",                                    /* 0x27 */
        "US Modular",                                               /* 0x28 */
        "Wintegra Ltd.",                                            /* 0x29 */
        "Mathstar",                                                 /* 0x2A */
        "StarCore",                                                 /* 0x2B */
        "Oplus Technologies",                                       /* 0x2C */
        "Mindspeed",                                                /* 0x2D */
        "Just Young Computer",                                      /* 0x2E */
        "Radia Communications",                                     /* 0x2F */
        "OCZ",                                                      /* 0x30 */
        "Emuzed",                                                   /* 0x31 */
        "LOGIC Devices",                                            /* 0x32 */
        "Inphi Corporation",                                        /* 0x33 */
        "Quake Technologies",                                       /* 0x34 */
        "Vixel",                                                    /* 0x35 */
        "SolusTek",                                                 /* 0x36 */
        "Kongsberg Maritime",                                       /* 0x37 */
        "Faraday Technology",                                       /* 0x38 */
        "Altium Ltd.",                                              /* 0x39 */
        "Insyte",                                                   /* 0x3A */
        "ARM Ltd.",                                                 /* 0x3B */
        "DigiVision",                                               /* 0x3C */
        "Vativ Technologies",                                       /* 0x3D */
        "Endicott Interconnect Technologies",                       /* 0x3E */
        "Pericom",                                                  /* 0x3F */
        "Bandspeed",                                                /* 0x40 */
        "LeWiz Communications",                                     /* 0x41 */
        "CPU Technology",                                           /* 0x42 */
        "Ramaxel Technology",                                       /* 0x43 */
        "DSP Group",                                                /* 0x44 */
        "Axis Communications",                                      /* 0x45 */
        "Legacy Electronics",                                       /* 0x46 */
        "Chrontel",                                                 /* 0x47 */
        "Powerchip Semiconductor",                                  /* 0x48 */
        "MobilEye Technologies",                                    /* 0x49 */
        "Excel Semiconductor",                                      /* 0x4A */
        "A-DATA Technology",                                        /* 0x4B */
        "VirtualDigm",                                              /* 0x4C */
        "G Skill Intl",                                             /* 0x4D */
        "Quanta Computer",                                          /* 0x4E */
        "Yield Microelectronics",                                   /* 0x4F */
        "Afa Technologies",                                         /* 0x50 */
        "KINGBOX Technology Co. Ltd.",                              /* 0x51 */
        "Ceva",                                                     /* 0x52 */
        "iStor Networks",                                           /* 0x53 */
        "Advance Modules",                                          /* 0x54 */
        "Microsoft",                                                /* 0x55 */
        "Open-Silicon",                                             /* 0x56 */
        "Goal Semiconductor",                                       /* 0x57 */
        "ARC International",                                        /* 0x58 */
        "Simmtec",                                                  /* 0x59 */
        "Metanoia",                                                 /* 0x5A */
        "Key Stream",                                               /* 0x5B */
        "Lowrance Electronics",                                     /* 0x5C */
        "Adimos",                                                   /* 0x5D */
        "SiGe Semiconductor",                                       /* 0x5E */
        "Fodus Communications",                                     /* 0x5F */
        "Credence Systems Corp.",                                   /* 0x60 */
        "Genesis Microchip Inc.",                                   /* 0x61 */
        "Vihana, Inc.",                                             /* 0x62 */
        "WIS Technologies",                                         /* 0x63 */
        "GateChange Technologies",                                  /* 0x64 */
        "High Density Devices AS",                                  /* 0x65 */
        "Synopsys",                                                 /* 0x66 */
        "Gigaram",                                                  /* 0x67 */
        "Enigma Semiconductor Inc.",                                /* 0x68 */
        "Century Micro Inc.",                                       /* 0x69 */
        "Icera Semiconductor",                                      /* 0x6A */
        "Mediaworks Integrated Systems",                            /* 0x6B */
        "O’Neil Product Development",                               /* 0x6C */
        "Supreme Top Technology Ltd.",                              /* 0x6D */
        "MicroDisplay Corporation",                                 /* 0x6E */
        "Team Group Inc.",                                          /* 0x6F */
        "Sinett Corporation",                                       /* 0x70 */
        "Toshiba Corporation",                                      /* 0x71 */
        "Tensilica",                                                /* 0x72 */
        "SiRF Technology",                                          /* 0x73 */
        "Bacoc Inc.",                                               /* 0x74 */
        "SMaL Camera Technologies",                                 /* 0x75 */
        "Thomson SC",                                               /* 0x76 */
        "Airgo Networks",                                           /* 0x77 */
        "Wisair Ltd.",                                              /* 0x78 */
        "SigmaTel",                                                 /* 0x79 */
        "Arkados",                                                  /* 0x7A */
        "Compete IT gmbH Co. KG",                                   /* 0x7B */
        "Eudar Technology Inc.",                                    /* 0x7C */
        "Focus Enhancements",                                       /* 0x7D */
        "Xyratex",                                                  /* 0x7E */
        "Continue"                                                  /* 0x7F */
    },

    /* bank 6 */
    {
        "Pad",                                                      /* 0x00 */
        "Specular Networks",                                        /* 0x01 */
        "Patriot Memory",                                           /* 0x02 */
        "U-Chip Technology Corp.",                                  /* 0x03 */
        "Silicon Optix",                                            /* 0x04 */
        "Greenfield Networks",                                      /* 0x05 */
        "CompuRAM GmbH",                                            /* 0x06 */
        "Stargen, Inc.",                                            /* 0x07 */
        "NetCell Corporation",                                      /* 0x08 */
        "Excalibrus Technologies Ltd",                              /* 0x09 */
        "SCM Microsystems",                                         /* 0x0A */
        "Xsigo Systems, Inc.",                                      /* 0x0B */
        "CHIPS & Systems Inc",                                      /* 0x0C */
        "Tier 1 Multichip Solutions",                               /* 0x0D */
        "CWRL Labs",                                                /* 0x0E */
        "Teradici",                                                 /* 0x0F */
        "Gigaram, Inc.",                                            /* 0x10 */
        "g2 Microsystems",                                          /* 0x11 */
        "PowerFlash Semiconductor",                                 /* 0x12 */
        "P.A. Semi, Inc.",                                          /* 0x13 */
        "NovaTech Solutions, S.A.",                                 /* 0x14 */
        "c2 Microsystems, Inc.",                                    /* 0x15 */
        "Level5 Networks",                                          /* 0x16 */
        "COS Memory AG",                                            /* 0x17 */
        "Innovasic Semiconductor",                                  /* 0x18 */
        "02IC Co. Ltd",                                             /* 0x19 */
        "Tabula, Inc.",                                             /* 0x1A */
        "Crucial Technology",                                       /* 0x1B */
        "Chelsio Communications",                                   /* 0x1C */
        "Solarflare Communications",                                /* 0x1D */
        "Xambala Inc.",                                             /* 0x1E */
        "EADS Astrium",                                             /* 0x1F */
        "Terra Semiconductor, Inc.",                                /* 0x20 */
        "Imaging Works, Inc.",                                      /* 0x21 */
        "Astute Networks, Inc.",                                    /* 0x22 */
        "Tzero",                                                    /* 0x23 */
        "Emulex",                                                   /* 0x24 */
        "Power-One",                                                /* 0x25 */
        "Pulse~LINK Inc.",                                          /* 0x26 */
        "Hon Hai Precision Industry",                               /* 0x27 */
        "White Rock Networks Inc.",                                 /* 0x28 */
        "Telegent Systems USA, Inc.",                               /* 0x29 */
        "Atrua Technologies, Inc.",                                 /* 0x2A */
        "Acbel Polytech Inc.",                                      /* 0x2B */
        "eRide Inc.",                                               /* 0x2C */
        "ULi Electronics Inc.",                                     /* 0x2D */
        "Magnum Semiconductor Inc.",                                /* 0x2E */
        "neoOne Technology, Inc.",                                  /* 0x2F */
        "Connex Technology, Inc.",                                  /* 0x30 */
        "Stream Processors, Inc.",                                  /* 0x31 */
        "Focus Enhancements",                                       /* 0x32 */
        "Telecis Wireless, Inc.",                                   /* 0x33 */
        "uNav Microelectronics",                                    /* 0x34 */
        "Tarari, Inc.",                                             /* 0x35 */
        "Ambric, Inc.",                                             /* 0x36 */
        "Newport Media, Inc.",                                      /* 0x37 */
        "VMTS",                                                     /* 0x38 */
        "Enuclia Semiconductor, Inc.",                              /* 0x39 */
        "Virtium Technology Inc.",                                  /* 0x3A */
        "Solid State System Co., Ltd.",                             /* 0x3B */
        "Kian Tech LLC",                                            /* 0x3C */
        "Artimi",                                                   /* 0x3D */
        "Power Quotient International",                             /* 0x3E */
        "Avago Technologies",                                       /* 0x3F */
        "ADTechnology",                                             /* 0x40 */
        "Sigma Designs",                                            /* 0x41 */
        "SiCortex, Inc.",                                           /* 0x42 */
        "Ventura Technology Group",                                 /* 0x43 */
        "eASIC",                                                    /* 0x44 */
        "M.H.S. SAS",                                               /* 0x45 */
        "Micro Star International",                                 /* 0x46 */
        "Rapport Inc.",                                             /* 0x47 */
        "Makway International",                                     /* 0x48 */
        "Broad Reach Engineering Co.",                              /* 0x49 */
        "Semiconductor Mfg Intl Corp",                              /* 0x4A */
        "SiConnect",                                                /* 0x4B */
        "FCI USA Inc.",                                             /* 0x4C */
        "Validity Sensors",                                         /* 0x4D */
        "Coney Technology Co. Ltd.",                                /* 0x4E */
        "Spans Logic",                                              /* 0x4F */
        "Neterion Inc.",                                            /* 0x50 */
        "Qimonda",                                                  /* 0x51 */
        "New Japan Radio Co. Ltd.",                                 /* 0x52 */
        "Velogix",                                                  /* 0x53 */
        "Montalvo Systems",                                         /* 0x54 */
        "iVivity Inc.",                                             /* 0x55 */
        "Walton Chaintech",                                         /* 0x56 */
        "AENEON",                                                   /* 0x57 */
        "Lorom Industrial Co. Ltd.",                                /* 0x58 */
        "Radiospire Networks",                                      /* 0x59 */
        "Sensio Technologies, Inc.",                                /* 0x5A */
        "Nethra Imaging",                                           /* 0x5B */
        "Hexon Technology Pte Ltd",                                 /* 0x5C */
        "CompuStocx (CSX)",                                         /* 0x5D */
        "Methode Electronics, Inc.",                                /* 0x5E */
        "Connect One Ltd.",                                         /* 0x5F */
        "Opulan Technologies",                                      /* 0x60 */
        "Septentrio NV",                                            /* 0x61 */
        "Goldenmars Technology Inc.",                               /* 0x62 */
        "Kreton Corporation",                                       /* 0x63 */
        "Cochlear Ltd.",                                            /* 0x64 */
        "Altair Semiconductor",                                     /* 0x65 */
        "NetEffect, Inc.",                                          /* 0x66 */
        "Spansion, Inc.",                                           /* 0x67 */
        "Taiwan Semiconductor Mfg",                                 /* 0x68 */
        "Emphany Systems Inc.",                                     /* 0x69 */
        "ApaceWave Technologies",                                   /* 0x6A */
        "Mobilygen Corporation",                                    /* 0x6B */
        "Tego",                                                     /* 0x6C */
        "Cswitch Corporation",                                      /* 0x6D */
        "Haier (Beijing) IC Design Co.",                            /* 0x6E */
        "MetaRAM",                                                  /* 0x6F */
        "Axel Electronics Co. Ltd.",                                /* 0x70 */
        "Tilera Corporation",                                       /* 0x71 */
        "Aquantia",                                                 /* 0x72 */
        "Vivace Semiconductor",                                     /* 0x73 */
        "Redpine Signals",                                          /* 0x74 */
        "Octalica",                                                 /* 0x75 */
        "InterDigital Communications",                              /* 0x76 */
        "Avant Technology",                                         /* 0x77 */
        "Asrock, Inc.",                                             /* 0x78 */
        "Availink",                                                 /* 0x79 */
        "Quartics, Inc.",                                           /* 0x7A */
        "Element CXI",                                              /* 0x7B */
        "Innovaciones Microelectronicas",                           /* 0x7C */
        "VeriSilicon Microelectronics",                             /* 0x7D */
        "W5 Networks",                                              /* 0x7E */
        "Continue"                                                  /* 0x7F */
    },

    /* bank 7 */
    {
        "Pad",                                                      /* 0x00 */
        "MOVEKING",                                                 /* 0x01 */
        "Mavrix Technology, Inc.",                                  /* 0x02 */
        "CellGuide Ltd.",                                           /* 0x03 */
        "Faraday Technology",                                       /* 0x04 */
        "Diablo Technologies, Inc.",                                /* 0x05 */
        "Jennic",                                                   /* 0x06 */
        "Octasic",                                                  /* 0x07 */
        "Molex Incorporated",                                       /* 0x08 */
        "3Leaf Networks",                                           /* 0x09 */
        "Bright Micron Technology",                                 /* 0x0A */
        "Netxen",                                                   /* 0x0B */
        "NextWave Broadband Inc.",                                  /* 0x0C */
        "DisplayLink",                                              /* 0x0D */
        "ZMOS Technology",                                          /* 0x0E */
        "Tec-Hill",                                                 /* 0x0F */
        "Multigig, Inc.",                                           /* 0x10 */
        "Amimon",                                                   /* 0x11 */
        "Euphonic Technologies, Inc.",                              /* 0x12 */
        "BRN Phoenix",                                              /* 0x13 */
        "InSilica",                                                 /* 0x14 */
        "Ember Corporation",                                        /* 0x15 */
        "Avexir Technologies Corporation",                          /* 0x16 */
        "Echelon Corporation",                                      /* 0x17 */
        "Edgewater Computer Systems",                               /* 0x18 */
        "XMOS Semiconductor Ltd.",                                  /* 0x19 */
        "GENUSION, Inc.",                                           /* 0x1A */
        "Memory Corp NV",                                           /* 0x1B */
        "SiliconBlue Technologies",                                 /* 0x1C */
        "Rambus Inc.",                                              /* 0x1D */
        "Andes Technology Corporation",                             /* 0x1E */
        "Coronis Systems",                                          /* 0x1F */
        "Achronix Semiconductor",                                   /* 0x20 */
        "Siano Mobile Silicon Ltd.",                                /* 0x21 */
        "Semtech Corporation",                                      /* 0x22 */
        "Pixelworks Inc.",                                          /* 0x23 */
        "Gaisler Research AB",                                      /* 0x24 */
        "Teranetics",                                               /* 0x25 */
        "Toppan Printing Co. Ltd.",                                 /* 0x26 */
        "Kingxcon",                                                 /* 0x27 */
        "Silicon Integrated Systems",                               /* 0x28 */
        "I-O Data Device, Inc.",                                    /* 0x29 */
        "NDS Americas Inc.",                                        /* 0x2A */
        "Solomon Systech Limited",                                  /* 0x2B */
        "On Demand Microelectronics",                               /* 0x2C */
        "Amicus Wireless Inc.",                                     /* 0x2D */
        "SMARDTV SNC",                                              /* 0x2E */
        "Comsys Communication Ltd.",                                /* 0x2F */
        "Movidia Ltd.",                                             /* 0x30 */
        "Javad GNSS, Inc.",                                         /* 0x31 */
        "Montage Technology Group",                                 /* 0x32 */
        "Trident Microsystems",                                     /* 0x33 */
        "Super Talent",                                             /* 0x34 */
        "Optichron, Inc.",                                          /* 0x35 */
        "Future Waves UK Ltd.",                                     /* 0x36 */
        "SiBEAM, Inc.",                                             /* 0x37 */
        "Inicore,Inc.",                                             /* 0x38 */
        "Virident Systems",                                         /* 0x39 */
        "M2000, Inc.",                                              /* 0x3A */
        "ZeroG Wireless, Inc.",                                     /* 0x3B */
        "Gingle Technology Co. Ltd.",                               /* 0x3C */
        "Space Micro Inc.",                                         /* 0x3D */
        "Wilocity",                                                 /* 0x3E */
        "Novafora, Ic.",                                            /* 0x3F */
        "iKoa Corporation",                                         /* 0x40 */
        "ASint Technology",                                         /* 0x41 */
        "Ramtron",                                                  /* 0x42 */
        "Plato Networks Inc.",                                      /* 0x43 */
        "IPtronics AS",                                             /* 0x44 */
        "Infinite-Memories",                                        /* 0x45 */
        "Parade Technologies Inc.",                                 /* 0x46 */
        "Dune Networks",                                            /* 0x47 */
        "GigaDevice Semiconductor",                                 /* 0x48 */
        "Modu Ltd.",                                                /* 0x49 */
        "CEITEC",                                                   /* 0x4A */
        "Northrop Grumman",                                         /* 0x4B */
        "XRONET Corporation",                                       /* 0x4C */
        "Sicon Semiconductor AB",                                   /* 0x4D */
        "Atla Electronics Co. Ltd.",                                /* 0x4E */
        "TOPRAM Technology",                                        /* 0x4F */
        "Silego Technology Inc.",                                   /* 0x50 */
        "Kinglife",                                                 /* 0x51 */
        "Ability Industries Ltd.",                                  /* 0x52 */
        "Silicon Power Computer & Communications",                  /* 0x53 */
        "Augusta Technology, Inc.",                                 /* 0x54 */
        "Nantronics Semiconductors",                                /* 0x55 */
        "Hilscher Gesellschaft",                                    /* 0x56 */
        "Quixant Ltd.",                                             /* 0x57 */
        "Percello Ltd.",                                            /* 0x58 */
        "NextIO Inc.",                                              /* 0x59 */
        "Scanimetrics Inc.",                                        /* 0x5A */
        "FS-Semi Company Ltd.",                                     /* 0x5B */
        "Infinera Corporation",                                     /* 0x5C */
        "SandForce Inc.",                                           /* 0x5D */
        "Lexar Media",                                              /* 0x5E */
        "Teradyne Inc.",                                            /* 0x5F */
        "Memory Exchange Corp.",                                    /* 0x60 */
        "Suzhou Smartek Electronics",                               /* 0x61 */
        "Avantium Corporation",                                     /* 0x62 */
        "ATP Electronics Inc.",                                     /* 0x63 */
        "Valens Semiconductor Ltd",                                 /* 0x64 */
        "Agate Logic, Inc.",                                        /* 0x65 */
        "Netronome",                                                /* 0x66 */
        "Zenverge, Inc.",                                           /* 0x67 */
        "N-trig Ltd",                                               /* 0x68 */
        "SanMax Technologies Inc.",                                 /* 0x69 */
        "Contour Semiconductor Inc.",                               /* 0x6A */
        "TwinMOS",                                                  /* 0x6B */
        "Silicon Systems, Inc.",                                    /* 0x6C */
        "V-Color Technology Inc.",                                  /* 0x6D */
        "Certicom Corporation",                                     /* 0x6E */
        "JSC ICC Milandr",                                          /* 0x6F */
        "PhotoFast Global Inc.",                                    /* 0x70 */
        "InnoDisk Corporation",                                     /* 0x71 */
        "Muscle Power",                                             /* 0x72 */
        "Energy Micro",                                             /* 0x73 */
        "Innofidei",                                                /* 0x74 */
        "CopperGate Communications",                                /* 0x75 */
        "Holtek Semiconductor Inc.",                                /* 0x76 */
        "Myson Century, Inc.",                                      /* 0x77 */
        "FIDELIX",                                                  /* 0x78 */
        "Red Digital Cinema",                                       /* 0x79 */
        "Densbits Technology",                                      /* 0x7A */
        "Zempro",                                                   /* 0x7B */
        "MoSys",                                                    /* 0x7C */
        "Provigent",                                                /* 0x7D */
        "Triad Semiconductor, Inc.",                                /* 0x7E */
        "Continue"                                                  /* 0x7F */
    },

    /* bank 8 */
    {
        "Pad",                                                      /* 0x00 */
        "Siklu Communication Ltd.",                                 /* 0x01 */
        "A Force Manufacturing Ltd.",                               /* 0x02 */
        "Strontium",                                                /* 0x03 */
        "Abilis Systems",                                           /* 0x04 */
        "Siglead, Inc.",                                            /* 0x05 */
        "Ubicom, Inc.",                                             /* 0x06 */
        "Unifosa Corporation",                                      /* 0x07 */
        "Stretch, Inc.",                                            /* 0x08 */
        "Lantiq Deutschland GmbH",                                  /* 0x09 */
        "Visipro",                                                  /* 0x0A */
        "EKMemory",                                                 /* 0x0B */
        "Microelectronics Institute ZTE",                           /* 0x0C */
        "Cognovo Ltd.",                                             /* 0x0D */
        "Carry Technology Co. Ltd.",                                /* 0x0E */
        "Nokia",                                                    /* 0x0F */
        "King Tiger Technology",                                    /* 0x10 */
        "Sierra Wireless",                                          /* 0x11 */
        "HT Micron",                                                /* 0x12 */
        "Albatron Technology Co. Ltd.",                             /* 0x13 */
        "Leica Geosystems AG",                                      /* 0x14 */
        "BroadLight",                                               /* 0x15 */
        "AEXEA",                                                    /* 0x16 */
        "ClariPhy Communications, Inc.",                            /* 0x17 */
        "Green Plug",                                               /* 0x18 */
        "Design Art Networks",                                      /* 0x19 */
        "Mach Xtreme Technology Ltd.",                              /* 0x1A */
        "ATO Solutions Co. Ltd.",                                   /* 0x1B */
        "Ramsta",                                                   /* 0x1C */
        "Greenliant Systems, Ltd.",                                 /* 0x1D */
        "Teikon",                                                   /* 0x1E */
        "Antec Hadron",                                             /* 0x1F */
        "NavCom Technology, Inc.",                                  /* 0x20 */
        "Shanghai Fudan Microelectronics",                          /* 0x21 */
        "Calxeda, Inc.",                                            /* 0x22 */
        "JSC EDC Electronics",                                      /* 0x23 */
        "Kandit Technology Co. Ltd.",                               /* 0x24 */
        "Ramos Technology",                                         /* 0x25 */
        "Goldenmars Technology",                                    /* 0x26 */
        "XeL Technology Inc.",                                      /* 0x27 */
        "Newzone Corporation",                                      /* 0x28 */
        "ShenZhen MercyPower Tech",                                 /* 0x29 */
        "Nanjing Yihuo Technology",                                 /* 0x2A */
        "Nethra Imaging Inc.",                                      /* 0x2B */
        "SiTel Semiconductor BV",                                   /* 0x2C */
        "SolidGear Corporation",                                    /* 0x2D */
        "Topower Computer Ind Co Ltd.",                             /* 0x2E */
        "Wilocity",                                                 /* 0x2F */
        "Profichip GmbH",                                           /* 0x30 */
        "Gerad Technologies",                                       /* 0x31 */
        "Ritek Corporation",                                        /* 0x32 */
        "Gomos Technology Limited",                                 /* 0x33 */
        "Memoright Corporation",                                    /* 0x34 */
        "D-Broad, Inc.",                                            /* 0x35 */
        "HiSilicon Technologies",                                   /* 0x36 */
        "Syndiant Inc.",                                            /* 0x37 */
        "Enverv Inc.",                                              /* 0x38 */
        "Cognex",                                                   /* 0x39 */
        "Xinnova Technology Inc.",                                  /* 0x3A */
        "Ultron AG",                                                /* 0x3B */
        "Concord Idea Corporation",                                 /* 0x3C */
        "AIM Corporation",                                          /* 0x3D */
        "Lifetime Memory Products",                                 /* 0x3E */
        "Ramsway",                                                  /* 0x3F */
        "Recore Systems B.V.",                                      /* 0x40 */
        "Haotian Jinshibo Science Tech",                            /* 0x41 */
        "Being Advanced Memory",                                    /* 0x42 */
        "Adesto Technologies",                                      /* 0x43 */
        "Giantec Semiconductor, Inc.",                              /* 0x44 */
        "HMD Electronics AG",                                       /* 0x45 */
        "Gloway International (HK)",                                /* 0x46 */
        "Kingcore",                                                 /* 0x47 */
        "Anucell Technology Holding",                               /* 0x48 */
        "Accord Software & Systems Pvt. Ltd.",                      /* 0x49 */
        "Active-Semi Inc.",                                         /* 0x4A */
        "Denso Corporation",                                        /* 0x4B */
        "TLSI Inc.",                                                /* 0x4C */
        "Qidan",                                                    /* 0x4D */
        "Mustang",                                                  /* 0x4E */
        "Orca Systems",                                             /* 0x4F */
        "Passif Semiconductors",                                    /* 0x50 */
        "GigaDevice Semiconductor (Beijing) Inc.",                  /* 0x51 */
        "Memphis Electronic",                                       /* 0x52 */
        "Beckhoff Automation GmbH",                                 /* 0x53 */
        "Harmony Semiconductor Corp",                               /* 0x54 */
        "Air Computers SRL",                                        /* 0x55 */
        "TMT Memory",                                               /* 0x56 */
        "Eorex Corporation",                                        /* 0x57 */
        "Xingtera",                                                 /* 0x58 */
        "Netsol",                                                   /* 0x59 */
        "Bestdon Technology Co. Ltd.",                              /* 0x5A */
        "Baysand Inc.",                                             /* 0x5B */
        "Triple Grow Industrial Ltd.",                              /* 0x5C */
        "Wilk Electronik S.A.",                                     /* 0x5D */
        "AAI",                                                      /* 0x5E */
        "Unknown",                                                  /* 0x5F */
        "Unknown",                                                  /* 0x60 */
        "Unknown",                                                  /* 0x61 */
        "Unknown",                                                  /* 0x62 */
        "Unknown",                                                  /* 0x63 */
        "Unknown",                                                  /* 0x64 */
        "Unknown",                                                  /* 0x65 */
        "Unknown",                                                  /* 0x66 */
        "Unknown",                                                  /* 0x67 */
        "Unknown",                                                  /* 0x68 */
        "Unknown",                                                  /* 0x69 */
        "Unknown",                                                  /* 0x6A */
        "Unknown",                                                  /* 0x6B */
        "Unknown",                                                  /* 0x6C */
        "Unknown",                                                  /* 0x6D */
        "Unknown",                                                  /* 0x6E */
        "Unknown",                                                  /* 0x6F */
        "Unknown",                                                  /* 0x70 */
        "Unknown",                                                  /* 0x71 */
        "Unknown",                                                  /* 0x72 */
        "Unknown",                                                  /* 0x73 */
        "Unknown",                                                  /* 0x74 */
        "Unknown",                                                  /* 0x75 */
        "Unknown",                                                  /* 0x76 */
        "Unknown",                                                  /* 0x77 */
        "Unknown",                                                  /* 0x78 */
        "Unknown",                                                  /* 0x79 */
        "Unknown",                                                  /* 0x7A */
        "Unknown",                                                  /* 0x7B */
        "Unknown",                                                  /* 0x7C */
        "Unknown",                                                  /* 0x7D */
        "Unknown",                                                  /* 0x7E */
        "Continue"                                                  /* 0x7F */
    }
};
