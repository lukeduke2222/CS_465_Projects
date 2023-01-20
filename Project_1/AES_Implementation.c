#include <stdio.h>
#include <stdlib.h>
#include "aes_helper.h"
#include <stdint.h>
#include <string.h>

//---------change these constants to correspond with key bit size----------//
static const unsigned int Nr = 10; //stand = 10 num rounds
static const unsigned int Nb = 4; //stand = 4 num columns in state
static const unsigned int Nk = 4; //stand = 4 num of 32-bit words in the Key
//-------------------------------------------------------------------------//

void cipher(const byte in[4*Nb], byte out[4*Nb], word_t word[Nb*(Nr+1)]);
void invCipher(const byte in[4*Nb], byte out[4*Nb], word_t word[Nb*(Nr+1)]);
void addRoundKey(byte round, state_t state, word_t w[4*(Nr+1)]);
void subBytes(state_t);
void shiftRows(state_t);
void mixColumns(state_t);

void invSubBytes(state_t);
void invShiftRows(state_t);
void invMixColumns(state_t);

void keyExpansion(byte key[4*Nk], word_t w[4*(Nr+1)]);
void printState(state_t);
void printDebug(unsigned int round, char label[], state_t state);
void printRoundKeyDebug(unsigned int round, char label[], word_t w[4*(Nr+1)]);

int main(int argc, char** argv) {

    //plaintext ,0x
    byte in[16] = {0x43,0x28,0x2b,0x32,0x2f,0x7e,0x94,0x46,0x86,0x96,0xe8,0x6e,0x2d,0x8f,0xfd,0x50};

    //will hold encrypted
    byte out[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


    //different sized keys that can be commented out
    byte key128[16] = {0x45,0x7f,0x0c,0xc8,0xc9,0x0b,0x8a,0x3b,0xe4,0x67,0x97,0x2c,0x97,0x03,0x47,0xe2 };

//    byte key192[24] = {0x45,0x7f,0x0c,0xc8,0xc9,0x0b,0x8a,0x3b,
//                       0xe4,0x67,0x97,0x2c,0x97,0x03,0x47,0xe2,
//                       0x2e,0xcb,0x28,0xb5,0x70,0xe1,0x67,0xa7};

//    byte key256[32] = {0x45,0x7f,0x0c,0xc8,0xc9,0x0b,0x8a,0x3b,0xe4,
//                       0x67,0x97,0x2c,0x97,0x03,0x47,0xe2,0x2e,
//                       0xcb,0x28,0xb5,0x70,0xe1,0x67,0xa7,0x8a,
//                       0xd4,0xb0,0xee,0x37,0xae,0x74,0xd6};

    word_t w[4*(Nr+1)];
    keyExpansion(key128, w); //-----make sure to change argument and file constants to match corresponding key-----//

    cipher(in, out, w);
    //out is now filled

    //place text to decrypt here
    byte encryptedText[16] = {0x8d,0x33,0x34,0x69,0xdf,0x9a,
                              0x7f,0xa4,0xde,0xc7,0x2c,0x12,0x95,0x85,0x95,0x88};
//    memcpy(encryptedText,out,16); // an alternative if you dont care about separate messages

    byte decryptedText[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    invCipher(encryptedText, decryptedText, w);
    //decryptedText is now filled

    return 0;
}

void cipher(const byte in[4*Nb], byte out[4*Nb], word_t w[4*(Nr+1)]) {
    state_t state;
    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            state[i][j] = in[(i*4)+j];
        }
    }
    printDebug(0, "input", state);
    printRoundKeyDebug(0, "k_sch", w);
    addRoundKey(0, state, w);

    for(unsigned int i = 1; i < Nr; i++) {
        printDebug(i, "start", state);
        subBytes(state);
        printDebug(i, "s_box", state);
        shiftRows(state);
        printDebug(i, "s_row", state);
//        printState(state);
        mixColumns(state);
        printDebug(i, "m_col", state);
        addRoundKey(i, state, w);
        printRoundKeyDebug(i, "k_sch", w);
    }
    subBytes(state);
    printDebug(Nr, "s_box", state);
    shiftRows(state);
    printDebug(Nr, "s_row", state);
    addRoundKey(Nr, state, w);
    printRoundKeyDebug(Nr, "k_sch", w);

    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            out[(i*4)+j] = state[i][j];
        }
    }
    printDebug(Nr, "output", state);
}

