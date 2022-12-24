#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

struct CACHE_SLOT{
  uint8_t data[BLOCK_SIZE];
  uintptr_t tag;
  bool valid;
  bool dirty;
};

static struct CACHE_SLOT *cache_slot;
static uint32_t cache_total_width = 0, cache_associativity_width = 0, cache_group_width = 0;
static uintptr_t group_num_mask = 0, tag_mask = 0, block_num_mask = 0, block_in_addr_mask = 0;

#define get_group_num(x) ((uintptr_t)(((x) & group_num_mask) >> BLOCK_WIDTH))
#define get_block_in_addr(x) ((uintptr_t)((x) & block_in_addr_mask))
#define get_tag(x) ((uintptr_t)(((x) & tag_mask) >> (BLOCK_WIDTH + cache_group_width)))
#define get_block_num(x) ((uintptr_t)(((x) & block_num_mask) >> BLOCK_WIDTH))
#define get_inner_addr(x) ((uintptr_t)((get_block_in_addr((x))) & (~0x3)))

#define get_line_num(x, i) ((uintptr_t)((get_group_num(x) << cache_associativity_width) + i))
#define get_ramdom_line(n) ((uint32_t)(rand() % (int)(n))) 
#define get_block_from_cache(tag, num) ((uintptr_t)(((tag) << cache_group_width) | (num)))

static void read_block_from_mem(uintptr_t addr, uint32_t index) {
  struct CACHE_SLOT *goal = &cache_slot[get_line_num(addr, index)];
  printf("Read from happens in line %u with tag 0x%08lx, block: %u\n", get_line_num(addr, index), goal->tag, get_block_num(addr));
  mem_read(get_block_num(addr), goal->data);
  goal->tag = get_tag(addr);
  goal->valid = true;
  goal->dirty = false;
}

static void write_back(uintptr_t addr, uint32_t index) {
  struct CACHE_SLOT *goal = &cache_slot[get_line_num(addr, index)];
  if (!goal->dirty) return;
  printf("Write_back happens in line %u with tag 0x%08lx, block: %u\n", get_line_num(addr, index), goal->tag, get_block_from_cache(goal->tag, get_group_num(addr)));
  mem_write(get_block_from_cache(goal->tag, get_group_num(addr)), goal->data);
  goal->dirty = false;
}

static uint32_t get_index(uintptr_t addr) {
  uint32_t index = 0;
  bool find_empty = false;
  for (index = get_line_num(addr, 0); index < get_line_num(addr, cache_associativity_width); index++) {
    if (!cache_slot[index].valid) {
      find_empty = true;
      break;
    }
  }
  if (!find_empty) index = get_ramdom_line(cache_associativity_width);
  return index;
}

uint32_t cache_read(uintptr_t addr) {
  for (uint32_t i = get_line_num(addr, 0); i < get_line_num(addr, cache_associativity_width); i++) {
    if (cache_slot[i].valid && get_tag(addr) == cache_slot[i].tag)
      return *((uint32_t *)(&cache_slot[i].data[get_inner_addr(addr)]));  
  }
  uint32_t index = get_index(addr);
  printf("Cache read not hit, addr: 0x%08lx, index: %u, line: %u, group: %ld, \n", addr, index, get_line_num(addr, index), get_group_num(addr));
  write_back(addr, index);
  read_block_from_mem(addr, index);
  return *((uint32_t *)(&cache_slot[get_line_num(addr, index)].data[get_inner_addr(addr)]));
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  bool is_hit = false;
  uint32_t i;
  for (i = get_line_num(addr, 0); i < get_line_num(addr, cache_associativity_width); i++) {
    if (cache_slot[i].valid && get_tag(addr) == cache_slot[i].tag) {
      is_hit = true;
      break;
    }
  }
  struct CACHE_SLOT *goal;
  if (is_hit) goal = &cache_slot[i];
  else {
    uint32_t index = get_index(addr);
  printf("Cache write not hit, addr: 0x%08lx, index: %u, line: %u, group: %ld, \n", addr, index, get_line_num(addr, index), get_group_num(addr));
    goal = &cache_slot[get_line_num(addr, index)];
    if (goal->valid) write_back(addr, index);
    read_block_from_mem(addr, index);
  }

  uint32_t *updating_data = (uint32_t *)(&goal->data[get_inner_addr(addr)]);
  *updating_data = (data & wmask) | (*updating_data & (~wmask));
  goal->dirty = true;
}

void init_cache(int total_size_width, int associativity_width) {
  cache_total_width = total_size_width;
  cache_associativity_width = associativity_width;
  cache_group_width = total_size_width - BLOCK_WIDTH - associativity_width;

  block_in_addr_mask = mask_with_len(BLOCK_WIDTH);
  block_num_mask = ~block_in_addr_mask;
  group_num_mask = mask_with_len(cache_group_width) << BLOCK_WIDTH;
  tag_mask = ~(group_num_mask | block_in_addr_mask);

  cache_slot = malloc(cache_group_width * cache_associativity_width * sizeof(struct CACHE_SLOT));
  assert(cache_slot);
}

void display_statistic(void) {
}
