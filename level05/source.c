#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
    int i = 0;
    char buffer[100]; // 100

    fgets(buffer, 100, stdin);
    while (i < strlen(buffer)) {
        if (buffer[i] > 64 && buffer[i] <= 90) {
            buffer[i] ^= 0x20;
        }
        i++;
    }
    printf(buffer);
    exit(0);
}