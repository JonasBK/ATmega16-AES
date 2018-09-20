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
	for (int i = 0; i < 16; i++) {
		newState[i] = S[currentState[i]];
	}
	return newState;
}

unsigned char sBox(unsigned char ch) {
	return S[ch];
}

unsigned char* shiftRows(unsigned char currentState[16]) {
	static unsigned char newState[16];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int currentIndex = i * 4 + j;
			newState[currentIndex] = currentState[i * 4 + (j + i) % 4];
		}
	}
	return newState;
}

unsigned char M[4][4] = {
	{0x02, 0x03, 0x01, 0x01},
	{0x01, 0x02, 0x03, 0x01},
	{0x01, 0x01, 0x02, 0x03},
	{0x03, 0x01, 0x01, 0x02} };

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

	unsigned char vectorsDotProd(unsigned char *x, const unsigned char *y) {
		unsigned char res = 0x00;
		for (int i = 0; i < 4; i++) {
			res ^= gMul(x[i], y[i]);
		}
		return res;
	}

	unsigned char* matrixVectorMult(const unsigned char *vec) {
		static unsigned char result[4];
		for (int i = 0; i < 4; i++) {
			result[i] = vectorsDotProd(M[i], vec);
		}
		return result;
	}

	unsigned char* mixColumns(unsigned char state[16]) {
		static unsigned char result[16];
		unsigned char *subres;
		for (int i = 0; i < 4; i++) {
			unsigned char vec[4];
			for (int j = 0; j < 4; j++) {
				vec[j] = state[4 * j + i];
			}
			subres = matrixVectorMult(vec);
			for (int j = 0; j < 4; j++) {
				result[4 * j + i] = subres[j];
			}
		}
		return result;
	}

	unsigned char* addRoundKey(unsigned char currentState[16], unsigned char key[16]) {
		static unsigned char newState[16];
		for (int i = 0; i < 16; i++) {
			newState[i] = currentState[i] ^ key[i];
		}
		return newState;
	}

	unsigned char roundConst[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

	unsigned char* getFunnyWord(unsigned char subWord[4], unsigned char roundConst) {
		static unsigned char funnyWord[4];
		for (int i = 0; i < 4; i++) {
			if (i == 0) {
				funnyWord[i] = sBox(subWord[(i + 1) % 4]) ^ roundConst;
				} else {
				funnyWord[i] = sBox(subWord[(i + 1) % 4]);
			}
		}
		return funnyWord;
	}

	unsigned char* keySchedule(unsigned char currentKey[16], int round) {
		static unsigned char newKey[16];
		unsigned char currentSubKeys[4][4];
		unsigned char *funnyWord;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				currentSubKeys[i][j] = currentKey[4 * j + i];
			}
		}
		funnyWord = getFunnyWord(currentSubKeys[3], roundConst[round]);

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == 0) {
					newKey[4 * j + i] = currentSubKeys[i][j] ^ funnyWord[j];
					} else {
					newKey[4 * j + i] = currentSubKeys[i][j] ^ newKey[4 * j + (i - 1)];
				}
			}
		}
		return newKey;
	}

	int main(void) {
		const char plaintext[] = "f34481ec3cc627bacd5dc3fb08f273e6", key[] = "00000000000000000000000000000000";
		unsigned char res[16];
		const char *pos1 = plaintext, *pos2 = key;
		unsigned char state[16], tempKey2[16], tempState[16], tempKey[16], *newState, *newKey;

		// Plaintext to state array and key similar
		for (int i = 0; i < sizeof state/sizeof *state; i++) {
			sscanf(pos1, "%2hhx", &tempState[i]);
			sscanf(pos2, "%2hhx", &tempKey[i]);
			pos1 += 2;
			pos2 += 2;
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tempKey2[4 * i + j] = tempKey[4 * j + i];
				state[4 * i + j] = tempState[4 * j + i];
			}
		}

		newState = addRoundKey(state, tempKey2);
		newKey = keySchedule(tempKey2, 0);

		for (int i = 1; i < 10; i++) {
			newState = subBytes(newState);
			newState = shiftRows(newState);
			newState = mixColumns(newState);
			newState = addRoundKey(newState, newKey);
			newKey = keySchedule(newKey, i);
		}

		newState = subBytes(newState);
		newState = shiftRows(newState);
		newState = addRoundKey(newState, newKey);

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