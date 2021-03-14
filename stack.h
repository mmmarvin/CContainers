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
#ifndef STACK_H
#define STACK_H

#include "vector.h"

#define STACK_TYPE(T) stack_##T
#define STACK_TYPE_PTR(T) STACK_TYPE(T)*

//#define IMPL_STACK_REQUIRED(T) \
//  IMPL_VECTOR_BASE(T) \
//  IMPL_VECTOR_ALLOCATE(T) \
//  IMPL_VECTOR_PUSH_VAL(T) \
//  IMPL_VECTOR_POP(T) \
//  IMPL_VECTOR_EMPTY(T) \
//  IMPL_VECTOR_SIZE(T)

#define STACK_T(T) \
  IMPL_STACK_BASE(T) \
  IMPL_STACK_PUSH_VAL(T) \
  IMPL_STACK_POP(T) \
  IMPL_STACK_EMPTY(T) \
  IMPL_STACK_SIZE(T)

#define STACK_T_STRUCT(T) \
  IMPL_STACK_BASE(T) \
  IMPL_STACK_PUSH_PTR(T) \
  IMPL_STACK_POP(T) \
  IMPL_STACK_EMPTY(T) \
  IMPL_STACK_SIZE(T)

#define IMPL_STACK_BASE(T) \
typedef vector_##T stack_##T; \
\
void stack_init_##T(STACK_TYPE_PTR(T) s) \
{ \
  vector_init_##T(s); \
} \
\
void stack_destroy_##T(STACK_TYPE_PTR(T) s) \
{ \
  vector_destroy_##T(s); \
}

#define IMPL_STACK_PUSH_VAL(T) \
bool stack_push_##T(STACK_TYPE_PTR(T) s, T val) \
{ \
  return vector_push_back_##T(s, val); \
}

#define IMPL_STACK_PUSH_PTR(T) \
bool stack_push_##T(STACK_TYPE_PTR(T) s, const T* val) \
{ \
  return vector_push_back_##T(s, val); \
}

#define IMPL_STACK_POP(T) \
void stack_pop_##T(STACK_TYPE_PTR(T) s, T* out) \
{ \
  vector_pop_back_##T(s, out); \
}

#define IMPL_STACK_EMPTY(T) \
bool stack_empty_##T(STACK_TYPE_PTR(T) s) \
{ \
  return vector_empty_##T(s); \
}

#define IMPL_STACK_SIZE(T) \
size_t stack_size_##T(STACK_TYPE_PTR(T) s) \
{ \
  return vector_size_##T(s); \
}

#endif // STACK_H
