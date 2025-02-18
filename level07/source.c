#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Vide le tampon d'entrée jusqu'au saut de ligne ou EOF */
void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Lit un nombre entier depuis l'entrée standard */
int get_unum(void) {
    int num = 0;
    fflush(stdout);
    scanf("%d", &num);
    clear_stdin();
    return num;
}

/* Stocke un nombre dans le tableau de données.
   La fonction refuse de stocker si l'index est multiple de 3
   ou si le high-byte du nombre vaut 183. */
int store_number(int *data) {
    unsigned int number, index;
    
    printf(" Number: ");
    number = get_unum();
    
    printf(" Index: ");
    index = get_unum();
    
    /* Si l'index est un multiple de 3 ou si le high-byte du nombre vaut 183 */
    if ((index % 3 == 0) || (((number >> 8) & 0xFF) == 183)) {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1;
    } else {
        data[index] = number;
        return 0;
    }
}

/* Lit et affiche le nombre stocké à l'index donné */
int read_number(int *data) {
    int index;
    
    printf(" Index: ");
    index = get_unum();
    
    printf(" Number at data[%u] is %u\n", (unsigned int)index, (unsigned int)data[index]);
    return 0;
}

int main(int argc, char **argv, char **envp) {
    /* 400 octets pour le stockage (tableau de 100 int) */
    int data[100] = {0};
    char command[20] = {0};
    int ret = 0;
    int i;
    
    /* Efface le contenu des arguments de la ligne de commande */
    for (i = 0; argv[i] != NULL; i++) {
        memset(argv[i], 0, strlen(argv[i]));
    }
    
    /* Efface le contenu des variables d'environnement */
    for (i = 0; envp[i] != NULL; i++) {
        memset(envp[i], 0, strlen(envp[i]));
    }
    
    puts("----------------------------------------------------");
    puts("  Welcome to wil's crappy number storage service!   ");
    puts("----------------------------------------------------");
    puts(" Commands:                                          ");
    puts("    store - store a number into the data storage    ");
    puts("    read  - read a number from the data storage     ");
    puts("    quit  - exit the program                        ");
    puts("----------------------------------------------------");
    puts("   wil has reserved some storage :>                 ");
    puts("----------------------------------------------------");
    
    while (1) {
        printf("Input command: ");
        ret = 1;  /* valeur par défaut : commande échouée */
        
        if (fgets(command, sizeof(command), stdin) == NULL)
            break;
        
        /* Supprime le saut de ligne final */
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n')
            command[len - 1] = '\0';
        
        if (strncmp(command, "store", 5) == 0) {
            ret = store_number(data);
        } else if (strncmp(command, "read", 4) == 0) {
            ret = read_number(data);
        } else if (strncmp(command, "quit", 4) == 0) {
            break;
        }
        
        if (ret)
            printf(" Failed to do %s command\n", command);
        else
            printf(" Completed %s command successfully\n", command);
        
        memset(command, 0, sizeof(command));
    }
    
    return 0;
}
