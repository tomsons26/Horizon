/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Functions for getting random values
 *
 * @copyright Horizon is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "rand.h"
#include <stdlib.h>
#include <time.h>

int SimRandIndex = 0;
char RandNumb[4] = { 0x12, 0x34, 0x98, 0x76 };

void randomize()
{
    srand(time(0));
}

union u_int
{
    char c[4];
    int i;
};

u_int RandNumb2 = { 0x12349876 };

unsigned int __declspec(naked) __cdecl Random2()
{
    __asm
    {
    push    esi
    mov     esi, offset RandNumb2
    xor     eax, eax        ; Logical Exclusive OR
    mov     al, [esi]
    shr     al, 1           ; Shift Logical Right
    shr     al, 1           ; Shift Logical Right
    rcl     byte ptr [esi+2], 1 ; Rotate Through Carry Left
    rcl     byte ptr [esi+1], 1 ; Rotate Through Carry Left
    cmc                     ; Complement Carry Flag
    sbb     al, [esi]       ; Integer Subtraction with Borrow
    shr     al, 1           ; Shift Logical Right
    rcr     byte ptr [esi], 1 ; Rotate Through Carry Right
    mov     al, [esi]
    xor     al, [esi+1]     ; Logical Exclusive OR
    pop     esi
    retn
    }
}

unsigned char Random()
{
    unsigned char bVar2 = (unsigned char)RandNumb2.c[0] >> 1;
    unsigned char bVar1 = RandNumb2.c[1] << 1 | RandNumb2.c[2] < 0;
    RandNumb2.c[0] =
        RandNumb2.c[0] >> 1 | ((((unsigned char)RandNumb2.c[0] >> 0x2) - RandNumb2.c[0]) - (-1 < RandNumb2.c[1])) * -128;
    RandNumb2.c[1] = bVar1;
    RandNumb2.c[2] = RandNumb2.c[2] << 1 | bVar2 & 1;
    return RandNumb2.c[0] ^ bVar1;
    // ghidra
    /*
    byte bVar1;
    byte bVar2;

    bVar2 = (byte)RandNumb >> 1;
    bVar1 = RandNumb._1_1_ << 1 | (char)RandNumb._2_1_ < '\0';
    RandNumb._0_1_ = (byte)RandNumb >> 1 | ((((byte)RandNumb >> 2) - (byte)RandNumb) - (-1 < (char)RandNumb._1_1_)) * -128;
    RandNumb._1_1_ = bVar1;
    RandNumb._2_1_ = RandNumb._2_1_ << 1 | bVar2 & 1;
    return (uint)((byte)RandNumb ^ bVar1);
    */

    // ida
    /*
        unsigned __int8 v0; // cf@1
        char v1; // t0@1
        char v2; // t0@1
        char v3; // t0@1
        int result; // eax@1

        result = RandNumb;
        LOBYTE(result) = RandNumb >> 2;
        v1 = __RCL__(BYTE2(RandNumb), (RandNumb >> 1) & 1);
        v0 = __CFRCL__(BYTE2(RandNumb), (RandNumb >> 1) & 1);
        BYTE2(RandNumb) = v1;
        v2 = __RCL__(BYTE1(RandNumb), v0);
        v0 = __CFRCL__(BYTE1(RandNumb), v0);
        BYTE1(RandNumb) = v2;
        v3 = __RCR__(RandNumb, (result - (!v0 + RandNumb)) & 1);
        LOBYTE(RandNumb) = v3;
        LOBYTE(result) = BYTE1(RandNumb) ^ v3;
        return result;
    }
    */

    /*
    saturn
        short sVar1;
        uint uVar2;
        uint uVar3;
        char *pcVar4;

        sVar1 = *(short *)PTR_RandNumb_060752a0;
        pcVar4 = PTR_RandNumb_060752a0;
        if (true) {
            pcVar4 = PTR_RandNumb_060752a0 + 0x2;
        }
        uVar2 = SEXT14(*pcVar4);
        uVar3 = (int)sVar1 << 0x1;
        if ((uVar2 >> 0x1 & 0x1) == 0x1) {
            uVar3 |= 0x1;
        }
        *(uint *)(pcVar4 + -0x2) = uVar3;
        uVar2 = (((((uVar2 >> 0x2) - (uVar2 & 0xff)) - (uint)(((int)sVar1 & 0x80000000U) != 0x0)) + 0x1) * 0x100 | uVar2 &
    0xff) >> 0x1; *pcVar4 = (char)uVar2; return (uVar2 ^ uVar3 >> 0x8) & 0xff;
    */

    /*
    n64
    uint uVar1;

    uVar1 = rand();
    return uVar1 & 0xff;
    */
}

