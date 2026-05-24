.PHONY: build clean run

build:

	@mkdir -p build
	@cd build && cmake .. && cmake --build .

clean: 
	@rm -rf build

run:
	@./build/CarloChem
