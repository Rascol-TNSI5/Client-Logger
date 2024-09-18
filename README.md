# Client-Logger

---

Ici se trouvera désormais l'ensemble du code servant au '_client_', qui sera donc compilé en un seul fichier exécutable.

## Compilation

Pour compiler le programe il faudra [msys2](https://www.msys2.org/) en plus de [MinGW](https://sourceforge.net/projects/mingw/).

**Dans msys2 il faut executer les 2 commandes suivante**:

```sh
pacman -S mingw-w64-ucrt-x86_64-gcc

pacman -S mingw-w64-ucrt-x86_64-curl
```

Ensuite, il faut ajouter "_C:\msys64\ucrt64\bin_" dans la variable d'environnement Path

Puis exécute le fichier suivant dans le dossier ou se trouve _main.c_:

```sh
.\compile.bat
```

## License

Ce Projet est sous la License MIT.
