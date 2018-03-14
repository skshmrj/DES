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

    char *key = malloc(sizeof(char) * 8);

    /* Opening files for reading message and key. Filenames are supplied as the command line arguments 
        with '-m' flag for file having the message and -k for file having the key */
    int arg, prev_ind;
    FILE *file;
    while(prev_ind = optind, (arg = getopt(argc, argv, "g:m:k:")) != -1){
        if ( optind == prev_ind + 2 && *optarg == '-' ) {
            arg = ':';
            -- optind;
        }
        switch (arg){
            case 'g':
                if (!(file = fopen(optarg, "w"))) {
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
                size_t *number_of_chars_read;
                char *input = malloc(sizeof(char) * 64);
                char *output = malloc(sizeof(char) * 8);
                do{
                    fseek(file, pos, 0);
                    char *message = read_64_bit_data_from_file(file, number_of_chars_read);
                    pos = ftell(file);
                    input = string_to_binary(message);
                    // TO DO : Convert the message to binary. Add padding if necessary and apply the DES algorithm
                 }while(*number_of_chars_read==8);
                 fclose(file);
                break;
            default:
                abort ();
        }
    }
    return 0;
}
