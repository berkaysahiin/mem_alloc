#include <stdio.h>
#include <assert.h>

#define MAX_CHUNK_COUNT 1024
#define HEAP_SIZE_BYTES 64000
char heap[HEAP_SIZE_BYTES];

typedef struct {
  void *start;
  size_t size;
}chunk;

typedef struct {
  int count;
  chunk chunks[MAX_CHUNK_COUNT];
}chunk_list;


chunk_list free_chunks = {
  .count = 1,
  .chunks = {
    [0] = {.start = heap, .size = sizeof(heap)}
  },
};

chunk_list allocated_chunks = {0};


void* mem_alloc(size_t);
void insert_chunk(chunk_list*, chunk);
void remove_chunk(chunk_list*, chunk);
void remove_at_index(chunk_list*, int);
void print_chunk_list(chunk_list);
void merge_fragments(chunk_list* list);
int index_chunk(chunk_list, chunk);
void insert_chunk_attr(chunk_list* list, void* start, size_t size);
void mem_free(void *ptr);

int main() 
{
  printf("-----INITIAL FREE CHUNKS-------\n");
  print_chunk_list(free_chunks);

  int* arr[20];
  for(int i = 0; i < 20; i++) {
    arr[i] = mem_alloc(sizeof(int));
  }

  printf("-----AFTER ALLOC: ALLOCATED CHUNKS-------\n");
  print_chunk_list(allocated_chunks);
  printf("----------FREE CHUNKS-----------------\n");
  print_chunk_list(free_chunks);

  for(int i = 0; i < 20; i++) {
    mem_free(arr[i]);
  } 

  printf("----------AFTER FREE ALL: ALLOCATED CHUNKS-----------------\n");
  print_chunk_list(allocated_chunks);
  printf("----------FREE CHUNKS-----------------\n");
  print_chunk_list(free_chunks);

  printf("----------------AFTER MERGE FRAGMENTS: FREE CHUNKS:------------------------\n");
  merge_fragments(&free_chunks);
  print_chunk_list(free_chunks);
}

void print_chunk_list(chunk_list list)
{
  printf("Chunks (%zu):\n", (unsigned long)list.count);
    for (size_t i = 0; i < list.count; ++i) {
        printf("start: %p, size: %zu\n",
               list.chunks[i].start,
               list.chunks[i].size);
    }
}

void* mem_alloc(size_t size)
{
  if(size < 0) return NULL;

  for(int i = 0; i < free_chunks.count; i++) {
    const chunk c = free_chunks.chunks[i];
    if(c.size >= size) {
      remove_at_index(&free_chunks, i);
      const size_t left_size = c.size - size;
      insert_chunk_attr(&allocated_chunks, c.start, size);
      
      if(left_size > 0) {
        insert_chunk_attr(&free_chunks, c.start + size, left_size);
      }

      return c.start;
    }
  }

  return NULL;
}

void mem_free(void *ptr)
{
  if(ptr == NULL) return;
  chunk temp;
  temp.start = ptr;
  int index = index_chunk(allocated_chunks, temp);
  if(index < 0) return;
  insert_chunk_attr(&free_chunks, 
                    allocated_chunks.chunks[index].start,
                    allocated_chunks.chunks[index].size);
  remove_at_index(&allocated_chunks, index);
}


void insert_chunk_attr(chunk_list* list, void* start, size_t size) 
{
  chunk c;
  c.start = start;
  c.size = size;
  insert_chunk(list, c);
}

void insert_chunk(chunk_list* list, chunk c)
{
  assert(list->count < MAX_CHUNK_COUNT);
  list->chunks[list->count] = c;

  for(size_t i = list->count; i > 0 && list->chunks[i].start < list->chunks[i-1].start; i--) {
    chunk temp = list->chunks[i];
    list->chunks[i] = list->chunks[i-1];
    list->chunks[i - 1] = temp;
  }
  
  list->count++;
}

void remove_chunk(chunk_list* list, chunk c)
{
  int index = index_chunk(*list, c);
  assert(index < list->count);

  for(int i = index; i < list->count -1; i++) {
    list->chunks[i] = list->chunks[i+1];
  }
  list->count--;
}

void remove_at_index(chunk_list* list, int index) 
{ 
  assert(index < list->count);

  for(int i = index; i < list->count -1; i++) {
    list->chunks[i] = list->chunks[i+1];
  }

  list->count--;
}

int index_chunk(chunk_list list, chunk c)
{
  for(int i = 0; i < list.count; i++) {
    if(list.chunks[i].start == c.start) {
      return i;
    }
  }
  return -1;
}


void merge_fragments(chunk_list* list)
{
  chunk_list merged;
  merged.count = 0;

  for (size_t i = 0; i < list->count; ++i) {
    const chunk c = list->chunks[i];
    if (merged.count > 0) {
      chunk *top_chunk = &merged.chunks[merged.count - 1];
      if (top_chunk->start + top_chunk->size == c.start) {
        top_chunk->size += c.size;
      }
      else {
        insert_chunk_attr(&merged, c.start, c.size);
      }
    } 
    else {
      insert_chunk_attr(&merged, c.start, c.size);
    }
  }
  *list = merged;
}



