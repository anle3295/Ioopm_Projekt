#include "heap.h"

// mallocates space for heap, places metadata in the front. 

heap_t *h_init(size_t bytes, bool unsafe_stack, float gc_threshold) {
  if(bytes < sizeof(heap_t)) {
    // if space allocated is not even enough for metadata, don't allocate
    return NULL;
  }

  void *new_heap = malloc(bytes);

  // create metadata struct and place it in the front of the heap
  // the user sees this as a heap_t struct and is therefore not
  // aware that it is the pointer to the whole heap. 
  ((heap_t*) new_heap)->meta_p = new_heap;
  ((heap_t*) new_heap)->user_start_p = new_heap + sizeof(heap_t);
  ((heap_t*) new_heap)->bump_p = new_heap + sizeof(heap_t);
  ((heap_t*) new_heap)->end_p = new_heap + (bytes * 8);
  ((heap_t*) new_heap)->total_size = bytes;
  ((heap_t*) new_heap)->user_size = bytes - sizeof(heap_t);
  ((heap_t*) new_heap)->unsafe_stack = unsafe_stack;
  ((heap_t*) new_heap)->gc_threshold = gc_threshold;;
  
  return new_heap;
}

void h_delete(heap_t *h) {
  free(h);
}
