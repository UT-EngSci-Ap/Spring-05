CXX       := clang++
# Added -I$(INCDIR) so the compiler knows where to find your headers
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -I$(INCDIR)
TARGET    := music_player

# Directory definitions
SRCDIR    := src
INCDIR    := include

SRCS      := $(wildcard $(SRCDIR)/*.cpp)
OBJS      := $(SRCS:.cpp=.o)

# miniaudio backend frameworks/libraries.
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    LDFLAGS := -framework CoreFoundation -framework CoreAudio \
               -framework AudioToolbox -lpthread -lm
else
    LDFLAGS := -lpthread -lm -ldl
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean