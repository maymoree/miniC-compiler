LLVMCODE = optimization
TEST = p3_const_prop

$(LLVMCODE): $(LLVMCODE).c
	g++ -g -I /usr/include/llvm-c-15/ -c $(LLVMCODE).c
	g++ $(LLVMCODE).o `llvm-config-15 --cxxflags --ldflags --libs core` -I /usr/include/llvm-c-15/ -o $@

llvm_file: ./opt_tests/$(TEST).c
	clang -S -emit-llvm ./opt_tests/$(TEST).c -o ./opt_tests/$(TEST).ll

clean: 
	rm -rf $(TEST).ll
	rm -rf $(LLVMCODE)
	rm -rf *.o
	rm -rf *.out
