OBJS = image.o qr.o
MAIN_OBJS = $(OBJS) main.o
TEST_OBJS = $(OBJS) test_main.o test_image.o test_qr.o
CPPFLAGS = -std=c++11
CC = g++
DEBUG = -g
CFLAGS = -c $(DEBUG)
NAME = HackQR
TEST_NAME = unitTester
BIN_DIR = bin/

$(BIN_DIR)$(NAME): $(addprefix $(BIN_DIR), $(MAIN_OBJS))
	g++ $(LFLAGS) $(CPPFLAGS) -o $@ $^

$(BIN_DIR) :
	mkdir $@

bin/main.o : source/main.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -Isrc/

bin/image.o  : $(addprefix source/, image.cpp image.h) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -Isrc/

bin/qr.o  : $(addprefix source/, qr.cpp qr.h) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -Isrc/

unit-test: $(BIN_DIR)$(TEST_NAME)

$(BIN_DIR)$(TEST_NAME): $(addprefix $(BIN_DIR), $(TEST_OBJS))
	g++ $(LFLAGS) $(CPPFLAGS) -o $@ $^  

bin/test_main.o : test/main.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -Isrc/

bin/test_image.o : test/image.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -Isrc/

bin/test_qr.o : test/qr.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -Isrc/

clean:
	rm -rf bin
