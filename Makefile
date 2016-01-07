CC=gcc
FLAGS_PROD=-Wall -std=c11
FLAGS_DEBUG=$(FLAGS_PROD) -ggdb
FLAGS_CUNIT=$(FLAGS_DEBUG) -lcunit
FLAGS_GCOV=$(FLAGS_DEBUG) --coverage

FILES_GCOV=gc_test.c gc.c
FILES_MAIN=gc.o heap.o traverser.o utilities.o linked_list.o
#removed collector.o from above temporarily (duplicate functions otherwise)


DIR_RESOURCES=./resources/

all:
	@echo "Not implemented yet."

#compile object files
%.o: %.c
	$(CC) $(FLAGS_PROD) -o $@ -c $^

# compile object files with debugging information
%.debug.o: %.c
	$(CC) $(FLAGS_DEBUG) -o $@ -c $^

traverser.run: traverser.c heap.o linked_list.o
	$(CC) $(FLAGS_DEBUG) -o $@ $^
	./$@

#generate documentation with doxygen
doc: $(DIR_RESOURCES)gc.doxy
	@doxygen $^
.PHONY: doc

#check if there are memory leaks
valgrind: gc_test
	@valgrind --leak-check=full ./gc_test.out
.PHONY: valgrind

#check the unit-test coverage of every source file
gcov: gcov_clean $(FILES_GCOV)
	@$(CC) $(FLAGS_GCOV) -lcunit -o gcov.out $(FILES_GCOV) #compile source files with gcov data
	@./gcov.out >> /dev/null #create profile data, silence the output
	@gcov $(FILES_GCOV)
.PHONY: gcov

# this part is executed when testing on multiple machines. change dependency to your needs (ex: os_dump, valgrind, gcov)
# DEFAULT: run_test
test: stack_c stack_test_c stack_run stack_test_run
.PHONY: test

# Flymake mode (Live syntax and error check)
# Insert the following lines in your .emacs file:
#	(require 'flymake)
#	(add-hook 'find-file-hook 'flymake-find-file-hook)
# To put in practical use: M-x flymake-mode RET
check-syntax:
	gcc -o nul -S ${CHK_SOURCES}

# run tests
run_test: gc_test
	@./gc_test.out
.PHONY: run_test

os_dump:
	@echo "-s : $(shell uname -s)"; \
	echo "-m : $(shell uname -m)"; \
	echo "-o : $(shell uname -o)"; \
	echo "-r : $(shell uname -r)"; \
	echo "-p : $(shell uname -p)"; \
	echo "-v : $(shell uname -v)"; \

#compile test
gc_test: traverser.debug.o linked_list.debug.o heap.debug.o gc.debug.o gc_test.c
	$(CC) -o $@.out $^ $(FLAGS_CUNIT)
.PHONY: gc_test

linked_list.o: linked_list.c linked_list.h
	$(CC) $(FLAGS_PROD) linked_list.c -o linked_list.o -c

heap.o: heap.c heap.h
	$(CC) $(FLAGS_PROD) heap.c -o heap.o -c

stack_c: stack_traverser.o linked_list.o heap.o
	$(CC) $(FLAGS_DEBUG) -o stack_traverser stack_traverser.c

stack_run:
	@./stack_traverser

stack_test_c: stack_traverser_test.debug.o stack_traverser.debug.o linked_list.debug.o heap.debug.o
	$(CC) -o $@.out $^ $(FLAGS_CUNIT)

stack_test_run:
	@./stack_traverser_test.out

#test with gui
test_gui: $(FILES_MAIN) gui.c
	$(CC) $(FLAGS_PROD) -o $@.run $^

#remove crap files.
clean: gcov_clean
	@rm -rf *.out
	@rm -rf *.o
	@rm -rf ./doc/*
	@rm -rf ./*.dSYM
	@rm -rf .DS_Store
	@rm -rf *.zip
	@rm -rf *.result.txt
	@echo "All cleaned up!"

gcov_clean:
	@rm -rf *.gc*
	@rm -rf gcov.out
