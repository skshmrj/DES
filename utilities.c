#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
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
    for(int i=(int)strlen(message_in_binary); i<64; i++){
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
    char *key = calloc(sizeof(char), lSize);
    size_t key_length = fread(key, 1, lSize, file);
    // Check if the file has a valid key i.e. a key of 8 bytes
    if(key_length != 8){
        fputs("Invalid key!", stderr);
        exit (1);
    }
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