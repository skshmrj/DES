#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "DES.h"

char *initial_permutation(char *input);
char *PC2(char *sub_key_56_bits);
char *key_transformation(char *key_64_bit);
void generate_sub_keys(char *key, sub_key_set *sub_keys);

int initial_message_permutation[] = {58, 50, 42, 34, 26, 18, 10, 2,
									60, 52, 44, 36, 28, 20, 12, 4,
									62, 54, 46, 38, 30, 22, 14, 6,
									64, 56, 48, 40, 32, 24, 16, 8,
									57, 49, 41, 33, 25, 17,  9, 1,
									59, 51, 43, 35, 27, 19, 11, 3,
									61, 53, 45, 37, 29, 21, 13, 5,
									63, 55, 47, 39, 31, 23, 15, 7};

int final_message_permutation[] =  {40,  8, 48, 16, 56, 24, 64, 32,
									39,  7, 47, 15, 55, 23, 63, 31,
									38,  6, 46, 14, 54, 22, 62, 30,
									37,  5, 45, 13, 53, 21, 61, 29,
									36,  4, 44, 12, 52, 20, 60, 28,
									35,  3, 43, 11, 51, 19, 59, 27,
									34,  2, 42, 10, 50, 18, 58, 26,
									33,  1, 41,  9, 49, 17, 57, 25};


int initial_key_permutaion[] = {57, 49,  41, 33,  25,  17,  9,
								1, 58,  50, 42,  34,  26, 18,
								10,  2,  59, 51,  43,  35, 27,
								19, 11,   3, 60,  52,  44, 36,
								63, 55,  47, 39,  31,  23, 15,
								7, 62,  54, 46,  38,  30, 22,
								14,  6,  61, 53,  45,  37, 29,
								21, 13,   5, 28,  20,  12,  4};

int sub_key_permutation[] =    {14, 17, 11, 24,  1,  5,
								 3, 28, 15,  6, 21, 10,
								23, 19, 12,  4, 26,  8,
								16,  7, 27, 20, 13,  2,
								41, 52, 31, 37, 47, 55,
								30, 40, 51, 45, 33, 48,
								44, 49, 39, 56, 34, 53,
								46, 42, 50, 36, 29, 32};

int key_shift_sizes[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

void encrypt(char *encrypted_message, char *plain_text_message, sub_key_set *sub_keys){
    char *input_permuted = calloc(sizeof(char), BLOCK_SIZE+1);
    input_permuted = initial_permutation(plain_text_message);
	// TO DO
	free(input_permuted);
}

void generate_sub_keys(char *key, sub_key_set *sub_keys){

	/* Initial permutation */
	char *key_56_bits = calloc(sizeof(char), 57);
	key_56_bits = key_transformation(key);
	key_56_bits[56] = '\0';
	
	/* Split the key into left and right halves, where each half has 28 bits */
	strncpy(sub_keys[0].c, substr(key_56_bits, 0, 28), 28);
	strncpy(sub_keys[0].d, substr(key_56_bits, 28, 56), 28);

	/* Each pair of subkeys is obtained by performing left shift operation (according to skey_shift_sizes[]), on the corresponding previous subkeys */
	for(int i=1; i<=NUMBER_OF_ROUNDS; i++){
		char *sub_key_56_bits = calloc(sizeof(char), 57);
		/* Apply left shifts on left and right sub keys */
		shift_left(sub_keys[i].c, sub_keys[i-1].c, key_shift_sizes[i-1]);
		shift_left(sub_keys[i].d, sub_keys[i-1].d, key_shift_sizes[i-1]);
		/* Concatenate left shifted halves */
		strncpy(sub_key_56_bits, sub_keys[i].c, 28);
		strncat(sub_key_56_bits, sub_keys[i].d, 28);

		/* Apply PC2 on concatenated string to obtain sub key */
		strncpy(sub_keys[i].sub_key, PC2(sub_key_56_bits), 48);
	}
	free(key_56_bits);
}

char *initial_permutation(char *input){
    char *output = calloc(sizeof(char), BLOCK_SIZE+1);
    for(int i=0; i<BLOCK_SIZE; i++){
		int index = initial_message_permutation[i]-1;
        output[i] = input[index];
    }
	output[BLOCK_SIZE] = '\0';
    return output;
}

char *PC2(char *input){
	char *output = calloc(sizeof(char), 48+1);
    for(int i=0; i<48; i++){
		int index = sub_key_permutation[i]-1;
        output[i] = input[index];
    }
	output[48] = '\0';
    return output;
}

char *final_permutation(char *input){
	char *output = calloc(sizeof(char), BLOCK_SIZE+1);
	for(int i=0; i<BLOCK_SIZE; i++){
		int index = final_message_permutation[i]-1;
		output[i] = input[index];
	}
	output[BLOCK_SIZE] = '\0';
	return output;
}

char *key_transformation(char *key_64_bit){
	char *key_56_bits = calloc(sizeof(char), 57);
	key_56_bits[56] = '\0';
	for(int i=0;i<56; i++){
		int index = initial_key_permutaion[i];
		key_56_bits[i] = key_64_bit[index];
	}
	return key_56_bits;
}


