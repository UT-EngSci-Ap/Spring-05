# ─── Compiler & flags ────────────────────────────────────────────────────────
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic

# Debug build: make DEBUG=1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g -O0 -DDEBUG
else
    CXXFLAGS += -O2
endif

# ─── Output binary ───────────────────────────────────────────────────────────
TARGET  := installer

# ─── Sources & objects ───────────────────────────────────────────────────────
SRCS    := main.cpp \
           ComponentState.cpp \
           Helpers.cpp \
           Installable.cpp \
           SystemLogger.cpp \
           Module.cpp \
           Package.cpp \
           InstallationEngine.cpp

OBJS    := $(SRCS:.cpp=.o)

# ─── Default target ──────────────────────────────────────────────────────────
.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# ─── Pattern rule: .cpp → .o ─────────────────────────────────────────────────
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# ─── Explicit header dependencies ────────────────────────────────────────────
main.o:                InstallationEngine.h Helpers.h
ComponentState.o:      ComponentState.h
Helpers.o:             Helpers.h
Installable.o:         Installable.h ComponentState.h Observer.h TransactionContext.h
SystemLogger.o:        SystemLogger.h Observer.h Installable.h
Module.o:              Module.h Installable.h
Package.o:             Package.h Installable.h
InstallationEngine.o:  InstallationEngine.h Module.h Package.h Helpers.h \
                       TransactionContext.h SystemLogger.h

# ─── Utility targets ─────────────────────────────────────────────────────────
.PHONY: clean run

clean:
	$(RM) $(OBJS) $(TARGET)

run: all
	./$(TARGET)
