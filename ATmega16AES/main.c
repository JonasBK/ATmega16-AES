#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char S[] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

unsigned char* subBytes(unsigned char currentState[16]) {
	static unsigned char newState[16];
	newState[0] = S[currentState[0]];
	newState[1] = S[currentState[1]];
	newState[2] = S[currentState[2]];
	newState[3] = S[currentState[3]];
	newState[4] = S[currentState[4]];
	newState[5] = S[currentState[5]];
	newState[6] = S[currentState[6]];
	newState[7] = S[currentState[7]];
	newState[8] = S[currentState[8]];
	newState[9] = S[currentState[9]];
	newState[10] = S[currentState[10]];
	newState[11] = S[currentState[11]];
	newState[12] = S[currentState[12]];
	newState[13] = S[currentState[13]];
	newState[14] = S[currentState[14]];
	newState[15] = S[currentState[15]];
	return newState;
}

unsigned char* shiftRows(unsigned char currentState[16]) {
    static unsigned char newState[16];
    newState[0] = currentState[0];
	newState[1] = currentState[1];
	newState[2] = currentState[2];
	newState[3] = currentState[3];
	newState[4] = currentState[5];
	newState[5] = currentState[6];
	newState[6] = currentState[7];
	newState[7] = currentState[4];
	newState[8] = currentState[10];
	newState[9] = currentState[11];
	newState[10] = currentState[8];
	newState[11] = currentState[9];
	newState[12] = currentState[15];
	newState[13] = currentState[12];
	newState[14] = currentState[13];
	newState[15] = currentState[14];
    return newState;
}

//Multiply two numbers in the GF(2^8) finite field defined by the polynomial x^8 + x^4 + x^3 + x + 1 = 0
char gMul(char a, char b) {
	char p = 0;
	for (int i = 0; i < 8; i++) {
		if (b & 1) {
			p ^= a;
		}
		if (a & 0x80) {
			a <<= 1;
			a ^= 0x1b;
			} else {
			a <<= 1;
		}
		b >>= 1;
	}
	return p;
}

unsigned char* mixColumns(unsigned char state[16]) {
	static unsigned char result[16];
	result[0] = gMul(0x02, state[0]) ^ gMul(0x03, state[4]) ^ state[8] ^ state[12];
	result[1] = gMul(0x02, state[1]) ^ gMul(0x03, state[5]) ^ state[9] ^ state[13];
	result[2] = gMul(0x02, state[2]) ^ gMul(0x03, state[6]) ^ state[10] ^ state[14];
	result[3] = gMul(0x02, state[3]) ^ gMul(0x03, state[7]) ^ state[11] ^ state[15];
	result[4] = state[0] ^ gMul(0x02, state[4]) ^ gMul(0x03, state[8]) ^ state[12];
	result[5] = state[1] ^ gMul(0x02, state[5]) ^ gMul(0x03, state[9]) ^ state[13];
	result[6] = state[2] ^ gMul(0x02, state[6]) ^ gMul(0x03, state[10]) ^ state[14];
	result[7] = state[3] ^ gMul(0x02, state[7]) ^ gMul(0x03, state[11]) ^ state[15];
	result[8] = state[0] ^ state[4] ^ gMul(0x02, state[8]) ^ gMul(0x03, state[12]);
	result[9] = state[1] ^ state[5] ^ gMul(0x02, state[9]) ^ gMul(0x03, state[13]);
	result[10] = state[2] ^ state[6] ^ gMul(0x02, state[10]) ^ gMul(0x03, state[14]);
	result[11] = state[3] ^ state[7] ^ gMul(0x02, state[11]) ^ gMul(0x03, state[15]);
	result[12] = gMul(0x03, state[0]) ^ state[4] ^ state[8] ^ gMul(0x02, state[12]);
	result[13] = gMul(0x03, state[1]) ^ state[5] ^ state[9] ^ gMul(0x02, state[13]);
	result[14] = gMul(0x03, state[2]) ^ state[6] ^ state[10] ^ gMul(0x02, state[14]);	
	result[15] = gMul(0x03, state[3]) ^ state[7] ^ state[11] ^ gMul(0x02, state[15]);
	return result;
}

