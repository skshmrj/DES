#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "DES.h"
#include "utilities.h"

int main(int argc, char **argv){

    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    printf("\nThis is an implementation of DES (Data Encryption Standard).");
    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");

    char *message = malloc(sizeof(char) * 1024);
    char *key = malloc(sizeof(char) * 8);

    /* Opening files for reading message and key. Filenames are supplied as the command line arguments 
        with '-m' flag for file having the message and -k for file having the key */
    int arg, prev_ind;
    FILE *file;
    while(prev_ind = optind, (arg = getopt(argc, argv, "m:k:")) != -1){
        if ( optind == prev_ind + 2 && *optarg == '-' ) {
            arg = ':';
            -- optind;
        }
        switch (arg){
            case 'm':
                if (!(file = fopen(optarg, "r"))) {
                    perror(optarg);
                    abort ();
                }

                break;
            case 'k':
                if (!(file = fopen(optarg, "r"))) {
                    perror(optarg);
                    abort ();
                }
                break;
            default:
                abort ();
        }
    }

    // char *res = string_to_binary(message);
    return 0;
}