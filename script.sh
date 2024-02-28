g++ -c main.cpp inst_replacer.cpp inst_replacer.h

g++ -o test main.o inst_replacer.o 
# ./main asm.s