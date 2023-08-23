CC = g++
CFLAGS = -std=c++17 -Wall -Werror -Wextra
GCOVFLAGS = --coverage
GTESTFLAGS = -lgtest #-fsanitize=address
VALGRINDFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

OPEN = open

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	GTESTFLAGS += -lpthread
	OPEN = xdg-open
endif

PROJECT_NAME = Calculator
APP_NAME = $(PROJECT_NAME)
ifeq ($(OS), Darwin)
	APP_NAME = $(PROJECT_NAME).app
endif

QMAKE = qmake
# QMAKE = $(shell find ~ /usr -type f -name "qmake" -perm /a=x | sort | head -1)
QT_INSTALL_PREFIX = $(shell $(QMAKE) -query QT_INSTALL_PREFIX)

SRC_DIR = .
MODEL_DIR = ./model
CONTROLLER_DIR = ./controller
VIEW_DIR = ./view
COMMON_DIR = ./common
TESTS_DIR = ./tests
REPORT_DIR = ./report
BUILD_DIR = ../build
APP_DIR = ../app
DIST_DIR = ../dist

RUN = $(APP_DIR)/$(APP_NAME)
ifeq ($(OS), Darwin)
	RUN = open $(APP_DIR)/$(APP_NAME)
endif

MODEL_SRC = $(wildcard $(MODEL_DIR)/*.cc)
MODEL_H = $(wildcard $(MODEL_DIR)/*.h)
TESTS_SRC = $(TESTS_DIR)/tests_main.cc
TESTS = $(TESTS_DIR)/tests

default: all

all: build

build:
	mkdir -p $(BUILD_DIR)
	# cmake CMakeLists.txt -B $(BUILD_DIR)
	cmake -DCMAKE_PREFIX_PATH=$(QT_INSTALL_PREFIX) CMakeLists.txt -B $(BUILD_DIR)
	make -C $(BUILD_DIR)

install: build
	mkdir -p $(APP_DIR)
	cp -r $(BUILD_DIR)/$(APP_NAME) $(APP_DIR)/$(APP_NAME)

uninstall:
	rm -rf $(APP_DIR)

rebuild: clean all

run:
	$(RUN)

dvi:
	mkdir -p $(APP_DIR)
	texi2dvi $(SRC_DIR)/manual.tex -c -o $(APP_DIR)/manual.dvi

dist: clean
	mkdir -p $(DIST_DIR)
	tar -zcvf $(DIST_DIR)/SmartCalc_v2.tar.gz $(SRC_DIR)/*


style:
	clang-format --verbose -n --style=google $(SRC_DIR)/*.cc
	clang-format --verbose -n --style=google $(MODEL_DIR)/*.cc
	clang-format --verbose -n --style=google $(MODEL_DIR)/*.h
	clang-format --verbose -n --style=google $(CONTROLLER_DIR)/*.cc
	clang-format --verbose -n --style=google $(CONTROLLER_DIR)/*.h
	clang-format --verbose -n --style=google $(VIEW_DIR)/*.cc
	clang-format --verbose -n --style=google $(VIEW_DIR)/*.h
	clang-format --verbose -n --style=google $(COMMON_DIR)/*.h

cppcheck:
	cppcheck $(SRC_DIR) -iview/thirdparty -x c++ --std=c++17 --enable=all --suppress=missingInclude --suppress=unusedFunction

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(TESTS_DIR)/*calc*
	rm -rf $(TESTS_DIR)/*.h
	rm -rf $(TESTS) $(TESTS_DIR)/test_cov
	rm -rf $(SRC_DIR)/*.gcno $(SRC_DIR)/*.info $(SRC_DIR)/*.gcda $(SRC_DIR)/*.o
	rm -rf $(TESTS_DIR)/*.txt
	rm -rf $(REPORT_DIR)

tests:
	cp $(MODEL_DIR)/* $(TESTS_DIR)
	cp $(COMMON_DIR)/* $(TESTS_DIR)
	$(CC) $(CFLAGS) $(TESTS_DIR)/*.cc $(GTESTFLAGS) -o $(TESTS) 
	$(TESTS)

valgrind: tests
	valgrind $(VALGRINDFLAGS) --log-file=$(TESTS_DIR)/valgrind_report.txt $(TESTS)

leaks: tests
	leaks -atExit -- $(TESTS) > $(TESTS_DIR)/leaks_report.txt

gcov_report:
	mkdir -p $(REPORT_DIR)
	cp $(MODEL_DIR)/* $(TESTS_DIR)
	cp $(COMMON_DIR)/* $(TESTS_DIR)
	$(CC) $(CFLAGS) $(TESTS_DIR)/*.cc $(GCOVFLAGS) $(GTESTFLAGS) -o $(TESTS_DIR)/test_cov
	$(TESTS_DIR)/test_cov
	rm -rf calccore.* tests_main.*
	rm -rf $(TESTS_DIR)/*main.gcda $(TESTS_DIR)/*main.gcno
	# lcov -t "test_$(CORE_NAME)" -o $(TESTS_DIR)/test_$(CORE_NAME).info -c -d .
	# genhtml -o $(REPORT_DIR) $(TESTS_DIR)/test_$(CORE_NAME).info
	gcovr -e $(TESTS_SRC) -r . --html --html-details -o $(REPORT_DIR)/index.html
	$(OPEN) $(REPORT_DIR)/index.html

.PHONY: all clean tests gcov_report default clang_format valgrind leaks build install uninstall rebuild run dvi diss