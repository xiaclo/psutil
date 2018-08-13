/*
 * Copyright (c) 2009, Giampaolo Rodola'. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __SMC_H__
#define __SMC_H__

#define KERNEL_INDEX_SMC      2

#define SMC_CMD_READ_BYTES    5
#define SMC_CMD_READ_KEYINFO  9

#define DATATYPE_SP78         "sp78"

// Fans SMC key values
#define SMC_KEY_FAN_SPEED     "F%dAc"
#define SMC_KEY_FAN_NUM       "FNum"

typedef struct {
    char group[10];
    char key[6];
    char label[25];
} smc_key;

const static smc_key smc_keys[] = {
    // --- CPU
    {"CPU", "TCXC", "PECI CPU"},
    {"CPU", "TCXc", "PECI CPU"},
    {"CPU", "TC0P", "CPU 1 Proximity"},
    {"CPU", "TC0H", "CPU 1 Heatsink"},
    {"CPU", "TC0D", "CPU 1 Package"},
    {"CPU", "TC0E", "CPU 1"},
    {"CPU", "TC1C", "CPU Core 1"},
    {"CPU", "TC2C", "CPU Core 2"},
    {"CPU", "TC3C", "CPU Core 3"},
    {"CPU", "TC4C", "CPU Core 4"},
    {"CPU", "TC5C", "CPU Core 5"},
    {"CPU", "TC6C", "CPU Core 6"},
    {"CPU", "TC7C", "CPU Core 7"},
    {"CPU", "TC8C", "CPU Core 8"},
    {"CPU", "TCAH", "CPU 1 Heatsink Alt."},
    {"CPU", "TCAD", "CPU 1 Package Alt."},
    {"CPU", "TC1P", "CPU 2 Proximity"},
    {"CPU", "TC1H", "CPU 2 Heatsink"},
    {"CPU", "TC1D", "CPU 2 Package"},
    {"CPU", "TC1E", "CPU 2"},
    {"CPU", "TCBH", "CPU 2 Heatsink Alt."},
    {"CPU", "TCBD", "CPU 2 Package Alt."},
    {"CPU", "TCSC", "PECI SA"},
    {"CPU", "TCSc", "PECI SA"},
    {"CPU", "TCSA", "PECI SA"},

    // --- GPU
    {"GPU", "TCGC", "PECI GPU"},
    {"GPU", "TCGc", "PECI GPU"},
    {"GPU", "TG0P", "GPU Proximity"},
    {"GPU", "TG0D", "GPU Die"},
    {"GPU", "TG1D", "GPU Die"},
    {"GPU", "TG0H", "GPU Heatsink"},
    {"GPU", "TG1H", "GPU Heatsink"},

    // --- Memory
    {"Memory", "Ts0S", "Memory Proximity"},
    {"Memory", "TM0P", "Mem Bank A1"},
    {"Memory", "TM1P", "Mem Bank A2"},
    {"Memory", "TM8P", "Mem Bank B1"},
    {"Memory", "TM9P", "Mem Bank B2"},
    {"Memory", "TM0S", "Mem Module A1"},
    {"Memory", "TM1S", "Mem Module A2"},
    {"Memory", "TM8S", "Mem Module B1"},
    {"Memory", "TM9S", "Mem Module B2"},

    // --- HDD
    {"HDD", "TH0P", "HDD Bay 1"},
    {"HDD", "TH1P", "HDD Bay 2"},
    {"HDD", "TH2P", "HDD Bay 3"},
    {"HDD", "TH3P", "HDD Bay 4"},

    // --- Battery
    {"Battery", "TB0T", "Battery TS_MAX"},
    {"Battery", "TB1T", "Battery 1"},
    {"Battery", "TB2T", "Battery 2"},
    {"Battery", "TB3T", "Battery"},

    // --- Others
    {"Others", "TN0D", "Northbridge Die"},
    {"Others", "TN0P", "Northbridge Proximity 1"},
    {"Others", "TN1P", "Northbridge Proximity 2"},
    {"Others", "TN0C", "MCH Die"},
    {"Others", "TN0H", "MCH Heatsink"},
    {"Others", "TP0D", "PCH Die"},
    {"Others", "TPCD", "PCH Die"},
    {"Others", "TP0P", "PCH Proximity"},

    {"Others", "TA0P", "Airflow 1"},
    {"Others", "TA1P", "Airflow 2"},
    {"Others", "Th0H", "Heatpipe 1"},
    {"Others", "Th1H", "Heatpipe 2"},
    {"Others", "Th2H", "Heatpipe 3"},

    {"Others", "Tm0P", "Mainboard Proximity"},
    {"Others", "Tp0P", "Powerboard Proximity"},
    {"Others", "Ts0P", "Palm Rest"},
    {"Others", "Tb0P", "BLC Proximity"},

    {"Others", "TL0P", "LCD Proximity"},
    {"Others", "TW0P", "Airport Proximity"},
    {"Others", "TO0P", "Optical Drive"},

    {"Others", "Tp0P", "Power Supply 1"},
    {"Others", "Tp0C", "Power Supply 1 Alt."},
    {"Others", "Tp1P", "Power Supply 2"},
    {"Others", "Tp1C", "Power Supply 2 Alt."},
    {"Others", "Tp2P", "Power Supply 3"},
    {"Others", "Tp3P", "Power Supply 4"},
    {"Others", "Tp4P", "Power Supply 5"},
    {"Others", "Tp5P", "Power Supply 6"},

    {"Others", "TS0C", "Expansion Slots"},
    {"Others", "TA0S", "PCI Slot 1 Pos 1"},
    {"Others", "TA1S", "PCI Slot 1 Pos 2"},
    {"Others", "TA2S", "PCI Slot 2 Pos 1"},
    {"Others", "TA3S", "PCI Slot 2 Pos 2"},

    {"", ""} // Marker for end of array
};

typedef struct {
    char                  major;
    char                  minor;
    char                  build;
    char                  reserved[1];
    UInt16                release;
} SMCKeyData_vers_t;

typedef struct {
    UInt16                version;
    UInt16                length;
    UInt32                cpuPLimit;
    UInt32                gpuPLimit;
    UInt32                memPLimit;
} SMCKeyData_pLimitData_t;

typedef struct {
    UInt32                dataSize;
    UInt32                dataType;
    char                  dataAttributes;
} SMCKeyData_keyInfo_t;

typedef char              SMCBytes_t[32];

typedef struct {
    UInt32                  key;
    SMCKeyData_vers_t       vers;
    SMCKeyData_pLimitData_t pLimitData;
    SMCKeyData_keyInfo_t    keyInfo;
    char                    result;
    char                    status;
    char                    data8;
    UInt32                  data32;
    SMCBytes_t              bytes;
} SMCKeyData_t;

typedef char              UInt32Char_t[5];

typedef struct {
    UInt32Char_t            key;
    UInt32                  dataSize;
    UInt32Char_t            dataType;
    SMCBytes_t              bytes;
} SMCVal_t;

double SMCGetTemperature(char *key);
int SMCGetFanNumber(char *key);
float SMCGetFanSpeed(int fanNum);

#endif
