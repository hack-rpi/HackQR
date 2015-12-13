OBJS = main.o
CPPFLAGS = -std=c++11
CC = g++
DEBUG = -g
CFLAGS = -c $(DEBUG)
NAME = HackQR
CPPFILES = source/main.cpp
BIN_DIR = bin/

$(BIN_DIR)$(NAME): $(addprefix $(BIN_DIR), $(OBJS))
	g++ $(LFLAGS) $(CPPFLAGS) -o $@ $^

$(BIN_DIR) :
	mkdir $@

bin/main.o : source/main.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -Isrc/

clean:
	rm -rf bin
