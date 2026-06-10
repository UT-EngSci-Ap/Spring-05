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

# ─── Source directory ────────────────────────────────────────────────────────
SRCDIR  := src

# ─── Sources & objects ───────────────────────────────────────────────────────
SRCS    := $(SRCDIR)/main.cpp \
           $(SRCDIR)/ComponentState.cpp \
           $(SRCDIR)/Helpers.cpp \
           $(SRCDIR)/Installable.cpp \
           $(SRCDIR)/SystemLogger.cpp \
           $(SRCDIR)/Module.cpp \
           $(SRCDIR)/Package.cpp \
           $(SRCDIR)/InstallationEngine.cpp

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
$(SRCDIR)/main.o:                $(SRCDIR)/InstallationEngine.h $(SRCDIR)/Helpers.h
$(SRCDIR)/ComponentState.o:      $(SRCDIR)/ComponentState.h
$(SRCDIR)/Helpers.o:             $(SRCDIR)/Helpers.h
$(SRCDIR)/Installable.o:         $(SRCDIR)/Installable.h $(SRCDIR)/ComponentState.h $(SRCDIR)/Observer.h $(SRCDIR)/TransactionContext.h
$(SRCDIR)/SystemLogger.o:        $(SRCDIR)/SystemLogger.h $(SRCDIR)/Observer.h $(SRCDIR)/Installable.h
$(SRCDIR)/Module.o:              $(SRCDIR)/Module.h $(SRCDIR)/Installable.h
$(SRCDIR)/Package.o:             $(SRCDIR)/Package.h $(SRCDIR)/Installable.h
$(SRCDIR)/InstallationEngine.o:  $(SRCDIR)/InstallationEngine.h $(SRCDIR)/Module.h $(SRCDIR)/Package.h $(SRCDIR)/Helpers.h \
                                 $(SRCDIR)/TransactionContext.h $(SRCDIR)/SystemLogger.h

# ─── Utility targets ─────────────────────────────────────────────────────────
.PHONY: clean run

clean:
	$(RM) $(OBJS) $(TARGET)

run: all
	./$(TARGET)
