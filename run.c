#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "DES.h"

int main(int argc, char **argv){

    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    printf("\nThis is an implementation of DES (Data Encryption Standard).");
    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");

    char *key = calloc(sizeof(char), 9);

    /* Opening files for reading message and key. Filenames are supplied as the command line arguments 
        with '-m' flag for file having the message and -k for file having the key . Use -g flag to generate file having key. */
    int arg, prev_ind;
    FILE *file;
    while(prev_ind = optind, (arg = getopt(argc, argv, "g:m:k:")) != -1){
        if ( optind == prev_ind + 2 && *optarg == '-' ) {
            arg = ':';
            -- optind;
        }
        switch (arg){
            case 'g':
                if (!(file = fopen(strcat(optarg,".key"), "w"))) {
                    perror(optarg);
                    exit (1);
                }
                generate_key(file);
                fclose(file);
                break;
            case 'k':
                if (!(file = fopen(optarg, "r"))) {
                    perror(optarg);
                    abort ();
                }
                key = read_key_from_file(file);
                fclose(file);
                break;
            case 'm':
                if (!(file = fopen(optarg, "r"))) {
                    perror(optarg);
                    abort ();
                }

                /* Read data from file in chunks of 8 bytes */
                long int pos=0;
                size_t num = 0;
                size_t *number_of_chars_read = &num;
                // char *plain_text_message = calloc(sizeof(char), BLOCK_SIZE+1);                // Will be used for 64 bit message, having 0's and 1's
                char plain_text_message[64] = "0000000100100011010001010110011110001001101010111100110111101111";
                char *encrypted_message = calloc(sizeof(char), BLOCK_SIZE+1);                 // Will be used for 64 bit encrypted message
                char *message = calloc(sizeof(char), 9);                                      // Will be used for text message read from file
                // char *key_64_bit = calloc(sizeof(char), BLOCK_SIZE+1);
                // key_64_bit = string_to_binary(key);
                char key_64_bit[64] = "0001001100110100010101110111100110011011101111001101111111110001";

                /* Generate a set of 16 subkeys for each of the rounds */
                sub_key_set *sub_keys = calloc(sizeof(sub_key_set), 17);
	            generate_sub_keys(key_64_bit, sub_keys);
                
                /* Start encrypting data from file 8 bytes at a time */
                // do{
                //     fseek(file, pos, SEEK_SET);
                //     message = read_64_bit_data_from_file(file, number_of_chars_read);
                //     if (*number_of_chars_read == 0) continue;
                //     pos = ftell(file);
                //     plain_text_message = string_to_binary(message);
                //     encrypt(encrypted_message, plain_text_message, sub_keys);
                //     // TO DO : Convert the message to binary. Add padding if necessary and apply the DES algorithm
                // }while(*number_of_chars_read==8);
                encrypt(encrypted_message, plain_text_message, sub_keys);

                /* Free allocated memory */
                free(message);
                free(encrypted_message);
                // free(plain_text_message);

                /* Close open files */
                fclose(file);
                break;
            default:
                abort ();
        }
    }
    free(key);
    return 0;
}
