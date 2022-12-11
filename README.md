# mem_alloc()

This is an implementation of __stdlib malloc__. Heap is an array of uintptr_t. There is also a garbage collector which frees unreachable chunks of allocated memory.

`void* mem_alloc(size_t size_bytes)` Allocates memory on heap in words. (Padding is done automatically.)

`void mem_free(void* ptr)` Frees memory if its allocated.

`void merge_fragments()`
: Merge fragmented memory. This is called at every `mem_alloc()`.

`void heap_collect()`
: Frees unreachable chunks of allocated memory. This should be called by user.

---
**Limitations**

- No packed structures 
- No tricks that obscure the pointers

---
**I followed Tsoding's "Artifacts of that Memory Management Tsoding Session" tutorial so this reposity is very much identical to his one:**
- [Tsoding's repo](https://github.com/tsoding/memalloc/)
- [Tsoding Writing My Own Malloc in C Video](https://www.youtube.com/watch?v=sZ8GJ1TiMdk)
- [Tsoding Writing Garbage Collector in C Video](https://www.youtube.com/watch?v=2JgEKEd3tw8 )

**Other resources that might be helpful:**

- [Danluu's malloc tutorial](https://danluu.com/malloc-tutorial/)
- [Danluu's malloc repo](https://github.com/danluu/malloc-tutorial)
- [Data structure alignment Wikipedia page](https://en.wikipedia.org/wiki/Data_structure_alignment)
- [Garbage collection Wikipedia page](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science))
- [Jacob Sorber's Garbage collection video](https://www.youtube.com/watch?v=Htwj4jJQeRc)


