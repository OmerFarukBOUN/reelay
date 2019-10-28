CXX=g++ # C compiler
CXXFLAGS=-std=c++17 -fPIC -O2 -g -pthread -fno-new-ttp-matching # -Wall -Wextra  C flags
LDFLAGS=-shared  # linking flags

LIB_FLAGS=-lreelay -lantlr4-runtime
INCLUDE_FLAGS=-I. -I./include -I/usr/local/include/antlr4-runtime

NAME = reelay
VERSION = 1.2
TARGET = libreelay.so # target lib

PROJECT_INCLUDE = include/reelay

PREFIX = /usr/local
INCDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib
WORKDIR = ./build

ANTLR4_EXE = build/antlr-4.7.2-complete.jar
ANTLR4_FLAGS = -Dlanguage=Cpp -visitor -Xexact-output-dir
ANTLR4_GRAMMAR_DIR = grammar/antlr4

ANTLR4_TL_GRAMMAR = TemporalLogic
ANTLR4_TL_SOURCES = grammar/antlr4/temporal_logic
ANTLR4_TL_HEADERS = include/reelay/parser/antlr4/temporal_logic

SOURCES = $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)Lexer.cpp $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)Parser.cpp $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)Listener.cpp $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)BaseListener.cpp $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)Visitor.cpp $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)BaseVisitor.cpp
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean develop test install uninstall apps

all: antlr4-parser $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJECTS): $(ANTLR4_TL_SOURCES)/%.o : $(ANTLR4_TL_SOURCES)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

clean:
	rm -f $(ANTLR4_TL_SOURCES)/*.o
	rm -f $(TARGET)

install:
	mkdir -p $(DESTDIR)$(INCDIR)/$(NAME)

	cp -a $(PROJECT_INCLUDE) $(DESTDIR)$(INCDIR)
	cp -p $(TARGET) $(DESTDIR)$(LIBDIR)/$(TARGET)

uninstall:
	rm -rf $(DESTDIR)$(INCDIR)/$(NAME) 
	rm -rf $(DESTDIR)$(LIBDIR)/$(TARGET)

develop:
	ln -s $(PROJECT_INCLUDE) $(DESTDIR)$(INCDIR)
	ln -s $(TARGET) $(DESTDIR)$(LIBDIR)/$(TARGET)

antlr4-parser:
	mkdir -p build
	cd build && curl -O https://www.antlr.org/download/antlr-4.7.2-complete.jar
	mkdir -p $(ANTLR4_TL_SOURCES)	
	java -jar $(ANTLR4_EXE) $(ANTLR4_FLAGS) -o $(ANTLR4_TL_SOURCES) $(ANTLR4_GRAMMAR_DIR)/$(ANTLR4_TL_GRAMMAR).g4 
	mkdir -p $(ANTLR4_TL_HEADERS)
	cp -a $(ANTLR4_TL_SOURCES)/*.h $(ANTLR4_TL_HEADERS)

antlr4-parser-clean:
	rm -rf $(ANTLR4_TL_SOURCES)
	rm -rf $(ANTLR4_TL_HEADERS)

antlr4-runtime:
	mkdir -p build
	cd build && curl -O https://www.antlr.org/download/antlr-4.7.2-complete.jar
	cd build && rm -rf antlr4 && git clone https://github.com/antlr/antlr4.git
	cd build/antlr4/runtime/Cpp && cmake . -DANTLR_JAR_LOCATION=../../../antlr-4.7.2-complete.jar
	cd build/antlr4/runtime/Cpp && make

antlr4-runtime-install:
	cd build/antlr4/runtime/Cpp && make install

antlr4-runtime-uninstall:
	rm -rf /usr/local/include/antlr4-runtime
	rm -rf /usr/local/share/doc/libantlr4
	rm -r /usr/local/lib/libantlr4-runtime.*

antlr4-runtime-clean:
	rm -rf build/antlr4

timescales: 
	mkdir -p build
	cd build && rm -rf timescales && git clone https://github.com/doganulus/timescales.git
	cd build/timescales && make full 

timescales-clean:
	rm -rf build/timescales

apps:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) apps/mtl/rymtl.cpp -o bin/rymtl $(INCLUDE_FLAGS) $(LIB_FLAGS)

test_csvparser:
	mkdir -p bin/csvparser
	$(CXX) $(CXXFLAGS) apps/csvparser/basic.cpp -o bin/csvparser/csvparser_basic $(INCLUDE_FLAGS)
	$(CXX) $(CXXFLAGS) apps/csvparser/fast.cpp -o bin/csvparser/csvparser_fast $(INCLUDE_FLAGS)
	$(CXX) $(CXXFLAGS) apps/csvparser/modern.cpp -o bin/csvparser/csvparser_modern $(INCLUDE_FLAGS)
	multitime -n 10 bin/csvparser/csvparser_basic build/timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR1000.csv
	multitime -n 10 bin/csvparser/csvparser_fast build/timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR1000.csv
	multitime -n 10 bin/csvparser/csvparser_modern build/timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR1000.csv 

test: test_main test_untimed test_discrete_timed test_dense_timed test_untimed_robustness test_discrete_timed_robustness apps

test_main:
	mkdir -p build/test
	$(CXX) $(CXXFLAGS) -c test/test_main.cpp -o build/test/main.o $(INCLUDE_FLAGS) $(LIB_FLAGS)

test_untimed:
	$(CXX) $(CXXFLAGS) build/test/main.o test/test_setting_untimed.cpp -o build/test/test_setting_untimed $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./build/test/test_setting_untimed -r compact

test_untimed_robustness:
	$(CXX) $(CXXFLAGS) build/test/main.o test/test_setting_untimed_robustness.cpp -o build/test/test_setting_untimed_robustness $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./build/test/test_setting_untimed_robustness -r compact

test_discrete_timed:
	$(CXX) $(CXXFLAGS) build/test/main.o test/test_setting_discrete_timed.cpp -o build/test/test_setting_discrete_timed $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./build/test/test_setting_discrete_timed -r compact

test_discrete_timed_robustness:
	$(CXX) $(CXXFLAGS) build/test/main.o test/test_setting_discrete_timed_robustness.cpp -o build/test/test_setting_discrete_timed_robustness $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./build/test/test_setting_discrete_timed_robustness -r compact

test_dense_timed:
	$(CXX) $(CXXFLAGS) build/test/main.o test/test_setting_dense_timed.cpp -o build/test/test_setting_dense_timed $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./build/test/test_setting_dense_timed -r compact

test_mtl_performance_discrete: test/timescales/discrete/multitime/*.txt
	for batchfile in $^ ; do \
        multitime -n 10 -b $${batchfile} ; \
    done