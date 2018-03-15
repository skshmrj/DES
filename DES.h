#define ENCRYPTION_MODE 1
#define DECRYPTION_MODE 0
#define BLOCK_SIZE 64

char *string_to_binary(char *message, int length);
char *read_64_bit_data_from_file(FILE *file, size_t *number_of_chars_read);
char *read_key_from_file(FILE *file);
void generate_key(FILE *file);
char *encrypt(char *key, char *input);