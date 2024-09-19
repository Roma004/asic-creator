BUILD_DIR = ./build

EXEC_NAME = $(BUILD_DIR)/lab

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: cmake lab-build lab-run lab clean-build clean-all doc

cmake:
	@mkdir -p $(BUILD_DIR)
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
	gcovr -e main.cpp -e tests/ --exclude-unreachable-branches
	gcovr -e main.cpp -e tests/ --html-details build/coverage.html --exclude-unreachable-branches

clean-build:
	$(MAKE) -C $(BUILD_DIR) clean

clean-all:
	rm -rf $(BUILD_DIR)/*
