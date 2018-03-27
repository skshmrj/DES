#define ENCRYPTION_MODE 1
#define DECRYPTION_MODE 0
#define BLOCK_SIZE 64
#define NUMBER_OF_ROUNDS 16

typedef struct {
    char c[29];
    char d[29];
    char sub_key[49];
} sub_key_set;

void shift_left(char *str_shifted, char *arr, int n);
char *substr(char *str, int start_index, int end_index);
char *string_to_binary(char *message);
char *read_64_bit_data_from_file(FILE *file, size_t *number_of_chars_read);
char *read_key_from_file(FILE *file);
void generate_key(FILE *file);
void encrypt(char *encrypted_message, char *plain_text_message, sub_key_set *sub_keys);
void generate_sub_keys(char *key, sub_key_set *sub_keys);
char *XOR(char *A, char *B);
int binary_to_decimal(char *binary);
char *decimal_to_binary(int decimal);