void invCipher(const byte in[4*Nb], byte out[4*Nb], word_t w[4*(Nr+1)]) {
    state_t state;
    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            state[i][j] = in[(i*4)+j];
        }
    }
    printf("\n\n\n");
    printDebug(Nr, "iinput", state);
    addRoundKey(Nr, state, w);
    printRoundKeyDebug(Nr, "ik_sch", w);

    for(unsigned int i = Nr-1; i>0; i--) {
        printDebug(i, "istart", state);
        invShiftRows(state);
        printDebug(i, "is_row", state);
        invSubBytes(state);
        printDebug(i, "is_box", state);
        addRoundKey(i, state, w);
        printRoundKeyDebug(i, "ik_sch", w);
        printDebug(i, "ik_add", state);
        invMixColumns(state);
//        printDebug(i, "im_col", state);
    }

    printDebug(0, "istart", state);
    invShiftRows(state);
    printDebug(0, "is_row", state);
    invSubBytes(state);
    printDebug(0, "is_box", state);
    addRoundKey(0, state, w);
    printRoundKeyDebug(0, "ik_sch", w);
    printDebug(0, "ik_add", state);

    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            out[(i*4)+j] = state[i][j];
        }
    }
    printDebug(0, "ioutput", state);
}

void subBytes(state_t state) {
    for (unsigned int i =0; i < 4; i++) {
        for (unsigned int j =0; j < 4; j++) {
            state[i][j] = getSBoxValue(state[i][j]);
        }
    }
}

void shiftRows(state_t state) { //maybe make colum by column
//    printf("before\n");
//    printState(state);
    state_t state1 = {
            {state[0][0],state[1][0],state[2][0],state[3][0]},
            {state[0][1],state[1][1],state[2][1],state[3][1]},
            {state[0][2],state[1][2],state[2][2],state[3][2]},
            {state[0][3],state[1][3],state[2][3],state[3][3]}
    };
    memcpy(state, state1, 16);

    byte tmp = state[1][0]; //shift row 1
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = tmp;

    tmp = state[2][0]; //shift row 2
    state[2][0] = state[2][2];
    state[2][2] = tmp;
    tmp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = tmp;

    tmp = state[3][0]; //shift row 3
    state[3][0] = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = tmp;

    state_t state2 = {
            {state[0][0],state[1][0],state[2][0],state[3][0]},
            {state[0][1],state[1][1],state[2][1],state[3][1]},
            {state[0][2],state[1][2],state[2][2],state[3][2]},
            {state[0][3],state[1][3],state[2][3],state[3][3]}
    };
    memcpy(state, state2, 16);

//    printf("after\n");
//    printState(state);
}

void mixColumns(state_t state) {
    state_t state1 = {
            {state[0][0],state[1][0],state[2][0],state[3][0]},
            {state[0][1],state[1][1],state[2][1],state[3][1]},
            {state[0][2],state[1][2],state[2][2],state[3][2]},
            {state[0][3],state[1][3],state[2][3],state[3][3]}
    };
    memcpy(state, state1, 16);
    for (unsigned int i = 0; i < 4; i++) {
        byte a = fMult(state[0][i], mixConst[0][0]) ^ fMult(state[1][i], mixConst[0][1]) ^
                 fMult(state[2][i], mixConst[0][2]) ^ fMult(state[3][i], mixConst[0][3]);
        byte b = fMult(state[0][i], mixConst[1][0]) ^ fMult(state[1][i], mixConst[1][1]) ^
                 fMult(state[2][i], mixConst[1][2]) ^ fMult(state[3][i], mixConst[1][3]);
        byte c = fMult(state[0][i], mixConst[2][0]) ^ fMult(state[1][i], mixConst[2][1]) ^
                 fMult(state[2][i], mixConst[2][2]) ^ fMult(state[3][i], mixConst[2][3]);
        byte d = fMult(state[0][i], mixConst[3][0]) ^ fMult(state[1][i], mixConst[3][1]) ^
                 fMult(state[2][i], mixConst[3][2]) ^ fMult(state[3][i], mixConst[3][3]);
        state[0][i] = a;
        state[1][i] = b;
        state[2][i] = c;
        state[3][i] = d;
    }
    state_t state2 = {
            {state[0][0],state[1][0],state[2][0],state[3][0]},
            {state[0][1],state[1][1],state[2][1],state[3][1]},
            {state[0][2],state[1][2],state[2][2],state[3][2]},
            {state[0][3],state[1][3],state[2][3],state[3][3]}
    };
    memcpy(state, state2, 16);
}

void invSubBytes(state_t state) {
    for (unsigned int i =0; i < 4; i++) {
        for (unsigned int j =0; j < 4; j++) {
            state[i][j] = getInvsBoxValue(state[i][j]);
        }
    }
}

