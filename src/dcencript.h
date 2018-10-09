/*
Got this from: https://stackoverflow.com/questions/13801231/seeking-single-file-encryption-implemenation-which-can-handle-whole-file-en-de-c
*/

#ifndef ENCRIPT
#define ENCRIPT

/*
 * rc4.h -- Declarations for a simple rc4 encryption/decryption implementation.
 * The code was inspired by libtomcrypt.  See www.libtomcrypt.org.
 */
typedef struct TRC4State_s
{
    int x, y;
    unsigned char buf[256];
} TRC4State;

/* rc4.c */
void init_rc4(TRC4State *state);
void setup_rc4(TRC4State *state, char *key, int keylen);
unsigned endecrypt_rc4(unsigned char *buf, unsigned len, TRC4State *state);

#endif