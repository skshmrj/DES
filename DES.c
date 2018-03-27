#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "DES.h"

typedef struct {
    char L[BLOCK_SIZE/2 + 1];
    char R[BLOCK_SIZE/2 + 1];
} sub_message;

char *initial_permutation(char *input);
char *permute_sbox_output(char *input);
char *final_permutation(char *input);
char *PC2(char *sub_key_56_bits);
char *key_transformation(char *key_64_bit);
void generate_sub_keys(char *key, sub_key_set *sub_keys);
void generate_sub_messages(char *message, sub_message *sub_messages, sub_key_set *sub_keys);
char *expand_sub_message(char *input);
char *f(char *R, char *K);
char *transform_six_bits(char *six_bits, int s_box_num);
void get_row_and_col(char *six_bits, int *row, int *col);

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

int message_expansion[] =  {32,  1,  2,  3,  4,  5,
							 4,  5,  6,  7,  8,  9,
							 8,  9, 10, 11, 12, 13,
							12, 13, 14, 15, 16, 17,
							16, 17, 18, 19, 20, 21,
							20, 21, 22, 23, 24, 25,
							24, 25, 26, 27, 28, 29,
							28, 29, 30, 31, 32,  1};

const int Sbox[8][4][16] ={
							{
							{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
							{ 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
							{ 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
							{15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
							},
							
							{
							{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
							{ 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
							{ 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
							{13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
							},
							
							{
							{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
							{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
							{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
							{ 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
							},
							
							{
							{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
							{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
							{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
							{ 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
							},
							
							{
							{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
							{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
							{ 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
							{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
							},
							
							{
							{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
							{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
							{ 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
							{ 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
							},
							
							{
							{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
							{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
							{ 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
							{ 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
							},
							
							{
							{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
							{ 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
							{ 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
							{ 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11},
							}};

int right_sub_message_permutation[] = { 16,  7, 20, 21,
										29, 12, 28, 17,
									 	1, 15, 23, 26,
									 	5, 18, 31, 10,
									 	2,  8, 24, 14,
										32, 27,  3,  9,
										19, 13, 30,  6,
										22, 11,  4, 25 };

int key_shift_sizes[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

void encrypt(char *encrypted_message, char *plain_text_message, sub_key_set *sub_keys){
    char *input_permuted = calloc(sizeof(char), BLOCK_SIZE+1);
    input_permuted = initial_permutation(plain_text_message);
	
	/* Generate a set of 16 L and R subparts from the message for each of the rounds */
    sub_message *sub_messages = calloc(sizeof(sub_key_set), 17);
	generate_sub_messages(input_permuted, sub_messages, sub_keys);
	
	char *tmp = calloc(sizeof(char), BLOCK_SIZE+1);
	tmp[BLOCK_SIZE] = '\0';
	strncpy(tmp, sub_messages[16].R, 32);
	strncat(tmp, sub_messages[16].L, 32);

	strncpy(encrypted_message, final_permutation(tmp), 64);
	free(input_permuted);
}

void generate_sub_messages(char *message, sub_message *sub_messages, sub_key_set *sub_keys){
	
	/* Split the message into left and right halves, where each half has 32 bits */
	strncpy(sub_messages[0].L, substr(message, 0, 32), 32);
	strncpy(sub_messages[0].R, substr(message, 32, 64), 32);

	char *permuted_sbox = calloc(sizeof(char), 33);
	permuted_sbox[32] = '\0';
	/* Proceed throught the 16 iterations to calculate L(n) and R(n). L(n) = R(n-1) and R(n) = XOR(L(n-1), f(R(n-1), sub_keys(n))) */
	for(int i=1; i<=NUMBER_OF_ROUNDS; i++){
		strncpy(sub_messages[i].L, sub_messages[i-1].R, 32);
		permuted_sbox = permute_sbox_output(f(sub_messages[i-1].R, sub_keys[i].sub_key));
		strncpy(sub_messages[i].R, XOR(sub_messages[i-1].L, permuted_sbox), 32);
	}
	free(permuted_sbox);
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

char *permute_sbox_output(char *input){
	char *output = calloc(sizeof(char), 32+1);
	for(int i=0; i<32; i++){
		int index = right_sub_message_permutation[i]-1;
		output[i] = input[index];
	}
	output[32] = '\0';
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
		int index = initial_key_permutaion[i]-1;
		key_56_bits[i] = key_64_bit[index];
	}
	return key_56_bits;
}

char *f(char *R, char *K){
	
	char *message_expanded = calloc(sizeof(char), 49);
	message_expanded = expand_sub_message(R);
	
	char *xor = calloc(sizeof(char), 49);
	
	xor = XOR(message_expanded, K);
	char *six_bits = calloc(sizeof(char), 7);
	char *four_bits = calloc(sizeof(char), 5);
	char *output = calloc(sizeof(char), 33);
	six_bits[6] = '\0'; four_bits[4] = '\0'; output[32] = '\0';
	for(int i=0, s_box_num=0; i<=42; i+=6, s_box_num++){
		six_bits = substr(xor, i, i+6);
		four_bits = transform_six_bits(six_bits, s_box_num);
		strncat(output, four_bits, 4);

	}
	// TO DO : Apply S-box permutation
	free(six_bits);
	free(xor);
	free(message_expanded);
	// printf("Length of return of f : %ld\n\n", strlen(output));
	return output;
}

char *expand_sub_message(char *input){
	
	char *output = calloc(sizeof(char), 49);
	output[48] = '\0';
	for(int i=0; i<48; i++){
		int index = message_expansion[i]-1;
		output[i] = input[index];
	}
	return output;
}

char *transform_six_bits(char *six_bits, int s_box_num){

	int row, col;
	get_row_and_col(six_bits, &row, &col);
	
	int val = Sbox[s_box_num][row][col];
	char *four_bits = calloc(sizeof(char), 5);
	four_bits[4] = '\0';
	four_bits = decimal_to_binary(val);
	return four_bits;
}

void get_row_and_col(char *six_bits, int *row, int *col){
	char *R = calloc(sizeof(char), 3);
	char *C = calloc(sizeof(char), 5);
	R[0] = six_bits[0]; R[1] = six_bits[5]; R[2] = '\0';
	strncpy(C, substr(six_bits, 1, 5), 4);
	*row = binary_to_decimal(R);
	*col = binary_to_decimal(C);
	free(C);
	free(R);
}


