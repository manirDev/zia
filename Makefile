# As long as project compiles in under a few seconds no need to get fancy and just build everything

## Path to source folders
SRCPATH=src/
SRCWEBPATH=web/

## List all C files (.c) that our project includes
SRCFILES= $(SRCPATH)zia.c

## List all C files included in our WebAssembly build
WEBFILES= $(SRCWEBPATH)webMain.c \
$(SRCPATH)zia.c

## Name of our executable we build to run
BINARY=zia.out

## WebAssembly output 
WASMJS=$(SRCWEBPATH)zia.js

# Build the native binary
build:
	gcc -o $(BINARY) $(SRCFILES) -Wall -O2

# Run in interactive mode (if implemented)
run: build
	./$(BINARY)

# Run with a specific .zia file
start: build
	./$(BINARY) start.zia

# Build the WebAssembly version
web:
	emcc -O3 $(WEBFILES) -o $(WASMJS) --shell-file $(SRCWEBPATH)index.html \
	-s WASM=1 \
	-s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
	-s EXPORTED_FUNCTIONS='["_main", "_web_tokenize", "_web_interpret", "_init", "_cleanup"]' \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s ASSERTIONS=1 \
	-s NO_EXIT_RUNTIME=1 \
	-s ENVIRONMENT='web'

# Install Monaco Editor dependencies
deps:
	mkdir -p build_wasm
	cd build_wasm && npm init -y && npm install monaco-editor

# Set up the web version completely
websetup: deps web
	cp $(SRCWEBPATH)/*.js build_wasm/
	cp $(SRCWEBPATH)/*.css build_wasm/

# Start a web server for development
serve:
	python3 -m http.server

# Clean all build artifacts
clean:
	rm -f $(BINARY) *.o
	rm -f $(SRCWEBPATH)/*.wasm
	rm -f $(SRCWEBPATH)/*.js
	rm -f $(SRCWEBPATH)/*.js.map
	rm -f $(SRCWEBPATH)/*.wasm.map

# Help target
help:
	@echo "ZIA Interpreter Makefile"
	@echo "-------------------------"
	@echo "Available targets:"
	@echo "  build:    Build native executable"
	@echo "  run:      Run the interpreter in interactive mode"
	@echo "  start:    Run the interpreter with start.zia file"
	@echo "  web:      Build the WebAssembly version"
	@echo "  deps:     Install dependencies (Monaco Editor)"
	@echo "  websetup: Complete setup for web version"
	@echo "  serve:    Start a development web server"
	@echo "  clean:    Remove build artifacts"
	@echo "  help:     Show this help message"

.PHONY: build run start web deps websetup serve clean help