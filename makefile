CXX = g++
CXX_MINGW = x86_64-w64-mingw32-c++

CXXFLAGS = -Wall -O2
LDFLAGS_MINGW = -static-libgcc -static-libstdc++ -static


SRCS := $(shell find . -type f -name 'main.cpp')

LINUX_BINS := $(SRCS:%.cpp=%-linux)
WIN_BINS   := $(SRCS:%.cpp=%.exe)

all: $(LINUX_BINS) $(WIN_BINS)


%-linux: %.cpp
	@echo "Compiling $< to $@"
	$(CXX) $(CXXFLAGS) $< -o $@

%.exe: %.cpp
	@echo "Compiling $< to $@ (MinGW)"
	$(CXX_MINGW) $(CXXFLAGS) $< -o $@ $(LDFLAGS_MINGW)

clean:
	@echo "Cleaning up..."
	rm -f $(LINUX_BINS) $(WIN_BINS)
