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
#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"
#include "utility.h"

#define VECTOR_TYPE(T) vector_##T
#define VECTOR_TYPE_PTR(T) VECTOR_TYPE(T)*

#define VECTOR_T(T) \
  IMPL_VECTOR_BASE(T) \
  IMPL_VECTOR_ALLOCATE(T) \
  IMPL_VECTOR_CLEAR(T) \
  IMPL_VECTOR_PUSH_VAL(T) \
  IMPL_VECTOR_INSERT_VAL(T) \
  IMPL_VECTOR_POP(T) \
  IMPL_VECTOR_AT(T) \
  IMPL_VECTOR_FRONT(T) \
  IMPL_VECTOR_BACK(T) \
  IMPL_VECTOR_DATA(T) \
  IMPL_VECTOR_EMPTY(T) \
  IMPL_VECTOR_SIZE(T) \
  IMPL_VECTOR_CAPACITY(T) \
  IMPL_VECTOR_SHRINK(T) \

#define VECTOR_T_STRUCT(T) \
  IMPL_VECTOR_BASE(T) \
  IMPL_VECTOR_ALLOCATE(T) \
  IMPL_VECTOR_CLEAR(T) \
  IMPL_VECTOR_PUSH_PTR(T) \
  IMPL_VECTOR_INSERT_PTR(T) \
  IMPL_VECTOR_POP(T) \
  IMPL_VECTOR_AT(T) \
  IMPL_VECTOR_FRONT(T) \
  IMPL_VECTOR_BACK(T) \
  IMPL_VECTOR_DATA(T) \
  IMPL_VECTOR_EMPTY(T) \
  IMPL_VECTOR_SIZE(T) \
  IMPL_VECTOR_CAPACITY(T) \
  IMPL_VECTOR_SHRINK(T) \

#define IMPL_VECTOR_BASE(T) \
typedef struct \
{ \
  T*      _ptr; \
  size_t  _size; \
  size_t  _capacity; \
} VECTOR_TYPE(T); \
\
void vector_init_##T(VECTOR_TYPE_PTR(T) vec) \
{ \
  vec->_ptr = NULL; \
  vec->_capacity = vec->_size = 0; \
} \
\
void vector_destroy_##T(VECTOR_TYPE_PTR(T) vec) \
{ \
  if(vec->_ptr) { \
    (*_ccontainer_default_allocator.dealloc)(vec->_ptr); \
  } \
} \

#define IMPL_VECTOR_ALLOCATE(T) \
bool _vector_allocate_##T(VECTOR_TYPE_PTR(T) vec, size_t size) \
{ \
  size_t new_capacity; \
  T* new_data; \
\
  new_capacity = size; \
  new_data = (*_ccontainer_default_allocator.alloc)(sizeof(T) * new_capacity); \
  if(!new_data) { \
    return false; \
  } \
\
  if(vec->_ptr) { \
    memcpy(new_data, vec->_ptr, sizeof(T) * vec->_size); \
    (*_ccontainer_default_allocator.dealloc)(vec->_ptr); \
  } \
\
  vec->_ptr = new_data; \
  vec->_capacity = new_capacity; \
  return true; \
} \
\
bool _vector_grow_##T(VECTOR_TYPE_PTR(T) vec) \
{ \
  return _vector_allocate_##T(vec, CCONTAINER_MAX(vec->_capacity * 2, 10)); \
}

#define IMPL_VECTOR_CLEAR(T) \
void vector_clear_##T(VECTOR_TYPE_PTR(T) vec) \
{ \
  vec->_size = 0; \
}

#define IMPL_VECTOR_PUSH_VAL(T) \
bool vector_push_back_##T(VECTOR_TYPE_PTR(T) vec, T val) \
{ \
  if(vec->_size == vec->_capacity) { \
    if(!_vector_grow_##T(vec)) { \
      return false; \
    } \
  } \
\
  memcpy(&vec->_ptr[vec->_size++], &val, sizeof(T)); \
  return true; \
}

#define IMPL_VECTOR_PUSH_PTR(T) \
bool vector_push_back_##T(VECTOR_TYPE_PTR(T) vec, const T* val) \
{ \
  if(vec->_size == vec->_capacity) { \
    if(!_vector_grow_##T(vec)) { \
      return false; \
    } \
  } \
\
  memcpy(&vec->_ptr[vec->_size++], val, sizeof(T)); \
  return true; \
}

#define IMPL_VECTOR_POP(T) \
void vector_pop_back_##T(VECTOR_TYPE_PTR(T) vec, T* out) \
{ \
  memcpy(out, &vec->_ptr[--vec->_size], sizeof(T)); \
}

#define IMPL_VECTOR_INSERT_VAL(T) \
bool vector_insert_##T(VECTOR_TYPE_PTR(T) vec, size_t pos, T val) \
{ \
  size_t i; \
\
  if(vec->_size == vec->_capacity) { \
    if(!_vector_grow_##T(vec)) { \
      return false; \
    } \
  } \
\
  if(vec->_size) { \
    i = vec->_size; \
    while(i > pos) { \
      memcpy(&vec->_ptr[i], &vec->_ptr[i - 1], sizeof(T)); \
      --i; \
    } \
  } \
  memcpy(&vec->_ptr[pos], &val, sizeof(T)); \
  ++vec->_size; \
\
  return true; \
}

#define IMPL_VECTOR_INSERT_PTR(T) \
bool vector_insert_##T(VECTOR_TYPE_PTR(T) vec, size_t pos, const T* val) \
{ \
  size_t i; \
\
  if(vec->_size == vec->_capacity) { \
    if(!_vector_grow_##T(vec)) { \
      return false; \
    } \
  } \
\
  if(vec->_size) { \
    i = vec->_size; \
    while(i > pos) { \
      memcpy(&vec->_ptr[i], &vec->_ptr[i - 1], sizeof(T)); \
      --i; \
    } \
  } \
  memcpy(&vec->_ptr[pos], val, sizeof(T)); \
  ++vec->_size; \
\
  return true; \
}

#define IMPL_VECTOR_AT(T) \
T* vector_at_##T(VECTOR_TYPE_PTR(T) vec, size_t index) \
{ \
  return &vec->_ptr[index]; \
}

#define IMPL_VECTOR_FRONT(T) \
T* vector_front_##T(VECTOR_TYPE_PTR(T) vec) \
{ \
  return &vec->_ptr[0]; \
}

#define IMPL_VECTOR_BACK(T) \
T* vector_back_##T(VECTOR_TYPE_PTR(T) vec) \
{ \
  return &vec->_ptr[vec->_size - 1]; \
}

#define IMPL_VECTOR_DATA(T) \
T* vector_data_##T(VECTOR_TYPE_PTR(T) vec) \
{ \
  return vec->_ptr; \
}

#define IMPL_VECTOR_EMPTY(T) \
bool vector_empty_##T(VECTOR_TYPE_PTR(T) vec) \
{ \
  return !vec->_size; \
}

#define IMPL_VECTOR_SIZE(T) \
size_t vector_size_##T(VECTOR_TYPE_PTR(T) vec) \
{ \
  return vec->_size; \
}

#define IMPL_VECTOR_CAPACITY(T) \
size_t vector_capacity_##T(VECTOR_TYPE_PTR(T) vec) \
{ \
  return vec->_capacity; \
}

#define IMPL_VECTOR_SHRINK(T) \
bool vector_shrink_to_fit_##T(VECTOR_TYPE_PTR(T) vec) \
{ \
  if(vec->_size) { \
    return _vector_allocate_##T(vec, vec->_size); \
  } \
\
  return true; \
}

#endif // VECTOR_H
