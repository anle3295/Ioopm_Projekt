#include "stack_traverser.h"
#include <string.h>

#define Dump_registers()			\
  jmp_buf env;					\
  if (setjmp(env)) abort();			\

extern void **environ;

void *get_stack_top() {
  int top_of_stack;
  return &top_of_stack;
}

ll_head get_alive_stack_pointers(heap_t *h) {
  return get_alive_stack_pointers_in_range(h, get_stack_top(), environ);
}

ll_head get_alive_stack_address_pointers(heap_t *h) {
  return get_alive_stack_address_pointers_in_range(h, get_stack_top(), environ);
}

ll_head get_alive_stack_pointers_in_range(heap_t *h, void *top, void *bottom) {

  Dump_registers();
  ll_node **root = LL_initRoot();

  if (top > bottom) {
    while (top > bottom) {
      void **content = top;
      if (validate_object(*content, h)) { // checks the pointers metadata to check whether it's pointing at an object or not
        LL_createAndInsertSequentially(root, *content);
      }
      --top;
    }
  } else {
    while (top < bottom) {
      void **content = top;
      if (validate_object(*content, h)) { // checks the pointers metadata to check whether it's pointing at an object or not
        LL_createAndInsertSequentially(root, *content);
      }
      ++top;
    }
  }

  return root;
}

ll_head get_alive_stack_address_pointers_in_range(heap_t *h, void *top, void *bottom) {

  Dump_registers();
  ll_node **root = LL_initRoot();

  if (top > bottom) {
    while (top > bottom) {
      void **content = top;
      if (validate_object(*content, h)) { // checks the pointers metadata to check whether it's pointing at an object or not
        LL_createAndInsertSequentially(root, content);
      }
      --top;
    }
  } else {
    while (top < bottom) {
      void **content = top;
      if (validate_object(*content, h)) { // checks the pointers metadata to check whether it's pointing at an object or not
        LL_createAndInsertSequentially(root, content);
      }
      ++top;
    }
  }

  return root;
}
