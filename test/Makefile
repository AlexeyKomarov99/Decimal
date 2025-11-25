CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra
EXECUTABLE = unit-tests

SRC_DIR = ../src
SOURCES = $(wildcard $(SRC_DIR)/core/*.c) $(wildcard $(SRC_DIR)/auxiliary/*.c)
HEADERS = $(SRC_DIR)/s21_decimal.h $(SRC_DIR)/auxiliary/s21_auxiliary.h

TEST_MAIN = unit_tests_main.c
TEST_FILES = $(wildcard test_s21_*.c)

CHECK_CFLAGS = $(shell pkg-config --cflags check)
CHECK_LIBS = $(shell pkg-config --libs check)
INCLUDES = -I$(SRC_DIR)
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    TEST_CFLAGS = $(CFLAGS) -Wno-stringop-overflow -Wno-stringop-truncation
else
    TEST_CFLAGS = $(CFLAGS)
endif

all: unit-test

unit-test: $(EXECUTABLE)
	./$(EXECUTABLE)

$(EXECUTABLE): $(TEST_MAIN) $(TEST_FILES) $(SOURCES) $(HEADERS)
	$(CC) $(TEST_CFLAGS) $(INCLUDES) $(CHECK_CFLAGS) -o $(EXECUTABLE) \
	$(TEST_MAIN) $(TEST_FILES) $(SOURCES) $(CHECK_LIBS)

gcov_report: $(TEST_MAIN) $(TEST_FILES) $(SOURCES) $(HEADERS)
	$(CC) $(TEST_CFLAGS) -g $(INCLUDES) $(CHECK_CFLAGS) $(GCOV_FLAGS) -o $(EXECUTABLE) \
	$(TEST_MAIN) $(TEST_FILES) $(SOURCES) $(CHECK_LIBS)
	./$(EXECUTABLE) || true
	cp *.gcda *.gcno ../src/ 2>/dev/null || true
	cp *.gcda *.gcno ../src/core/ 2>/dev/null || true
	cp *.gcda *.gcno ../src/auxiliary/ 2>/dev/null || true
	gcovr --html --html-details -o coverage.html --root=../src --gcov-ignore-errors=all --print-summary \
	--exclude=".*/auxiliary/.*" 2>/dev/null
	open coverage.html 2>/dev/null || true

asan: $(TEST_MAIN) $(TEST_FILES) $(SOURCES) $(HEADERS)
	$(CC) $(TEST_CFLAGS) -g -fsanitize=address $(INCLUDES) $(CHECK_CFLAGS) -o $(EXECUTABLE) \
	$(TEST_MAIN) $(TEST_FILES) $(SOURCES) $(CHECK_LIBS)
	./$(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE) *.gcno *.gcda *.gcov *.info *.html *.css report *.dSYM
	rm -f $(SRC_DIR)/core/*.o $(SRC_DIR)/auxiliary/*.o $(SRC_DIR)/s21_decimal.a
	find $(SRC_DIR) -name "*.gcno" -delete
	find $(SRC_DIR) -name "*.gcda" -delete
	find . -name "*.gcno" -delete
	find . -name "*.gcda" -delete

.PHONY: all unit-test gcov_report clean asan