unsigned char* addRoundKey(unsigned char currentState[16], unsigned char key[16]) {
	static unsigned char newState[16];
	newState[0] = currentState[0] ^ key[0];
	newState[1] = currentState[1] ^ key[1];
	newState[2] = currentState[2] ^ key[2];
	newState[3] = currentState[3] ^ key[3];
	newState[4] = currentState[4] ^ key[4];
	newState[5] = currentState[5] ^ key[5];
	newState[6] = currentState[6] ^ key[6];
	newState[7] = currentState[7] ^ key[7];
	newState[8] = currentState[8] ^ key[8];
	newState[9] = currentState[9] ^ key[9];
	newState[10] = currentState[10] ^ key[10];
	newState[11] = currentState[11] ^ key[11];
	newState[12] = currentState[12] ^ key[12];
	newState[13] = currentState[13] ^ key[13];
	newState[14] = currentState[14] ^ key[14];
	newState[15] = currentState[15] ^ key[15];	
	return newState;
}

unsigned char roundConst[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

unsigned char* keySchedule(unsigned char currentKey[16], int round) {
	static unsigned char newKey[16];
	newKey[0] = currentKey[0] ^ S[currentKey[7]] ^ roundConst[round];
	newKey[4] = currentKey[4] ^ S[currentKey[11]];
	newKey[8] = currentKey[8] ^ S[currentKey[15]];
	newKey[12] = currentKey[12] ^ S[currentKey[3]];
	newKey[1] = currentKey[1] ^ newKey[0];
	newKey[5] = currentKey[5] ^ newKey[4];
	newKey[9] = currentKey[9] ^ newKey[8];
	newKey[13] = currentKey[13] ^ newKey[12];
	newKey[2] = currentKey[2] ^ newKey[1];
	newKey[6] = currentKey[6] ^ newKey[5];
	newKey[10] = currentKey[10] ^ newKey[9];
	newKey[14] = currentKey[14] ^ newKey[13];
	newKey[3] = currentKey[3] ^ newKey[2];
	newKey[7] = currentKey[7] ^ newKey[6];
	newKey[11] = currentKey[11] ^ newKey[10];
	newKey[15] = currentKey[15] ^ newKey[14];
	return newKey;
}

int main(void) {
	unsigned char state[16] = {
			0xf3, 0x3c, 0xcd, 0x08, 
			0x44, 0xc6, 0x5d, 0xf2, 
			0x81, 0x27, 0xc3, 0x73, 
			0xec, 0xba, 0xfb, 0xe6
		}, 
		key[16] = {
			0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00
		}, 
		*newState, *newKey, res[16];

	newState = addRoundKey(state, key);
	newKey = keySchedule(key, 0);

	newState = subBytes(newState);
	newState = shiftRows(newState);
	newState = mixColumns(newState);
	newState = addRoundKey(newState, newKey);
	newKey = keySchedule(newKey, 1);
	
	newState = subBytes(newState);
	newState = shiftRows(newState);
	newState = mixColumns(newState);
	newState = addRoundKey(newState, newKey);
	newKey = keySchedule(newKey, 2);

	newState = subBytes(newState);
	newState = shiftRows(newState);
	newState = mixColumns(newState);
	newState = addRoundKey(newState, newKey);
	newKey = keySchedule(newKey, 3);
	
	newState = subBytes(newState);
	newState = shiftRows(newState);
	newState = mixColumns(newState);
	newState = addRoundKey(newState, newKey);
	newKey = keySchedule(newKey, 4);

	newState = subBytes(newState);
	newState = shiftRows(newState);
	newState = mixColumns(newState);
	newState = addRoundKey(newState, newKey);
	newKey = keySchedule(newKey, 5);
	
	newState = subBytes(newState);
	newState = shiftRows(newState);
	newState = mixColumns(newState);
	newState = addRoundKey(newState, newKey);
	newKey = keySchedule(newKey, 6);

	newState = subBytes(newState);
	newState = shiftRows(newState);
	newState = mixColumns(newState);
	newState = addRoundKey(newState, newKey);
	newKey = keySchedule(newKey, 7);
	
	newState = subBytes(newState);
	newState = shiftRows(newState);
	newState = mixColumns(newState);
	newState = addRoundKey(newState, newKey);
	newKey = keySchedule(newKey, 8);

	newState = subBytes(newState);
	newState = shiftRows(newState);
	newState = mixColumns(newState);
	newState = addRoundKey(newState, newKey);
	newKey = keySchedule(newKey, 9);

	newState = subBytes(newState);
	newState = shiftRows(newState);
	newState = addRoundKey(newState, newKey);

	// Only to make sure the encryption is correct and not considered part of optimized code
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res[4 * i + j] = newState[4 * j + i];
		}
	}
		
	return 0;
}

//Key			00000000000000000000000000000000
//Plaintext		f34481ec3cc627bacd5dc3fb08f273e6
//Ciphertext	0336763e966d92595a567cc9ce537f5e
