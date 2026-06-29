# Terminal Music Player — build file
#
# Run the resulting binary FROM THE PROJECT ROOT so the fixed data paths
# (Data/library.csv, Data/Playlists/, Data/settings.cfg) resolve correctly:
#
#     make
#     ./music_player

CXX       := clang++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2
TARGET    := music_player
SRCDIR    := src
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
