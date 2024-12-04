#!/bin/bash

# Chemins vers les répertoires d'inclusion et de bibliothèque de libcurl
CURL_INCLUDE_PATH=/usr/local/mingw64/include
CURL_LIB_PATH=/usr/local/mingw64/lib

# Créer le répertoire de build
mkdir -p build

# Compiler les fichiers source avec les chemins d'inclusion supplémentaires
x86_64-w64-mingw32-gcc -Iinclude -I$CURL_INCLUDE_PATH -c src/http.c -o build/http.o
x86_64-w64-mingw32-gcc -Iinclude -I$CURL_INCLUDE_PATH -c src/main.c -o build/main.o

# Lier les objets et créer l'exécutable avec les chemins de bibliothèque supplémentaires
x86_64-w64-mingw32-gcc build/http.o build/main.o -o build/main.exe -L$CURL_LIB_PATH -lcurl -lole32 -luuid

# Nettoyer les fichiers objets
rm build/main.o
rm build/http.o