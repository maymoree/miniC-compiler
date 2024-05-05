LLVMCODE = optimization_test
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# Define source files
SRC = optimization_test.c optimization.c

# Define object files
OBJ = $(SRC:.c=.o)

# Compiler flags
CFLAGS = -g -I /usr/include/llvm-c-15/ -I ~/miniC_compiler/ast

$(LLVMCODE): $(OBJ)
	g++ $(CFLAGS) $(OBJ) `llvm-config-15 --cxxflags --ldflags --libs core` -o $@

%.o: %.c optimization.h
	g++ $(CFLAGS) -c $< -o $@

llvm_file: ./optimizer_test_results/$(TEST).c
	clang -S -emit-llvm ./optimizer_test_results/$(TEST).c -o ./optimizer_test_results/$(TEST).ll

valgrind: $(LLVMCODE)
	$(VALGRIND) ./$(LLVMCODE) ./optimizer_test_results/p4_const_prop.ll

test: $(LLVMCODE)
	./$(LLVMCODE) ./optimizer_test_results/p4_const_prop.ll

clean: 
	rm -rf $(TEST).ll
	rm -rf $(LLVMCODE)
	rm -rf $(OBJ)
	rm -rf *.out