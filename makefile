# Compile/Link 
GCC = gcc 
OPTMIZE = -g -O3 -w
LIBS = -lm
#Objetos 
OBJS =  tp1.c arvoreb.c memoriaPrincipal.c
# Nome do aplicativo 
APPNAME = tp1
release : ; $(GCC) $(OPTMIZE) $(LIBS) $(OBJS) -o $(APPNAME) 
clean : rm -rf *.o 
run :
	make 
	./tp1 input.txt output.txt
