Ce level est comme le level03. il faut reverse engineer le code pour trouver un password.

Le programme vas nous demander deux entrer différente:

> Login
> Serial

c'est deux entrer vont passer dans une fonction __auth__ qui vas encrypter le login en un nombre qui devras être identique au serial pour passer l'authentification et acceder a un __"/bin/sh"__

pour trouver le bon serial on récupère la partie encrypting du programme et on regarde ce quelle nous donne avec un login donné.

on récupère donc le serial ce qui nous donne accès au __"bin/sh"__.

on execute donc apres: 

```bash
cat /home/users/level07/.pass
```
