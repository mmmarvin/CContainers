/**********
 * MIT License
 *
 * Copyright 2020 Marvin Manese
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ***********/
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"

#define LIST_TYPE(T) list_##T
#define LIST_TYPE_PTR(T) LIST_TYPE(T)*

#define LIST_T(T) \
  IMPL_LIST_BASE(T) \
  IMPL_LIST_CLEAR(T) \
  IMPL_LIST_PUSH_BACK_VAL(T) \
  IMPL_LIST_PUSH_FRONT_VAL(T) \
  IMPL_LIST_INSERT_VAL(T) \
  IMPL_LIST_ERASE(T) \
  IMPL_LIST_POP_BACK(T) \
  IMPL_LIST_POP_FRONT(T) \
  IMPL_LIST_AT(T) \
  IMPL_LIST_BACK(T) \
  IMPL_LIST_FRONT(T) \
  IMPL_LIST_EMPTY(T) \
  IMPL_LIST_SIZE(T)

#define LIST_T_STRUCT(T) \
  IMPL_LIST_BASE(T) \
  IMPL_LIST_CLEAR(T) \
  IMPL_LIST_PUSH_BACK_PTR(T) \
  IMPL_LIST_PUSH_FRONT_PTR(T) \
  IMPL_LIST_INSERT_PTR(T) \
  IMPL_LIST_ERASE(T) \
  IMPL_LIST_POP_BACK(T) \
  IMPL_LIST_POP_FRONT(T) \
  IMPL_LIST_AT(T) \
  IMPL_LIST_BACK(T) \
  IMPL_LIST_FRONT(T) \
  IMPL_LIST_EMPTY(T) \
  IMPL_LIST_SIZE(T) \

#define IMPL_LIST_BASE(T) \
struct detail_list_data \
{ \
  T*                        data; \
  struct detail_list_data*  next; \
  struct detail_list_data*  prev; \
}; \
\
typedef struct \
{ \
  struct detail_list_data* _head; \
  struct detail_list_data* _tail; \
} LIST_TYPE(T); \
\
void detail_list_connect_##T(LIST_TYPE_PTR(T) l, struct detail_list_data* root, struct detail_list_data* new_data) \
{ \
  if(!l->_head) { \
    l->_head = l->_tail = new_data; \
  } else { \
    if(l->_head == root && l->_head != l->_tail) { \
      new_data->next = root; \
      new_data->prev = root->prev; \
      root->prev = new_data; \
      l->_head = new_data; \
    } else { \
      new_data->next = root->next; \
      new_data->prev = root; \
      root->next = new_data; \
      if(l->_tail == root) { \
        l->_tail = new_data; \
      } \
    } \
  } \
} \
\
void detail_list_disconnect_##T(LIST_TYPE_PTR(T) l, struct detail_list_data* data) \
{ \
  if(l->_head == l->_tail) { \
    l->_head = l->_tail = NULL; \
  } else { \
    if(l->_head == data) { \
      data->next->prev = NULL; \
      l->_head = data->next; \
    } else if(l->_tail == data) { \
      data->prev->next = NULL; \
      l->_tail = data->prev; \
    } else { \
      data->prev->next = data->next; \
      data->next->prev = data->prev; \
    } \
  } \
} \
\
void detail_list_destroy_item_##T(struct detail_list_data* data) \
{ \
  (*_ccontainer_default_allocator.dealloc)(data->data); \
  (*_ccontainer_default_allocator.dealloc)(data); \
} \
\
void list_init_##T(LIST_TYPE_PTR(T) l) \
{ \
  l->_head = NULL; \
  l->_tail = NULL; \
} \
\
void list_destroy_##T(LIST_TYPE_PTR(T) l) \
{ \
  struct detail_list_data* tail; \
  while(l->_head) { \
    tail = l->_tail; \
    detail_list_disconnect_##T(l, tail); \
\
    detail_list_destroy_item_##T(tail); \
  } \
}

