// THIS CODE IS TAKEN FROM https://github.com/Jibus22/malloc
// THE CODE IS DISTRIBUTED WITHOUT A LICENSE.
// THIS REDISTRIBUTION INCLUDES MODIFICATIONS MADE TO INTEGRATE IT WITH THE REST
// OF THE FRESH CRT.

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sysdeps/intf.h>
#include <unistd.h>

#define TINY_FACTOR 10
#define SMALL_FACTOR 150

typedef enum zone { tiny, small, large } e_zone;

/**
 * allocation's metadata
 */
typedef struct s_alloc {
  struct s_alloc *prev, *next;
  char *payload;
  unsigned int unusued;
  unsigned int size;
} t_alloc;

/**
 * zone's metadata
 */
typedef struct s_zone {
  struct s_zone *prev, *next;
  t_alloc *start; /* address of first allocation */
  e_zone type;
  unsigned int vacant_max; /* Size of page(s) - size of t_zone */
} t_zone;

/**
 * main global node for internal malloc, free, (...) memory managment
 */
typedef struct s_mnode {
  t_zone *zone;
  unsigned int tiny_smax;
  unsigned int small_smax;
} t_mnode;

void _mnode_init();
t_alloc *_find_alloc(t_zone *zone, void *ptr);
t_zone *_find_zone(t_zone *zone, void *ptr);
void _setAllocType(size_t size, e_zone *alloc_type);
size_t _getZoneSize(e_zone alloc_type, size_t size);
void _updateVacantMax(t_zone *zone, size_t zonesize);

void _concat_address(char *dst, unsigned long int n);
void _concat_uint(char *dst, unsigned int n);
void _print_addr(void *ptr, size_t size, const char *title);

void _optional_abort(const char *msg, void *ptr);

static t_mnode g_mnode = {NULL, 0, 0};

void _mnode_init() {
  int pagesize = SYS_PAGE_SIZE;

  g_mnode.tiny_smax =
      ((TINY_FACTOR * pagesize - sizeof(t_zone)) / 100) - sizeof(t_alloc);
  g_mnode.small_smax =
      ((SMALL_FACTOR * pagesize - sizeof(t_zone)) / 100) - sizeof(t_alloc);
}

void _setAllocType(size_t size, e_zone *alloc_type) {
  *alloc_type =
      (0 * (size <= g_mnode.tiny_smax)) +
      (1 * (size > g_mnode.tiny_smax) && (size <= g_mnode.small_smax)) +
      (2 * (size > g_mnode.small_smax));
}

/**
 * return the full size of zone, from start to the end, without substracting any
 * dope vector. The bitwise trick for large type is made to get a size which
 * is a multiple of pagesize. It rounds up the value.
 */
size_t _getZoneSize(e_zone alloc_type, size_t large_alloc_size) {
  int pagesize = SYS_PAGE_SIZE;
  return ((pagesize * TINY_FACTOR) * (alloc_type == tiny)) +
         ((pagesize * SMALL_FACTOR) * (alloc_type == small)) +
         ((((large_alloc_size + sizeof(t_zone) + sizeof(t_alloc)) +
            (pagesize - 1)) &
           ~(pagesize - 1)) *
          (alloc_type == large));
}

/**
 * roam the t_zone to calculate space into each t_alloc and find and sets the
 * biggest free memory area so that the vacant_max data of the current zone
 * get updated.
 * zonesize must be the true zonesize given by _getZoneSize() with the size of
 * a block.
 */
void _updateVacantMax(t_zone *zone, size_t zonesize) {
  t_alloc *head = zone->start;
  unsigned int new_vacant = 0;
  unsigned long diff;

  if (!head) {
    zone->vacant_max = zonesize - sizeof(t_zone);
    return;
  }
  while (head->next) {
    diff = ((char *)head->next) - ((char *)(head->payload + head->size));
    new_vacant =
        (new_vacant * (diff <= new_vacant)) + (diff * (diff > new_vacant));
    head = head->next;
  }
  diff = ((char *)zone + zonesize) - ((char *)head->payload + head->size);
  new_vacant =
      (new_vacant * (diff <= new_vacant)) + (diff * (diff > new_vacant));
  zone->vacant_max = new_vacant;
}

t_alloc *_find_alloc(t_zone *zone, void *ptr) {
  t_alloc *head;

  if (!zone)
    return NULL;
  head = zone->start;
  while (head) {
    if (head->payload == ptr)
      return head;
    head = head->next;
  }
  return NULL;
}

t_zone *_find_zone(t_zone *zone, void *ptr) {
  char *end;

  while (zone) {
    if (!zone->start) {
      zone = zone->next;
      continue;
    }
    end = (char *)zone + _getZoneSize(zone->type, zone->start->size);
    if (ptr >= (void *)((t_alloc *)(zone + 1) + 1) && (char *)ptr < end)
      return zone;
    zone = zone->next;
  }
  return NULL;
}

/**
 * main global node for internal malloc, free, (...) memory managment
 */

/**
 * Create a zone of a size corresponding to which had been requested by the
 * client: tiny, small or large.
 * The memory is requested with mmap() and t_zone metadata added to its
 * begining. Its address is returned.
 */
static void *_create_zone(size_t size, e_zone alloc_type) {
  void *addr;
  size_t zonesize = _getZoneSize(alloc_type, size);

  t_zone new_zone = {NULL, NULL, NULL, alloc_type, zonesize - sizeof(t_zone)};

  addr = mmap(NULL, zonesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE,
              -1, 0);
  if (addr == MAP_FAILED)
    return NULL;
  memcpy(addr, &new_zone, sizeof(new_zone));
  return addr;
}

