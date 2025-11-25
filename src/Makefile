CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra
C_FILES = $(wildcard core/*.c) $(wildcard auxiliary/*.c)

SOURCES = $(C_FILES)
OBJECTS = $(SOURCES:.c=.o)
LIBRARY = s21_decimal.a
HEADERS = $(wildcard *.h) $(wildcard auxiliary/*.h)

TEST_DIR = ../test
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c) $(wildcard $(TEST_DIR)/suites/*.c)
TEST_EXECUTABLE = $(TEST_DIR)/unit-tests

ARCHIVE = ar
RC = ranlib

CLANG_FORMAT = clang-format
CLANG_FORMAT_STYLE = ../materials/linters/.clang-format
CPPCHECK_FLAGS = --enable=all \
	--suppress=missingIncludeSystem \
	--suppress=unusedStructMember \
	--suppress=unusedFunction \
	--suppress=checkersReport \
	--suppress=normalCheckLevelMaxBranches \
	--suppress=unmatchedSuppression \
	--suppress=unreadVariable

CHECK_CFLAGS = $(shell pkg-config --cflags check 2>/dev/null || echo "")
CHECK_LIBS = $(shell pkg-config --libs check 2>/dev/null || echo "")
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

all: s21_decimal.a

s21_decimal.a: $(OBJECTS)
	$(ARCHIVE) rcs $(LIBRARY) $^
	$(RC) $(LIBRARY)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

cppcheck:
	cppcheck $(SOURCES) $(HEADERS) $(CPPCHECK_FLAGS)
	cd $(TEST_DIR) && cppcheck $(TEST_SOURCES) $(CPPCHECK_FLAGS)

style:
	$(CLANG_FORMAT) -n -style=file:$(CLANG_FORMAT_STYLE) $(SOURCES) $(HEADERS)
	cd $(TEST_DIR) && $(CLANG_FORMAT) -n -style=file:$(CLANG_FORMAT_STYLE) $(TEST_SOURCES)

format:
	$(CLANG_FORMAT) -i -style=file:$(CLANG_FORMAT_STYLE) $(SOURCES) $(HEADERS)
	cd $(TEST_DIR) && $(CLANG_FORMAT) -i -style=file:$(CLANG_FORMAT_STYLE) $(TEST_SOURCES)

check: style cppcheck

clean:
	rm -f $(OBJECTS) $(LIBRARY)
	cd $(TEST_DIR) && make clean

build: check s21_decimal.a

test: s21_decimal.a
	cd $(TEST_DIR) && make unit-test

gcov_report: s21_decimal.a
	cd $(TEST_DIR) && make gcov_report

asan: s21_decimal.a
	cd $(TEST_DIR) && make asan

.PHONY: all s21_decimal.a cppcheck style format check clean build test gcov_report asan