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
#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

#define QUEUE_TYPE(T) queue_##T
#define QUEUE_TYPE_PTR(T) QUEUE_TYPE(T)*

#define QUEUE_T(T) \
  IMPL_QUEUE_BASE(T) \
  IMPL_QUEUE_PUSH_VAL(T) \
  IMPL_QUEUE_POP(T) \
  IMPL_QUEUE_EMPTY(T) \
  IMPL_QUEUE_SIZE(T)

#define QUEUE_T_STRUCT(T) \
  IMPL_QUEUE_BASE(T) \
  IMPL_QUEUE_PUSH_PTR(T) \
  IMPL_QUEUE_POP(T) \
  IMPL_QUEUE_EMPTY(T) \
  IMPL_QUEUE_SIZE(T)

#define IMPL_QUEUE_BASE(T) \
typedef list_##T queue_##T; \
\
void queue_init_##T(QUEUE_TYPE_PTR(T) q) \
{ \
  list_init_##T(q); \
} \
\
void queue_destroy_##T(QUEUE_TYPE_PTR(T) q) \
{ \
  list_destroy_##T(q); \
}

#define IMPL_QUEUE_PUSH_VAL(T) \
void queue_push_##T(QUEUE_TYPE_PTR(T) q, T val) \
{ \
  list_push_back_##T(q, val); \
}

#define IMPL_QUEUE_PUSH_PTR(T) \
void queue_push_##T(QUEUE_TYPE_PTR(T) q, const T* val) \
{ \
  list_push_back_##T(q, val); \
}

#define IMPL_QUEUE_POP(T) \
void queue_pop_##T(QUEUE_TYPE_PTR(T) q, T* val) \
{ \
  list_pop_front_##T(q, val); \
}

#define IMPL_QUEUE_EMPTY(T) \
bool queue_empty_##T(QUEUE_TYPE_PTR(T) q) \
{ \
  return list_empty_##T(q); \
}

#define IMPL_QUEUE_SIZE(T) \
size_t queue_size_##T(QUEUE_TYPE_PTR(T) q) \
{ \
  return list_size_##T(q); \
}

#endif // QUEUE_H
