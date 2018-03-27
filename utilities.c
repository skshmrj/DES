#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <math.h>
#include "DES.h"

void swap(char *a, char *b){   
    
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

char *strrev(char *str){
    
    for(int left=0, right=strlen(str)-1; left<right; left++,right--){
        swap(str+left,str+right);
    }
    return str;
}

char *char_as_binary(char dec){
    
    char *dec_as_binary = calloc(sizeof(char), 9);
    char *res = calloc(sizeof(char), 2);
    int len = 0;

    do{
        int rem = dec%2;
        sprintf(res, "%d", rem);
        strncat(dec_as_binary, res, 1);
        dec /= 2; len++;
    }while(dec != 0);
    
    /* Pad with 0's to get string of length 8 */
    if(len<8){
        int n_padding = 8-len;
        for(int i=0; i<n_padding; i++){    
            dec_as_binary[len+i] = '0';
        }
    }
    /* Null terminate to make it a string */
    dec_as_binary[8] = '\0';

    /* Free allocated memory */
    free(res);
    
    return strrev(dec_as_binary);
}

char *string_to_binary(char *message){

    char *message_in_binary = calloc(sizeof(char), BLOCK_SIZE+1);
    char *c = calloc(sizeof(char), 9);
    
    for(int i=0; i<(int)strlen(message); i++){
        c = char_as_binary(message[i]);
        strncat(message_in_binary, c, 8);
    }

    /* Null terminate to make it a string */
    message_in_binary[BLOCK_SIZE] = '\0';
    
    /* Pad with 0's to get string of length 64 */
    for(int i=(int)strlen(message_in_binary); i<BLOCK_SIZE; i++){
        message_in_binary[i] = '0';
    }

    /* Free allocated memory */
    free(c);
    
    return message_in_binary;
}

char *read_64_bit_data_from_file(FILE *file, size_t *number_of_chars_read){
    
    char *buffer = calloc(sizeof(char),  9);
    *number_of_chars_read  = fread(buffer, 1, 8, file);
    buffer[8] = '\0';
    return buffer;
}

char *read_key_from_file(FILE *file){
    //Get size of the file
    fseek(file , 0 , SEEK_END);
    long int lSize = ftell(file);
    rewind(file);
    char *key = calloc(sizeof(char), lSize+1);
    size_t key_length = fread(key, 1, lSize, file);
    // Check if the file has a valid key i.e. a key of 8 bytes
    if(key_length != 8){
        fputs("Invalid key!", stderr);
        exit (1);
    }
    key[8] = '\0';
    return key;
}

void generate_key(FILE *file){
    char *key = calloc(sizeof(char), 8);
    for(int i=0;i<8;i++){
        key[i] = rand()%255;
    }
    fwrite(key, sizeof(char), 8, file);
    free(key);
}

char *substr(char *str, int start_index, int end_index){
    if (end_index >= (int)strlen(str)+1){
        puts(str);
        printf("length = %d, end_index = %d\n", (int)strlen(str), end_index);       
        fputs("Array index out of bounds.", stderr);
        exit (1);
    }
    char *substring = calloc(sizeof(char), (end_index-start_index+1));
    for(int i=start_index, k=0; i<end_index; i++,k++){
        substring[k] = str[i];
    }
    substring[end_index-start_index] = '\0';
    return substring;
}

void shift_left(char *str_shifted, char *arr, int n){
    int len = strlen(arr);
    char *tmp = calloc(sizeof(char), n);
    for(int i=0;i<n;i++){
        tmp[i] = arr[i];
    }
    for(int i=0; i<len-n; i++){
        str_shifted[i] = arr[i+n]; 
    }
    for(int i=len-n, k=0; i<len; i++, k++){
        str_shifted[i] = tmp[k];
    }
    free(tmp);
    str_shifted[len] = '\0';
}

char *XOR(char *A, char *B){
    int len_1 = (int)strlen(A);
    int len_2 = (int)strlen(B);
    /* Check if the arguments are of equal length */
    if (len_1 != len_2){
        fputs("Cannot do bitwise XOR. Inputs are of different lengths\n", stderr);
        exit (1);
    }

    char *O = calloc(sizeof(char), len_1+1);
    O[len_1] = '\0';
    for(int i=0; i<len_1; i++){
        O[i] = (A[i]==B[i])? '0' : '1';
    }
    return O;
}

int binary_to_decimal(char *binary){
    int len = (int)strlen(binary);
    int dec = 0;
    for(int i=len-1, k=0; i>=0; i--, k++){
        dec += (pow(2.0, k) * (int)(binary[i]-'0'));
    }
	return dec;
}

char *decimal_to_binary(int dec){
    char *dec_as_binary = calloc(sizeof(char), 5);
    dec_as_binary[4] = '\0';
    char *res = calloc(sizeof(char), 2);
    res[1] = '\0';
    do{
        int rem = dec%2;
        sprintf(res, "%d", rem);
        strcat(dec_as_binary, res);
        dec /= 2;
    }while(dec != 0);
    
    /* Null terminate to make it a string */
    dec_as_binary[4] = '\0';

    /* Free allocated memory */
    free(res);

     /* Pad with 0's to get string of length 4 */
     int len = (int)strlen(dec_as_binary);
    if(len<4){
        int n_padding = 4-len;
        for(int i=0; i<n_padding; i++){    
            dec_as_binary[len+i] = '0';
        }
    }
    
    // printf("%s\n", dec_as_binary);

    return strrev(dec_as_binary);
}