/*
n64
uint Get_Random_Value(void)

{
    int iVar1;

    iVar1 = DAT_800ec910 * 0x41c64e6d;
    DAT_800ec910 = iVar1 + 0x3039;
    return iVar1 + 0x303a >> 0x10 & 0x7fff;
}
*/

int Sim_Random()
{
    // clang-format off
static unsigned char _randvals[] = {
    71,  206, 198, 110, 215, 159, 152, 41,  146, 12,  116, 162, 101, 32,  75,
    79,  30,  237, 58,  223, 165, 125, 181, 200, 134, 1,   129, 202, 241, 23,
    214, 35,  225, 189, 14,  228, 98,  250, 217, 92,  104, 245, 127, 220, 231,
    185, 196, 179, 122, 216, 6,   62,  235, 9,   26,  49,  63,  70,  40,  18,
    240, 16,  132, 118, 59,  197, 83,  24,  20,  115, 126, 89,  72,  147, 170,
    29,  93,  121, 36,  97,  27,  253, 43,  168, 194, 219, 232, 42,  176, 37,
    149, 171, 150, 131, 252, 95,  156, 50,  120, 154, 158, 226, 142, 53,  76,
    65,  161, 105, 90,  254, 167, 164, 246, 109, 193, 88,  10,  207, 234, 195,
    186, 133, 153, 141, 54,  182, 221, 211, 4,   230, 69,  13,  96,  174, 163,
    34,  77,  233, 201, 155, 183, 15,  2,   66,  249, 11,  143, 67,  68,  135,
    112, 190, 227, 248, 238, 169, 188, 192, 103, 51,  22,  55,  87,  173, 94,
    157, 100, 64,  84,  5,   44,  224, 178, 151, 8,   175, 117, 138, 91,  251,
    78,  191, 145, 243, 203, 124, 99,  239, 137, 82,  108, 47,  33,  74,  247,
    205, 46,  244, 199, 111, 25,  177, 102, 204, 144, 140, 80,  81,  38,  123,
    218, 73,  128, 48,  85,  31,  210, 180, 209, 213, 107, 242, 114, 187, 19,
    61,  255, 21,  56,  229, 212, 222, 45,  39,  148, 160, 208, 57,  130, 139,
    3,   172, 60,  52,  119, 184, 236, 0,   7,   28,  136, 166, 86,  17,  113,
    106
};
    // clang-format on
    // this can overflow..
    return _randvals[++SimRandIndex];

    /*
    n64
    return Get_Random_Value() & 0xff;
    */
}

int Sim_IRandom(int minval, int maxval)
{
    int val = Sim_Random() * (maxval - minval) + 128;
    if (val & 0xFF000000) {
        val = 0xFFFFFF;
    }
    return (val / 256) + minval;

    /*
    n64
    uint uVar1;
    int iVar2;

    uVar1 = Sim_Random();
    uVar1 = uVar1 * (iParm2 - iParm1) + 0x80;
    iVar2 = 0xffff;
    if ((uVar1 & 0xff000000) == 0x0) {
        iVar2 = (int)uVar1 >> 0x8;
    }
    return iVar2 + iParm1;
    */
}

// from https://github.com/microsoft/JsonBuilder/blob/master/src/PodVector.cpp
static inline unsigned char Bit_Scan_Reverse(unsigned int *Index, unsigned int Mask)
{
    if (Mask == 0 || Index == 0)
        return 0;

    int ii = 0;
    for (ii = ((sizeof(Mask) * 8) - 1); ii >= 0; --ii) {
        unsigned long tempMask = 1 << ii;
        if ((Mask & tempMask) != 0) {
            *Index = ii;
            break;
        }
    }
    return (ii >= 0 ? (unsigned char)1 : (unsigned char)0);
}

