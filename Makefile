CXX = clang++
CXXFLAGS = -I include/
SRCS = $(wildcard src/*.cpp) main.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = main

# Verifica o sistema operacional
ifeq ($(OS),Windows_NT)
	CLEAN_CMD = if exist src\*.o (del /f /q src\*.o) && if exist main.o (del /f /q main.o) && if exist $(EXEC) (del /f /q $(EXEC))
else
	CLEAN_CMD = rm -f src/*.o main.o $(EXEC)
endif

all: $(EXEC) clean

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@$(CLEAN_CMD)
