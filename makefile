CC	= g++
STD	= c++17
FLAGS = -Wall -std=$(STD)

LIB = ilog
LIB_SRC = ilog/ilog.cpp
DEMO = main
DEMO_SRC = main.cpp mthreadlogger.cpp

all: lib demo

lib:
	$(CC) $(FLAGS) -fPIC -shared -o lib$(LIB).so $(LIB_SRC)

demo:
	$(CC) $(FLAGS) $(DEMO_SRC) -o $(DEMO) -L. -l$(LIB)

clean:
	rm *.o
