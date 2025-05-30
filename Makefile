# PolyBuild Makefile
# Wrapper around CMake for simplified build process

BUILD_DIR := build
BIN_DIR := bin
CMAKE_BUILD_TYPE ?= Release

.PHONY: all clean configure build install test help

# Default target
all: build

# Configure the build system
configure:
	@echo "Configuring PolyBuild build system..."
	@cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) .

# Build the project
build: configure
	@echo "Building PolyBuild..."
	@cmake --build $(BUILD_DIR) --config $(CMAKE_BUILD_TYPE)
	@echo "Build complete. Executable: $(BUILD_DIR)/bin/polybuild.exe"

# Install the built executable
install: build
	@echo "Installing PolyBuild..."
	@cmake --install $(BUILD_DIR)

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

# Run basic tests
test: build
	@echo "Running PolyBuild tests..."
	@$(BUILD_DIR)/bin/polybuild.exe --help

# Development build with debug symbols
debug:
	@$(MAKE) CMAKE_BUILD_TYPE=Debug build

# Release build with optimizations
release:
	@$(MAKE) CMAKE_BUILD_TYPE=Release build

# Display help information
help:
	@echo "PolyBuild Makefile Targets:"
	@echo "  all      - Build the project (default)"
	@echo "  build    - Build the project"
	@echo "  clean    - Clean build artifacts"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build with optimizations"
	@echo "  install  - Install the built executable"
	@echo "  test     - Run basic functionality tests"
	@echo "  help     - Display this help message"
	@echo ""
	@echo "Environment Variables:"
	@echo "  CMAKE_BUILD_TYPE - Build type (Debug/Release, default: Release)"
