#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DES.h"
#include "utilities.h"

int main(){
    char *message = malloc(sizeof(char) * 1024);
    char *key = malloc(sizeof(char) * 8);
    printf("\nThis is an implementation of DES (Data Encryption Standard).\n");
    printf("\nEnter message to be encrypted.\n(Note: If left blank, input will be taken from default <input> file)\n");
    // fgets(message, 1024, stdin);
    gets(message);
    char *res = string_to_binary(message);
    printf("%ld\n", strlen(res));
    puts(res);
    // printf("\nEnter key.\n(Note: If left blank, default key will be selected from <key> file. A valid key length is 8 characters.)\n");
    // fgets(key, 8, stdin);
    return 0;
}