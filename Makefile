OBJS = main.o image.o qr.o
CPPFLAGS = -std=c++11
CC = g++
DEBUG = -g
CFLAGS = -c $(DEBUG)
NAME = HackQR
CPPFILES = source/main.cpp source/image.cpp qr.cpp
BIN_DIR = bin/

$(BIN_DIR)$(NAME): $(addprefix $(BIN_DIR), $(OBJS))
	g++ $(LFLAGS) $(CPPFLAGS) -o $@ $^

$(BIN_DIR) :
	mkdir $@

bin/main.o : source/main.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -Isrc/

bin/image.o  : $(addprefix source/, image.cpp image.h) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -Isrc/

bin/qr.o  : $(addprefix source/, qr.cpp qr.h) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -Isrc/

clean:
	rm -rf bin
