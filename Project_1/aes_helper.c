//
// Created by lalbi on 1/11/2023.
//

#include <stdio.h>
#include "aes_helper.h"

byte fAdd(byte a, byte b) {
    return a ^ b;
}

byte xtime(byte x) {
    return (fAdd((x<<1), (((x>>7) & 1) * 0x1b)));
//    return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

byte fMult(byte a, byte b) {
    byte tot = 0;
    for(unsigned int i = 0; i < 8; i++) { //each bit of b right to left
        if (b & 1 << i) {
            byte time = a;
            for (unsigned int j = 0; j < i; j++) {
                time = xtime(time);
            }
            if (tot == 0) tot = time;
            else tot = fAdd(tot, time);
        }
    }
    return tot;
}

int getSBoxValue(int num) {
    int right = num & (0x0f);
    int left = (num & (0xf0)) >> 4;

    return s_box[left][right];
}

int getInvsBoxValue(int num) {
    int right = num & (0x0f);
    int left = (num & (0xf0)) >> 4;

    return invs_box[left][right];
}

void subword(word_t word) {
    word[0] = getSBoxValue(word[0]);
    word[1] = getSBoxValue(word[1]);
    word[2] = getSBoxValue(word[2]);
    word[3] = getSBoxValue(word[3]);
}

void rotword(word_t word) {
    byte temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}