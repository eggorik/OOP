CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -I.

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = snake_game

LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./snake_game

clean:
	rm -f $(OBJS) $(EXEC)
