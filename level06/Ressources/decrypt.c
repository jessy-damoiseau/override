#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    char *login = "level06";
    int len, i;
    unsigned int bin;

    len = strnlen(login, 32);
    if (len <= 5) {
        return 1;
    }

    bin = (login[3] ^ 0x1337) + 6221293;
    for (i = 0; i < len; ++i) {
        if (login[i] <= 31) {
            return 1;
        }
        bin += (bin ^ (unsigned int)login[i]) % 1337;
    }
    printf("for |%s|, the serial is |%u|\n", login, bin);
    return;
}