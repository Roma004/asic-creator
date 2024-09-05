BUILD_DIR = ./build

EXEC_NAME = $(BUILD_DIR)/lab1

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: cmake lab-build lab-run lab clean-build clean-all doc

cmake:
	(pushd $(BUILD_DIR) && cmake .. -DTESTING=ON -DDOXYGEN=ON && popd)

lab-build:
	$(MAKE) -C $(BUILD_DIR)

lab-run:
	$(VALGRIND) $(EXEC_NAME)

lab: cmake lab-build lab-run

doc:
	$(MAKE) -C $(BUILD_DIR) doc_doxygen

test: cmake
	$(MAKE) -C $(BUILD_DIR) all_tests
	./build/all_tests
	gcovr -e main.cpp -e lib/catch2 -e tests/

clean-build:
	$(MAKE) -C $(BUILD_DIR) clean

clean-all:
	rm -rf $(BUILD_DIR)/*
