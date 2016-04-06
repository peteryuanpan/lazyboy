g++ ChessBoard.cpp -c -o ChessBoard.o
g++ GraphEdge.cpp -c -o GraphEdge.o
g++ HashTable.cpp -c -o HashTable.o
g++ BasicFunction.cpp -c -o BasicFunction.o
g++ MoveToFuckOrAvoid.cpp -c -o MoveToFuckOrAvoid.o
g++ TurnRed.cpp -c -o TurnRed.o
g++ TurnBlack.cpp -c -o TurnBlack.o
g++ MainSearch.cpp -c -o MainSearch.o
g++ Shellmain.cpp -c -o Shellmain.o

g++ *.o -o xxx
./xxx

rm *.o xxx
