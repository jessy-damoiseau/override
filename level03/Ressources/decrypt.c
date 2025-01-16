#include <stdio.h>
#include <string.h>

int main() {
    char encrypted[] = "Q}|u`sfg~sf{}|a3";
    char expected[] = "Congratulations!";
    int a1 = 0;

    for (int i = 0; i < strlen(expected); i++) {
        a1 = encrypted[i] ^ expected[i];
        printf("Character %c decrypted to %c with key %d\n", encrypted[i], expected[i], a1);
    }

    printf("\nCorrect key (a1) is: %d\n", a1);
    return 0;
}
