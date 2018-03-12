#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char *itoa(int i){
    int len = (int)(sizeof(char) * (i+1));
    char *a = malloc(sizeof(char) * len);
    sprintf(a, "%d", i);
    return a;
}

void swap(char *a, char *b){   
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

char *strrev(char *str){
    int left = 0, right = strlen(str)-1;
    for(int left=0, right=strlen(str)-1; left<right; left++,right--){
        swap(str+left,str+right);
    }
    return str;
}

char *char_as_binary(char dec){
    char *dec_as_binary = calloc(sizeof(char), 9);
    do{
        int rem = dec%2;
        char *res = itoa(rem);
        strncat(dec_as_binary, res, 1);
        dec /= 2;
    }while(dec != 0);

    int len = strlen(dec_as_binary);
    if(len<8){
        int n_padding = 8-len;
        for(int i=0; i<n_padding; i++){    
            dec_as_binary[len+i] = '0';
        }
    }
    return strrev(dec_as_binary);
}

char *string_to_binary(char *message){
    
    int len_message_in_binary = 64;
    char *message_in_binary = calloc(sizeof(char), len_message_in_binary);
    for(int i=0; i<8; i++){
        char *c = (i<strlen(message))? char_as_binary(message[i]): char_as_binary('0');
        strncat(message_in_binary, c, 8);
    }
    return message_in_binary;
}

char *read_64_bit_data_from_file(FILE *file, size_t *number_of_chars_read){
    char *buffer = malloc(sizeof(char) * 8);
    *number_of_chars_read  = fread(buffer, 1, 8, file);
    return buffer;
}

char *read_key_from_file(FILE *file){
    //Get size of the file
    fseek(file , 0 , SEEK_END);
    long int lSize = ftell(file)-1;
    rewind(file);
    char *key = malloc(sizeof(char) * lSize);
    size_t key_length = fread(key, 1, lSize, file);
    // Check if the file has a valid key i.e. a key of 8 bytes
    if(key_length != 8){
        fputs("Invalid key!", stderr);
        exit (1);
    }
    return key;
}