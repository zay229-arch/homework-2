\# ============================================================
#  Makefile for HashSet project
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -fPIC -Iinclude

# Directories
BUILD_DIR     := build
OBJ_DIR       := $(BUILD_DIR)/objects
LIB_REL_DIR   := $(BUILD_DIR)/lib/release
LIB_DBG_DIR   := $(BUILD_DIR)/lib/debug
BIN_DIR       := $(BUILD_DIR)/bin

# Sources & objects
LIB_SRCS := src/lib.cpp src/linkedlist.cpp
LIB_OBJS := $(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(LIB_SRCS))
DBG_OBJS := $(patsubst src/%.cpp, $(OBJ_DIR)/%-dbg.o, $(LIB_SRCS))

STATIC_LIB  := $(LIB_REL_DIR)/libhashset.a
SHARED_REL  := $(LIB_REL_DIR)/libhashset.so
SHARED_DBG  := $(LIB_DBG_DIR)/libhashset.so

TEST_SRC := tests/benchmarker.cpp
TEST_OBJ := $(OBJ_DIR)/benchmarker.o
TEST_BIN := $(BIN_DIR)/test_runner

MAIN_SRC := src/bin/main.cpp
MAIN_OBJ := $(OBJ_DIR)/main.o
MAIN_BIN := $(BIN_DIR)/hashset

# ============================================================
.DEFAULT_GOAL := all
.PHONY: all static shared debug clean install test

all: static shared

# ── Static library ─────────────────────────────────────────
static: $(STATIC_LIB)

$(STATIC_LIB): $(LIB_OBJS)
	mkdir -p $(LIB_REL_DIR)
	ar rcs $@ $^

# ── Shared library (release) ───────────────────────────────
shared: $(SHARED_REL)

$(SHARED_REL): $(LIB_OBJS)
	mkdir -p $(LIB_REL_DIR)
	$(CXX) -shared -O2 -o $@ $^

# ── Shared library (debug) ────────────────────────────────
debug: $(SHARED_DBG)

$(SHARED_DBG): $(DBG_OBJS)
	mkdir -p $(LIB_DBG_DIR)
	$(CXX) -shared -g -O0 -o $@ $^

# ── Compile library object files (release) ────────────────
$(OBJ_DIR)/%.o: src/%.cpp include/hashset.hpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -O2 -c $< -o $@

# ── Compile library object files (debug) ──────────────────
$(OBJ_DIR)/%-dbg.o: src/%.cpp include/hashset.hpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -g -O0 -c $< -o $@

# ── Main executable ───────────────────────────────────────
$(MAIN_BIN): $(MAIN_OBJ) $(STATIC_LIB)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(MAIN_OBJ): $(MAIN_SRC)
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ── Tests ─────────────────────────────────────────────────
test: static $(TEST_BIN)
	$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJ) $(STATIC_LIB)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_OBJ): $(TEST_SRC)
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ── Install ───────────────────────────────────────────────
install: shared
	cp $(SHARED_REL) /usr/local/lib/
	ldconfig || true

# ── Clean ─────────────────────────────────────────────────
clean:
	rm -rf $(BUILD_DIR)