 
# CContainers
C++ containers implemented for C

# Example usage


```c++
#include "vector.h"
VECTOR_T(int) // instantiate definition for vector of int

int main()
{
	VECTOR_TYPE(int) v;
	int i;

	vector_init(&v);
	for(i = 0; i < 100; ++i) {
		vector_push_back_int(&v, i);
	}

	for(i = 0; i < vector_size_int(&v); ++i) {
		printf("Item #%d: %d", i + 1, *vector_at_int(&v, i));
	}
	vector_destroy(&v);

	return 0;
}
```

# Currently supported:
* Vector
	* clear
	* push_back
	* insert
	* pop_back
	* erase
	* empty
	* size
	* shrink_to_fit
	* at
	* back
	* front
	* capacity
* List
	* clear
	* push_back
	* push_front
	* insert
	* pop_back
	* pop_front
	* erase
	* empty
	* size
	* at
	* back
	* front
* Queue
	* push
	* pop
	* empty
	* size
* Stack
	* push
	* pop
	* empty
	* size

# Custom Allocators:
By default allocator / deallocator used is malloc and free. It can be changed by changing _ccontainer_default_allocator.alloc and _ccontainer_default_allocator.dealloc on header "allocator.h"