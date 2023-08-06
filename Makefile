CXX = g++
LD = g++
CXXFLAGS = -std=c++17 -Wall -pedantic
LIBS = 
NAME = EXE

SRC = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRC))


all: compile run

compile: $(OBJS)
	@mkdir -p build
	$(LD) $(CXXFLAGS) -o $(NAME) $(OBJS) $(LIBS)

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<


run: compile
	./$(NAME)

clean:
	rm -rf build/ 2>/dev/null
	rm $(NAME) 2>/dev/null


fsanitize: $(OBJS)
	@mkdir -p build
	$(LD) $(CXXFLAGS) -fsanitize=address -o $(NAME) $(OBJS) $(LIBS)
	./$(NAME)

valgrind: $(OBJS)
	@mkdir -p build
	$(LD) $(CXXFLAGS) -g -o $(NAME) $(OBJS) $(LIBS)
	valgrind --leak-check=full ./$(NAME)