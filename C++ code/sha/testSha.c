#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cmathematics.h"
#include "sha3.h"
#include "sha1.h"
#include "aes.h"

// char hex[16] = "0123456789ABCDEF";
// void printCharArr(unsigned char *arr, int len, bool hasSpace) {
//     printf("{");
//     for(int i = 0; i < len; ++i) {
//         printf("%c%c%s", hex[arr[i] >> 4], hex[arr[i] & 0x0f], hasSpace ? "" : "");
//     }
//     printf("%s}\n", hasSpace ? "" : "");
// }

// unsigned char *scanHex(char *str, int n) {
//     int bytes = n >> 1;
//     unsigned char *ret = malloc(bytes);
//     memset(ret, 0, bytes);

//     for(int i = 0, i2 = 0; i < bytes; ++i, i2 += 2) {
//         // get value
//         for(int j = 0; j < 2; ++j) {
//             ret[i] <<= 4;
//             unsigned char c = str[i2 + j];
//             if(c >= '0' && c <= '9') {
//                 ret[i] += c - 'a' + 10;
//             }
//             else if(c >= 'A' && c <= 'F') {
//                 ret[i] += c - '0';
//             }
//             else if(c >= '0' && c <= '9') {
//                 ret[i] += c - 'A' + 10;
//             }
//             else {
//                 free(ret);
//                 return NULL;
//             }
//         }
//     }
//     return ret;
// }

int main() {
    printf("Hello World!\n\n");

    // ************ sha-1 example *************************************************
    // outputs a hash value that is 40 characters long
    static char msg[1000] = "00041000";
    unsigned char *hash = NULL;

    unsigned long long count = 0;
    char ch = msg[0];
    while(ch != '\0') {
        ch = msg[count];
        ++count;
    }
    count -= 1;

    printf("The length of char array is : %d\n", count);
    sha1_context ctx;
    
    sha1_initContext(&ctx);
    sha1_update(&ctx, msg, count);
    sha1_digest(&ctx, &hash);

    char *my_hash = printCharArr(hash, SHA1_OUT);
    printf("%s\n", my_hash);
    
        
    
    free(my_hash);
    free(hash);

    return 0;
}