#define IMPL_LIST_CLEAR(T) \
void list_clear_##T(LIST_TYPE_PTR(T) l) \
{ \
  list_destroy_##T(l); \
}

#define IMPL_LIST_PUSH_BACK_VAL(T) \
bool list_push_back_##T(LIST_TYPE_PTR(T) l, T value) \
{ \
  struct detail_list_data* new_tail; \
\
  new_tail = (*_ccontainer_default_allocator.alloc)(sizeof(struct detail_list_data)); \
  if(!new_tail) { \
    return false; \
  } \
\
  new_tail->data = (*_ccontainer_default_allocator.alloc)(sizeof(T)); \
  if(!new_tail->data) { \
    (*_ccontainer_default_allocator.dealloc)(new_tail); \
    return false; \
  } \
\
  memcpy(new_tail->data, &value, sizeof(T)); \
  new_tail->next = NULL; \
  new_tail->prev = NULL; \
\
  detail_list_connect_##T(l, l->_tail, new_tail); \
  return true; \
}

#define IMPL_LIST_PUSH_FRONT_VAL(T) \
bool list_push_front_##T(LIST_TYPE_PTR(T) l, T value) \
{ \
  struct detail_list_data* new_head; \
\
  new_head = (*_ccontainer_default_allocator.alloc)(sizeof(struct detail_list_data)); \
  if(!new_head) { \
    return false; \
  } \
\
  new_head->data = (*_ccontainer_default_allocator.alloc)(sizeof(T)); \
  if(!new_head->data) { \
    (*_ccontainer_default_allocator.dealloc)(new_head); \
    return false; \
  } \
\
  memcpy(new_head->data, &value, sizeof(T)); \
  new_head->next = NULL; \
  new_head->prev = NULL; \
\
  detail_list_connect_##T(l, l->_head, new_head); \
  return true; \
}

#define IMPL_LIST_PUSH_BACK_PTR(T) \
bool list_push_back_##T(LIST_TYPE_PTR(T) l, const T* value) \
{ \
  struct detail_list_data* new_tail; \
\
  new_tail = (*_ccontainer_default_allocator.alloc)(sizeof(struct detail_list_data)); \
  if(!new_tail) { \
    return false; \
  } \
\
  new_tail->data = (*_ccontainer_default_allocator.alloc)(sizeof(T)); \
  if(!new_tail->data) { \
    (*_ccontainer_default_allocator.dealloc)(new_tail); \
    return false; \
  } \
\
  memcpy(new_tail->data, value, sizeof(T)); \
  new_tail->next = NULL; \
  new_tail->prev = NULL; \
\
  detail_list_connect_##T(l, l->_tail, new_tail); \
  return true; \
}

#define IMPL_LIST_PUSH_FRONT_PTR(T) \
bool list_push_front_##T(LIST_TYPE_PTR(T) l, const T* value) \
{ \
  struct detail_list_data* new_head; \
\
  new_head = (*_ccontainer_default_allocator.alloc)(sizeof(struct detail_list_data)); \
  if(!new_head) { \
    return false; \
  } \
\
  new_head->data = (*_ccontainer_default_allocator.alloc)(sizeof(T)); \
  if(!new_head->data) { \
    (*_ccontainer_default_allocator.dealloc)(new_head); \
    return false; \
  } \
\
  memcpy(new_head->data, value, sizeof(T)); \
  new_head->next = NULL; \
  new_head->prev = NULL; \
\
  detail_list_connect_##T(l, l->_head, new_head); \
  return true; \
}

