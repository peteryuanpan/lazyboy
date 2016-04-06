g++ XQWSmain.cpp -c -o XQWSmain.o
g++ Ucci.cpp -c -o Ucci.o
g++ *.o -o xqws

./xqws

rm *.o xqws