/**
 * returns a zone according to the allocation size requested and which has
 * enough space inside, or NULL
 */
static t_zone *_getZone(size_t size, e_zone alloc_type) {
  t_zone *head = g_mnode.zone;

  if (alloc_type == large)
    return NULL;
  while (head) {
    if (head->type == alloc_type &&
        (head->vacant_max > (size + sizeof(t_alloc))))
      return head;
    head = head->next;
  }
  return head;
}

/**
 * add a new zone to the list pointed by g_mnode.zone
 */
static void _addZone(t_zone *zone) {
  t_zone *head = g_mnode.zone;

  if (!head) {
    g_mnode.zone = zone;
    return;
  }
  while (head->next)
    head = head->next;
  head->next = zone;
  zone->prev = head;
}

static t_alloc *_setAlloc(t_alloc *head, t_alloc *prev, t_alloc *next,
                          unsigned int size) {
  t_alloc setalloc = {prev, next, (char *)(head + 1), 0, size};

  memcpy(head, &setalloc, sizeof(*head));
  return head;
}

/**
 * roam t_zone to find a place where there is enough space to set a new t_alloc,
 *  then set the new t_alloc and update the previous and next t_alloc so the
 * linked list of t_alloc stays consistent.
 * Returns the newly created t_alloc.
 */
static t_alloc *_getAlloc(t_zone *zone, size_t size, void *end) {
  t_alloc *ptr, *head = zone->start;

  if (!head) {
    ptr = _setAlloc((t_alloc *)(zone + 1), NULL, NULL, size);
    zone->start = ptr;
    return ptr;
  }
  if ((char *)head > ((char *)(zone + 1) + sizeof(t_alloc) + size)) {
    ptr = _setAlloc((t_alloc *)(zone + 1), NULL, head, size);
    zone->start = ptr;
    head->prev = ptr;
    return ptr;
  }
  while (head->next) {
    if ((char *)head->next >
        (char *)(head->payload + head->size + size + sizeof(t_alloc))) {
      ptr = _setAlloc((t_alloc *)(head->payload + head->size), head, head->next,
                      size);
      head->next->prev = ptr;
      head->next = ptr;
      return ptr;
    }
    head = head->next;
  }
  if (((char *)end >=
       (char *)(head->payload + head->size + sizeof(t_alloc) + size))) {
    ptr = (t_alloc *)(head->payload + head->size);
    head->next = ptr;
    return _setAlloc(ptr, head, NULL, size);
  }
  return NULL;
}

static void *_create_client_alloc(t_zone *zone, size_t size) {
  size_t zonesize = _getZoneSize(zone->type, size);
  t_alloc *head = _getAlloc(zone, size, (char *)zone + zonesize);

  if (!head)
    return NULL;
  _updateVacantMax(zone, zonesize);
  return head->payload;
}

void *malloc(size_t size) {
  t_zone *zone;
  e_zone alloc_type;
  void *client_alloc;

  if (!size)
    return NULL;
  size = (size + 15) & ~15;
  if (!g_mnode.tiny_smax)
    _mnode_init();
  _setAllocType(size, &alloc_type);
  zone = _getZone(size, alloc_type);
  if (!zone) {
    zone = _create_zone(size, alloc_type);
    if (!zone) {
      return NULL;
    }
    _addZone(zone);
  }
  client_alloc = _create_client_alloc(zone, size);
  return client_alloc;
}

static bool _is_same_type(size_t size_c, size_t size_n) {
  e_zone alloc_type_c, alloc_type_n;

  if (!g_mnode.tiny_smax)
    _mnode_init();
  _setAllocType(size_c, &alloc_type_c);
  _setAllocType(size_n, &alloc_type_n);
  return alloc_type_c == alloc_type_n;
}

static char *_soft_realloc(t_zone *zone, t_alloc *alloc, size_t size,
                           size_t zonesize) {
  char *next;

  if (!_is_same_type(alloc->size, size))
    return NULL;
  if (size < alloc->size) {
  } else {
    next = (char *)(((unsigned long)alloc->next *
                     (unsigned long)(alloc->next != NULL)) +
                    ((unsigned long)((char *)zone + zonesize) *
                     (unsigned long)(!alloc->next)));
    if (next > alloc->payload + size) {

    } else
      return NULL;
  }
  alloc->size = size;
  _updateVacantMax(zone, zonesize);
  return alloc->payload;
}

static char *_realloc(t_zone *zone, t_alloc *alloc, size_t size) {
  size_t zonesize = _getZoneSize(zone->type, zone->start->size);
  char *res = _soft_realloc(zone, alloc, size, zonesize);
  void *new;

  if (res)
    return res;
  new = malloc(size);
  if (!new)
    return NULL;
  memmove(new, alloc->payload, size > alloc->size ? alloc->size : size);
  free(alloc->payload);
  return new;
}

void *realloc(void *ptr, size_t size) {
  t_zone *zone = g_mnode.zone;
  t_alloc *match;

  if (!ptr)
    return malloc(size);
  else if (!size) {
    free(ptr);
    return NULL;
  }
  zone = _find_zone(zone, ptr);
  match = _find_alloc(zone, ptr);
  size = (size + 15) & ~15;
  if (!match) {
    return NULL;
  } else if (match->size == size) {
    return ptr;
  }
  return _realloc(zone, match, size);
}
