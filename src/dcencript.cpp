/*
Got this from: https://stackoverflow.com/questions/13801231/seeking-single-file-encryption-implemenation-which-can-handle-whole-file-en-de-c
RC4 incription, not the best form of encription, there are know vulrnabilities for it.
However for our purposes it will randomise any data enough that it will be impossible to tell anything is there.
If the defender knows enough to know that there is hidden data encripted in RC4 then we have bigger problems.
*/

#include "dcencript.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void init_rc4(TRC4State *state)
{
    int x;
    state->x = state->y = 0;
    for (x = 0; x < 256; x++)
        state->buf[x] = x;
}

void setup_rc4(TRC4State *state, const char *key, int keylen)
{
    unsigned tmp;
    int x, y;

    // use only first 256 characters of key
    if (keylen > 256)
        keylen = 256;

    for (x = y = 0; x < 256; x++)
    {
        y = (y + state->buf[x] + key[x % keylen]) & 255;
        tmp = state->buf[x];
        state->buf[x] = state->buf[y];
        state->buf[y] = tmp;
    }
    state->x = 255;
    state->y = y;
}

unsigned endecrypt_rc4(unsigned char *buf, unsigned len, TRC4State *state)
{
    int x, y;
    unsigned char *s, tmp;
    unsigned n;

    x = state->x;
    y = state->y;
    s = state->buf;
    n = len;
    while (n--)
    {
        x = (x + 1) & 255;
        y = (y + s[x]) & 255;
        tmp = s[x];
        s[x] = s[y];
        s[y] = tmp;
        tmp = (s[x] + s[y]) & 255;
        *buf++ ^= s[tmp];
    }
    state->x = x;
    state->y = y;
    return len;
}

int endecrypt_file(FILE *f_in, FILE *f_out, const char *key)
{
    TRC4State state[1];
    unsigned char buf[4096];
    size_t n_read, n_written;

    init_rc4(state);
    setup_rc4(state, key, strlen(key));
    do
    {
        n_read = fread(buf, 1, sizeof buf, f_in);
        endecrypt_rc4(buf, n_read, state);
        n_written = fwrite(buf, 1, n_read, f_out);
    } while (n_read == sizeof buf && n_written == n_read);
    return (n_written == n_read) ? 0 : 1;
}

int endecrypt_file_at(const char *f_in_name, const char *f_out_name, const char *key)
{
    int rtn;

    FILE *f_in = fopen(f_in_name, "rb");
    if (!f_in)
    {
        return 1;
    }
    FILE *f_out = fopen(f_out_name, "wb");
    if (!f_out)
    {
        fclose(f_in);
        return 2;
    }
    rtn = endecrypt_file(f_in, f_out, key);
    fclose(f_in);
    fclose(f_out);
    return rtn;
}

#ifdef TEST

// Simple test.
int main(void)
{
    char *key = "This is the key!";

    endecrypt_file_at("rc4.pas", "rc4-scrambled.c", key);
    endecrypt_file_at("rc4-scrambled.c", "rc4-unscrambled.c", key);
    return 0;
}
#endif