gcc -I../include -c src/http.c -o build/http.o && gcc -I../include -c src/main.c -o build/main.o
gcc build/http.o build/main.o -o build/main -lcurl
del "build\\main.o"
del "build\\http.o"