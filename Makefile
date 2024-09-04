CFLAGS   = -g `sdl2-config --cflags`
LDFLAGS  = `sdl2-config --libs` -lSDL2_ttf -lm
PROG = tetris
CXX = gcc

OBJS   = tetris_main.o view.o model.o controller.o file.o

# top-level rule to create the program.
all: $(PROG)

# compiling other source files.
%.o: %.c %.h
	$(CXX) $(CFLAGS) -c $<

# linking the program
$(PROG): $(OBJS)
	$(CXX) $(OBJS) -o $(PROG) $(LDFLAGS)

# cleaning everything that can be automatically recreated with "make"
clean:
	rm $(PROG) *.o