#define IMPL_LIST_INSERT_VAL(T) \
bool list_insert_##T(LIST_TYPE_PTR(T) l, size_t pos, T value) \
{ \
  struct detail_list_data* new_data, * root; \
\
  new_data = (*_ccontainer_default_allocator.alloc)(sizeof(struct detail_list_data)); \
  if(!new_data) { \
    return false; \
  } \
\
  new_data->data = (*_ccontainer_default_allocator.alloc)(sizeof(T)); \
  if(!new_data->data) { \
    (*_ccontainer_default_allocator.dealloc)(new_data); \
    return false; \
  } \
\
  memcpy(new_data->data, &value, sizeof(T)); \
  new_data->next = NULL; \
  new_data->prev = NULL; \
\
  root = l->_head; \
  while(pos) { \
    root = root->next; \
    --pos; \
  } \
\
  detail_list_connect_##T(l, root, new_data); \
  return true; \
}

#define IMPL_LIST_INSERT_PTR(T) \
bool list_insert_##T(LIST_TYPE_PTR(T) l, size_t pos, const T* value) \
{ \
  struct detail_list_data* new_data, * root; \
  \
  new_data = (*_ccontainer_default_allocator.alloc)(sizeof(struct detail_list_data)); \
  if(!new_data) { \
  return false; \
  } \
  \
  new_data->data = (*_ccontainer_default_allocator.alloc)(sizeof(T)); \
  if(!new_data->data) { \
  (*_ccontainer_default_allocator.dealloc)(new_data); \
  return false; \
  } \
  \
  memcpy(new_data->data, value, sizeof(T)); \
  new_data->next = NULL; \
  new_data->prev = NULL; \
  \
  root = l->_head; \
  while(pos) { \
  root = root->next; \
  --pos; \
  } \
  \
  detail_list_connect_##T(l, root, new_data); \
  return true; \
}

#define IMPL_LIST_ERASE(T) \
void list_erase_##T(LIST_TYPE_PTR(T) l, size_t index) \
{ \
  struct detail_list_data* cur = l->_head; \
\
  while(index--) { \
    cur = cur->next; \
  } \
\
  detail_list_disconnect_##T(l, cur); \
  (*_ccontainer_default_allocator.dealloc)(cur->data); \
  (*_ccontainer_default_allocator.dealloc)(cur); \
}

#define IMPL_LIST_POP_BACK(T) \
void list_pop_back_##T(LIST_TYPE_PTR(T) l, T* out) \
{ \
  struct detail_list_data* tail = l->_tail; \
\
  detail_list_disconnect_##T(l, tail); \
  memcpy(out, tail, sizeof(T)); \
\
  detail_list_destroy_item_##T(tail); \
}

#define IMPL_LIST_POP_FRONT(T) \
void list_pop_front_##T(LIST_TYPE_PTR(T) l, T* out) \
{ \
  struct detail_list_data* head = l->_head; \
\
  detail_list_disconnect_##T(l, head); \
  memcpy(out, head, sizeof(T)); \
\
  detail_list_destroy_item_##T(head); \
}

#define IMPL_LIST_AT(T) \
T* list_at_##T(LIST_TYPE_PTR(T) l, size_t index) \
{ \
  struct detail_list_data* cur; \
\
  cur = l->_head; \
  while(index) { \
    cur = cur->next; \
    --index; \
  } \
\
  return cur->data; \
}

#define IMPL_LIST_BACK(T) \
T* list_back_##T(LIST_TYPE_PTR(T) l) \
{ \
  return l->_tail->data; \
}

#define IMPL_LIST_FRONT(T) \
T* list_front_##T(LIST_TYPE_PTR(T) l) \
{ \
  return l->_head->data; \
}

#define IMPL_LIST_EMPTY(T) \
bool list_empty_##T(LIST_TYPE_PTR(T) l) \
{ \
  return l->_head == NULL; \
}

#define IMPL_LIST_SIZE(T) \
size_t list_size_##T(LIST_TYPE_PTR(T) l) \
{ \
  size_t ret = 0; \
  struct detail_list_data* cur; \
\
  cur = l->_head; \
  while(cur != NULL) { \
    ++ret; \
    cur = cur->next; \
  } \
\
  return ret; \
}

#endif // LIST_H