void invShiftRows(state_t state) {
    state_t state1 = {
            {state[0][0],state[1][0],state[2][0],state[3][0]},
            {state[0][1],state[1][1],state[2][1],state[3][1]},
            {state[0][2],state[1][2],state[2][2],state[3][2]},
            {state[0][3],state[1][3],state[2][3],state[3][3]}
    };
    memcpy(state, state1, 16);

    byte tmp = state[1][3]; //inv shift row 1
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = tmp;

    tmp = state[2][3]; //inv shift row 2
    state[2][3] = state[2][1];
    state[2][1] = tmp;
    tmp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = tmp;

    tmp = state[3][3]; //inv shift row 3
    state[3][3] = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = tmp;

    state_t state2 = {
            {state[0][0],state[1][0],state[2][0],state[3][0]},
            {state[0][1],state[1][1],state[2][1],state[3][1]},
            {state[0][2],state[1][2],state[2][2],state[3][2]},
            {state[0][3],state[1][3],state[2][3],state[3][3]}
    };
    memcpy(state, state2, 16);
}

void invMixColumns(state_t state) {
    state_t state1 = {
            {state[0][0],state[1][0],state[2][0],state[3][0]},
            {state[0][1],state[1][1],state[2][1],state[3][1]},
            {state[0][2],state[1][2],state[2][2],state[3][2]},
            {state[0][3],state[1][3],state[2][3],state[3][3]}
    };
    memcpy(state, state1, 16);
    for (unsigned int i = 0; i < 4; i++) {
        byte a = fMult(state[0][i], invMixConst[0][0]) ^ fMult(state[1][i], invMixConst[0][1]) ^
                 fMult(state[2][i], invMixConst[0][2]) ^ fMult(state[3][i], invMixConst[0][3]);
        byte b = fMult(state[0][i], invMixConst[1][0]) ^ fMult(state[1][i], invMixConst[1][1]) ^
                 fMult(state[2][i], invMixConst[1][2]) ^ fMult(state[3][i], invMixConst[1][3]);
        byte c = fMult(state[0][i], invMixConst[2][0]) ^ fMult(state[1][i], invMixConst[2][1]) ^
                 fMult(state[2][i], invMixConst[2][2]) ^ fMult(state[3][i], invMixConst[2][3]);
        byte d = fMult(state[0][i], invMixConst[3][0]) ^ fMult(state[1][i], invMixConst[3][1]) ^
                 fMult(state[2][i], invMixConst[3][2]) ^ fMult(state[3][i], invMixConst[3][3]);
        state[0][i] = a;
        state[1][i] = b;
        state[2][i] = c;
        state[3][i] = d;
    }
    state_t state2 = {
            {state[0][0],state[1][0],state[2][0],state[3][0]},
            {state[0][1],state[1][1],state[2][1],state[3][1]},
            {state[0][2],state[1][2],state[2][2],state[3][2]},
            {state[0][3],state[1][3],state[2][3],state[3][3]}
    };
    memcpy(state, state2, 16);
}

void keyExpansion(byte key[4*Nk], word_t w[4*(Nr+1)]) {
    word_t temp;
    unsigned int i =0;
    while (i < Nk) {
        word_t word = {key[4*i], key[(4*i)+1],key[(4*i)+2],key[(4*i)+3]};
        memcpy(w[i], word, 4);
        i++;
    }

    i = Nk;
    while (i < 4*(Nr+1)) {
        memcpy(temp, w[i-1], 4);
        if (i%Nk == 0) {
            rotword(temp);
            subword(temp);
            temp[0] ^= Rcon[i/Nk][0];
            temp[1] ^= Rcon[i/Nk][1];
            temp[2] ^= Rcon[i/Nk][2];
            temp[3] ^= Rcon[i/Nk][3];
        } else if (Nk > 6 && i%Nk == 4) {
            subword(temp);
        }
        word_t word = {w[i-Nk][0]^temp[0], w[i-Nk][1]^temp[1], w[i-Nk][2]^temp[2], w[i-Nk][3]^temp[3]};
        memcpy(w[i], word, 4);
        i++;
    }
}

void addRoundKey(byte round, state_t state, word_t w[4*(Nr+1)]) {
//    printState(state);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
//            printf("XORing i:%d j:%d 0x%02x and 0x%02x\t",i,j,state[i][j], w[(round*4)+i][j]);
            state[i][j] ^= w[(round*4)+i][j];
        }
//        printf("\n");
    }
}

void printState(state_t state) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("0x%02x ", state[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printDebug(unsigned int round, char label[], state_t state) {
    printf("round[%2d].%s    ", round, label);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%02x", state[i][j]);
        }
//        printf("\n");
    }
    printf("\n");
}

void printRoundKeyDebug(unsigned int round, char label[], word_t w[4*(Nr+1)]) {
    printf("round[%2d].%s    ", round, label);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%02x", w[(4*round)+i][j]);
        }
//        printf("\n");
    }
    printf("\n");
}