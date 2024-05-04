
.PHONY: all frontend optimization clean test valgrind

all: frontend optimization

frontend:
	$(MAKE) -C frontend

optimization:
	$(MAKE) -C optimization

test: test_frontend test_optimization

test_frontend:
	$(MAKE) -C frontend smta_good
	$(MAKE) -C frontend smta_bad

test_optimization:
	$(MAKE) -C optimization test

valgrind: valgrind_frontend valgrind_optimization

valgrind_frontend:
	$(MAKE) -C frontend smta_good
	$(MAKE) -C frontend smta_bad

valgrind_optimization:
	$(MAKE) -C optimization valgrind

clean:
	$(MAKE) -C frontend clean
	$(MAKE) -C optimization clean
