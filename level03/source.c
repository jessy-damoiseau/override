#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int decrypt(char a1) {
    unsigned int i;
    unsigned int v3;
    char v4[29];

    // Initialisation du buffer avec une chaîne codée
    strcpy(v4, "Q}|u`sfg~sf{}|a3");
    v3 = strlen(v4);

    // Décryptage de chaque caractère
    for (i = 0; i < v3; ++i) {
        v4[i] ^= a1;
    }

    // Vérification si le résultat correspond à "Congratulations!"
    if (!strcmp(v4, "Congratulations!")) {
        return system("/bin/sh");
    } else {
        return puts("\nInvalid Password");
    }
}

int test(int a1, int a2) {
    int result;
    char v3;

    // Différents cas pour appeler decrypt
    switch (a2 - a1) {
        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
        case 8: case 9: case 16: case 17: case 18: case 19:
        case 20: case 21:
            result = decrypt(a2 - a1);
            break;
        default:
            v3 = rand() % 256;  // Générer un caractère aléatoire
            result = decrypt(v3);
            break;
    }
    return result;
}

int main(int argc, const char **argv) {
    unsigned int seed;
    int input;

    // Initialisation de la graine pour le générateur de nombres aléatoires
    seed = (unsigned int)time(NULL);
    srand(seed);

    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");
    printf("Password: ");
    if (scanf("%d", &input) != 1) {
        puts("Invalid input.");
        return 1;
    }

    // Appeler la fonction de test avec l'entrée utilisateur et une constante
    test(input, 322424845);
    return 0;
}
