mkdir build
gcc -I../include -c src/computer_informations.c -o build/computer_informations.o
gcc -I../include -c src/http.c -o build/http.o
gcc -I../include -c src/uid.c -o build/uid.o
gcc -I../include -c src/main.c -o build/main.o
gcc build/http.o  build/computer_informations.o build/uid.o build/main.o -o build/main -lcurl -lole32 -luuid
del "build\\main.o"
del "build\\http.o"
del "build\\uid.o"
del "build\\computer_informations.o"
