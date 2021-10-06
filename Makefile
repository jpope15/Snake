CC = g++
C_FLAGS = -std=c++11 -g -c -Wall
OBJ = main.o food.o snake.o world.o
DEPS = world.h food.h snake.h constants.h helpers.h 
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
EXE = Snake

all: $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(LIBS)

%.o: %.cpp $(DEPS)
	$(CC) $(C_FLAGS) -o $@ $<

run:
	./$(EXE)

clean:
	rm $(OBJ) $(EXE)