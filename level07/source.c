#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define STORAGE_SIZE 100

int clear_stdin() {
    int result;
    do {
        result = getchar();
    } while (result != '\n' && result != EOF);
    return result;
}

int get_unum() {
    int v1 = 0;
    fflush(stdout);
    if (scanf("%d", &v1) != 1) {
        v1 = 0;
    }
    clear_stdin();
    return v1;
}

int store_number(int *storage) {
    int number, index;
    
    printf(" Number: ");
    number = get_unum();
    
    printf(" Index: ");
    index = get_unum();
    
    if (index % 3 == 0 || ((number >> 24) & 0xFF) == 183) {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1;
    }
    
    storage[index] = number;
    return 0;
}

int read_number(int *storage) {
    int index;
    
    printf(" Index: ");
    index = get_unum();
    
    if (index >= STORAGE_SIZE) {
        puts(" *** ERROR! Invalid index! ***");
        return 1;
    }
    
    printf(" Number at data[%u] is %u\n", index, storage[index]);
    return 0;
}

int main() {
    int storage[STORAGE_SIZE];
    char command[20];
    
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
        if (!fgets(command, sizeof(command), stdin)) {
            continue;
        }
        
        command[strcspn(command, "\n")] = 0; // Supprimer le \n final
        
        if (strcmp(command, "store") == 0) {
            if (store_number(storage)) {
                printf(" Failed to do store command\n");
            } else {
                printf(" Completed store command successfully\n");
            }
        } else if (strcmp(command, "read") == 0) {
            if (read_number(storage)) {
                printf(" Failed to do read command\n");
            } else {
                printf(" Completed read command successfully\n");
            }
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            puts(" Invalid command!");
        }
    }
    
    return 0;
}