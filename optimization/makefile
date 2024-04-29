LLVMCODE = optimization
TEST = p3_const_prop
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

$(LLVMCODE): $(LLVMCODE).c
	g++ -g -I /usr/include/llvm-c-15/ -c $(LLVMCODE).c
	g++ $(LLVMCODE).o `llvm-config-15 --cxxflags --ldflags --libs core` -I /usr/include/llvm-c-15/ -o $@

llvm_file: ./opt_tests/$(TEST).c
	clang -S -emit-llvm ./opt_tests/$(TEST).c -o ./opt_tests/$(TEST).ll

valgrind: $(LLVMCODE)
	$(VALGRIND) ./$(LLVMCODE) ./opt_tests/p3_const_prop.ll

test: $(LLVMCODE)
	./$(LLVMCODE) ./opt_tests/p4_const_prop.ll

clean: 
	rm -rf $(TEST).ll
	rm -rf $(LLVMCODE)
	rm -rf *.o
	rm -rf *.out
