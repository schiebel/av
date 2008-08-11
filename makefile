
CXXFLAGS:=-DPGPLOT -DAIPS_LITTLE_ENDIAN -g
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)

av: $(OBJ)
	g++ -o av *.o -L/opt/local/lib -lpgsbox -lcpgplot -lpgplot -lwcs -lXm -L/usr/X11R6/lib -lXt -lX11 -lgfortran
