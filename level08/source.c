#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

uint64_t log_wrapper(FILE *a1, const char *a2, const char *a3) {
    char dest[264];
    strcpy(dest, a2);
    snprintf(&dest[strlen(dest)], 254 - strlen(dest), "%s", a3);
    dest[strcspn(dest, "\n")] = 0;
    fprintf(a1, "LOG: %s\n", dest);
    return 0; // Suppression de la protection de stack
}

int main(int argc, char **argv) {
    FILE *v4;
    FILE *stream;
    int fd;
    char buf;
    char dest[104];

    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }
    
    v4 = fopen("./backups/.log", "w");
    if (!v4) {
        printf("ERROR: Failed to open %s\n", "./backups/.log");
        return 1;
    }
    
    log_wrapper(v4, "Starting back up: ", argv[1]);
    
    stream = fopen(argv[1], "r");
    if (!stream) {
        printf("ERROR: Failed to open %s\n", argv[1]);
        fclose(v4);
        return 1;
    }
    
    snprintf(dest, sizeof(dest), "./backups/%s", argv[1]);
    fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if (fd < 0) {
        printf("ERROR: Failed to open %s\n", dest);
        fclose(stream);
        fclose(v4);
        return 1;
    }
    
    while ((buf = fgetc(stream)) != EOF) {
        write(fd, &buf, 1);
    }
    
    log_wrapper(v4, "Finished back up ", argv[1]);
    
    fclose(stream);
    close(fd);
    fclose(v4);
    
    return 0;
}