unsigned int __cdecl Get_Random_Mask(int num)
{
    unsigned int index;
    unsigned int mask = 1;
    // bsr instruction - find most significant non-zero bit
    // First_True_Bit that runs backwards needed here?
    // if (_BitScanReverse(&index, num)) { //
    if (Bit_Scan_Reverse(&index, num)) { //
        mask = (1 << (index + 1)) - 1;
    }
    return mask;
}

// console game version
/*
unsigned int Get_Random_Mask(int num)
{
    // clang-format off
    unsigned int RandMask[]{
        0xFFFFFFFF, 0x7FFFFFFF, 0x3FFFFFFF, 0x1FFFFFFF,
        0xFFFFFFF,  0x7FFFFFF,  0x3FFFFFF,  0x1FFFFFF,
        0xFFFFFF,   0x7FFFFF,   0x3FFFFF,   0x1FFFFF,
        0xFFFFF,    0x7FFFF,    0x3FFFF,    0x1FFFF,
        0xFFFF,     0x7FFF,     0x3FFF,     0x1FFF,
        0xFFF,      0x7FF,      0x3FF,      0x1FF,
        0xFF,       0x7F,       0x3F,       0x1F,
        0xF,        7,          3,          1,
    };
    // clang-format on

    if ((num & 0x80000000) != 0x0) {
        return RandMask[00];
    }
    if ((num & 0x40000000) != 0x0) {
        return RandMask[01];
    }
    if ((num & 0x20000000) != 0x0) {
        return RandMask[02];
    }
    if ((num & 0x10000000) != 0x0) {
        return RandMask[03];
    }
    if ((num & 0x8000000) != 0x0) {
        return RandMask[04];
    }
    if ((num & 0x4000000) != 0x0) {
        return RandMask[05];
    }
    if ((num & 0x2000000) != 0x0) {
        return RandMask[06];
    }
    if ((num & 0x1000000) != 0x0) {
        return RandMask[07];
    }
    if ((num & 0x00800000) != 0x0) {
        return RandMask[08];
    }
    if ((num & 0x00400000) != 0x0) {
        return RandMask[09];
    }
    if ((num & 0x00200000) != 0x0) {
        return RandMask[10];
    }
    if ((num & 0x00100000) != 0x0) {
        return RandMask[11];
    }
    if ((num & 0x000080000) != 0x0) {
        return RandMask[12];
    }
    if ((num & 0x000040000) != 0x0) {
        return RandMask[13];
    }
    if ((num & 0x000020000) != 0x0) {
        return RandMask[14];
    }
    if ((num & 0x000010000) != 0x0) {
        return RandMask[15];
    }
    if ((num & 0x000008000) != 0x0) {
        return RandMask[16];
    }
    if ((num & 0x000004000) != 0x0) {
        return RandMask[17];
    }
    if ((num & 0x000002000) != 0x0) {
        return RandMask[18];
    }
    if ((num & 0x000001000) != 0x0) {
        return RandMask[19];
    }
    if ((num & 0x000000800) != 0x0) {
        return RandMask[20];
    }
    if ((num & 0x000000400) != 0x0) {
        return RandMask[21];
    }
    if ((num & 0x000000200) != 0x0) {
        return RandMask[22];
    }
    if ((num & 0x000000100) != 0x0) {
        return RandMask[23];
    }
    if ((num & 0x000000080) != 0x0) {
        return RandMask[24];
    }
    if ((num & 0x000000040) != 0x0) {
        return RandMask[25];
    }
    if ((num & 0x000000020) != 0x0) {
        return RandMask[26];
    }
    if ((num & 0x000000010) != 0x0) {
        return RandMask[27];
    }
    if ((num & 0x000000008) != 0x0) {
        return RandMask[28];
    }
    if ((num & 0x000000004) != 0x0) {
        return RandMask[29];
    }
    if ((num & 0x000000002) != 0x0) {
        return RandMask[30];
    }
    if ((num & 0x000000001) != 0x0) {
        return RandMask[31];
    }
}
*/

// this is called by a templated function in Sole, inlined in TD?
int IRandom(int minval, int maxval)
{
    if (minval > maxval) {
        // pc
        // maxval ^= minval ^ maxval;
        // minval ^= maxval ^ maxval;

        // n64
        minval ^= maxval;
        maxval ^= minval;
        minval ^= maxval;
    }
    int num;
    int mask = Get_Random_Mask(maxval - minval);
    do {
        num = (mask & rand()) + minval;
    } while (num > maxval);
    return num;
}
