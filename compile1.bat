mkdir build
gcc -I../include -I"C:\msys64\mingw64\include" -c src/computer_informations.c -o build/computer_informations.o
gcc -I../include -I"C:\msys64\mingw64\include" -c src/http.c -o build/http.o
gcc -I../include -I"C:\msys64\mingw64\include" -c src/uid.c -o build/uid.o
gcc -I../include -I"C:\msys64\mingw64\include" -c src/main.c -o build/main.o
gcc build/http.o build/computer_informations.o build/uid.o build/main.o -o build/main -L"C:\msys64\mingw64\lib" -lcurl -lole32 -luuid
del "build\\main.o"
del "build\\http.o"
del "build\\uid.o"
del "build\\computer_informations.o"