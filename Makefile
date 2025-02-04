#CC = gcc -fPIC
#CFLAGS = -Wall `sdl-config --cflags`
#LIBS = `sdl-config --libs` -lm -lSDL_ttf -lSDL_image

#%: %.c
#	rm -f $@
#	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS)


#variable du compilateur gcc
CC = gcc

#The final excutable name
EXEC = app

#All files with .c extension
SRC = $(wildcard *.c)

#All files with .c are copied with .o extension
OBJ = $(SRC:.c=.o)

$(EXEC) : $(OBJ)
	$(CC) -o $@ $^
%.o : %.c
	$(CC) -o $@ -c $<
clean:
	rm -rf *.o
mrproper : clean
	rm -rf $(EXEC)