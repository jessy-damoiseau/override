
En decompilant le programe level00 on vois que si on
rentre le bon pwd un bash c'execute.

on trouve un if de verification:
```c
if ( v4 == 5276 ){
	system("/bin/sh");
}
```

le pwd est donc 5276.

une fois le bash lancer on execute:

```bash
cat /home/users/level01/.pass
```

on obtient donc le flag