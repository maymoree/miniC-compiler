LLVMCODE = optimization
TEST = cfold_add
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

$(LLVMCODE): $(LLVMCODE).c
	g++ -g -I /usr/include/llvm-c-15/ -c $(LLVMCODE).c
	g++ $(LLVMCODE).o `llvm-config-15 --cxxflags --ldflags --libs core` -I /usr/include/llvm-c-15/ -o $@

llvm_file: ./optimizer_test_results/$(TEST).c
	clang -S -emit-llvm ./optimizer_test_results/$(TEST).c -o ./optimizer_test_results/$(TEST).ll

valgrind: $(LLVMCODE)
	$(VALGRIND) ./$(LLVMCODE) ./optimizer_test_results/p4_const_prop.ll

test: $(LLVMCODE)
	./$(LLVMCODE) ./optimizer_test_results/p4_const_prop.ll

clean: 
	rm -rf $(TEST).ll
	rm -rf $(LLVMCODE)
	rm -rf *.o
	rm -rf *.out
