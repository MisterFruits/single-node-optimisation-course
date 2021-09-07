# The myIntrinsics.h wrapper documentation

## Purpose

This is a wrapper for vector intrinsic functions. It works for SSE-128, AVX-256 and ARM NEON-128 instructions.
SSE-128 and AVX-256 wrapper supports simple and double precision floating-point numbers. There is no support for integers.
ARM NEON-128 wrapper supports only simple precision floating-point numbers.

With the `myIntrinsics.h` wrapper you do not need to write a specific intrinsic code anymore. Just use provided functions and the wrapper will automatically generates the right intrisic calls.

## Manual

### Vector register declaration

Just use the `vec` type.
```c
vec r1, r2, r3; // we have declared 3 registers
```

But we do not know the size of the vectors here. The size of vectors can be read from the `VECTOR_SIZE` macro.
```c
for(int i = 0; i < n; i += VECTOR_SIZE) {
	// ...
}
```

### Vector load and store instructions

Firstly, vectors loads or stores need to be aligned on the vector size.
The `REQUIRED_ALIGNEMENT` macro  returns the good alignment (in bytes) for data allocations depending on the instructions type.
```c
float *myPointer = (float*)_mm_malloc(n * sizeof(float), REQUIRED_ALIGNEMENT);
```

Now, if the data are correctly allocated we can perform a vector loading with the `vec_load` call:
```c
// prototype
vec vec_load(type *mem_addr);

// example of use
int n = VECTOR_SIZE * 10;
float *myPointer = (float*)_mm_malloc(n * sizeof(float), REQUIRED_ALIGNEMENT);
int i = 0;
vec r1 = vec_load(myPointer + i*VECTOR_SIZE);
```

Store can be done with the `vec_store` call:
```c
// prototype
void vec_store(type *mem_addr, vec v);

// example of use
int n = VECTOR_SIZE * 10;
float *myPointer = (float*)_mm_malloc(n * sizeof(float), REQUIRED_ALIGNEMENT);
int i = 0;
vec r1 = vec_load(myPointer + i*VECTOR_SIZE);

// do something with r1 (or nothing in fact)

vec_store(myPointer + (i+1)*VECTOR_SIZE, r1);

_mm_free(myPointer);
```

### Vector initialization

We can initialize a full vector from a scalar value with the `vec_set1` call:
```c
// prototype
vec vec_set1(type val);

// example of use
vec r1; // r1 = | unknown | unknown | unknown | unknown |

r1 = vec_set1(1.0); // r1 = | +1.0 | +1.0 | +1.0 | +1.0 |
```

### Vector computational instructions

Add two vectors:
```c
// prototype, return v1 + v2
vec vec_add(vec v1, vec v2);

// example of use
vec r1, r2, r3;

r1 = vec_set1(1.0);   // r1 = | +1.0 | +1.0 | +1.0 | +1.0 |
r2 = vec_set2(2.0);   // r2 = | +2.0 | +2.0 | +2.0 | +2.0 |

r3 = vec_add(r1, r2); // r3 = | +3.0 | +3.0 | +3.0 | +3.0 |
```

Subtract two vectors:
```c
// prototype, return v1 - v2
vec vec_sub(vec v1, vec v2);

// example of use
vec r1, r2, r3;

r1 = vec_set1(1.0);   // r1 = | +1.0 | +1.0 | +1.0 | +1.0 |
r2 = vec_set2(2.0);   // r2 = | +2.0 | +2.0 | +2.0 | +2.0 |

r3 = vec_sub(r1, r2); // r3 = | -1.0 | -1.0 | -1.0 | -1.0 |
```

Multiply two vectors:
```c
// prototype, return v1 * v2
vec vec_mul(vec v1, vec v2);

// example of use
vec r1, r2, r3;

r1 = vec_set1(1.0);   // r1 = | +1.0 | +1.0 | +1.0 | +1.0 |
r2 = vec_set2(2.0);   // r2 = | +2.0 | +2.0 | +2.0 | +2.0 |
 
r3 = vec_mul(r1, r2); // r3 = | +2.0 | +2.0 | +2.0 | +2.0 |
```

Divide two vectors:
```c
// prototype, return v1 / v2
vec vec_div(vec v1, vec v2);

// example of use
vec r1, r2, r3;

r1 = vec_set1(1.0);   // r1 = | +1.0 | +1.0 | +1.0 | +1.0 |
r2 = vec_set2(2.0);   // r2 = | +2.0 | +2.0 | +2.0 | +2.0 |

r3 = vec_div(r1, r2); // r3 = | +0.5 | +0.5 | +0.5 | +0.5 |
```

Fused multiply and add of three vectors:
```c
// prototype, return (v1 * v2) + v3
vec vec_fmadd(vec v1, vec v2, vec v3);

// example of use
vec r1, r2, r3, r4;

r1 = vec_set1(2.0);         // r1 = | +2.0 | +2.0 | +2.0 | +2.0 |
r2 = vec_set2(3.0);         // r2 = | +3.0 | +3.0 | +3.0 | +3.0 |
r2 = vec_set2(1.0);         // r3 = | +1.0 | +1.0 | +1.0 | +1.0 |

r4 = vec_fmadd(r1, r2, r3); // r4 = | +7.0 | +7.0 | +7.0 | +7.0 |
```

Square root of a vector:
```c
// prototype, return sqrt(v1)
vec vec_sqrt(vec v1);

// example of use
vec r1, r2;

r1 = vec_set1(9.0); // r1 = | +9.0 | +9.0 | +9.0 | +9.0 |

r2 = vec_sqrt(r1);  // r2 = | +3.0 | +3.0 | +3.0 | +3.0 |
```

Reciprocal square root of a vector (be careful: this instrinsic exists only for simple precision floating-point numbers):
```c
// prototype, return (1.0 / sqrt(v1))
vec vec_rsqrt(vec v1);

// example of use
vec r1, r2;

r1 = vec_set1(9.0); // r1 = | +9.0 | +9.0 | +9.0 | +9.0 |

r2 = vec_rsqrt(r1); // r2 = | +0.3 | +0.3 | +0.3 | +0.3 |
```

### Vector comparisons

Find the minimum between two vectors:
```c
// prototype, return min(v1, v2)
vec vec_min(vec v1, vec v2);

// example of use
vec r1, r2, r3;

r1 = vec_set1(2.0);   // r1 = | +2.0 | +2.0 | +2.0 | +2.0 |
r2 = vec_set1(3.0);   // r2 = | +3.0 | +3.0 | +3.0 | +3.0 |

r3 = vec_min(r1, r2); // r3 = | +2.0 | +2.0 | +2.0 | +2.0 |
```

Find the maximum between two vectors:
```c
// prototype, return max(v1, v2)
vec vec_max(vec v1, vec v2);

// example of use
vec r1, r2, r3;

r1 = vec_set1(2.0);   // r1 = | +2.0 | +2.0 | +2.0 | +2.0 |
r2 = vec_set1(3.0);   // r2 = | +3.0 | +3.0 | +3.0 | +3.0 |

r3 = vec_max(r1, r2); // r3 = | +3.0 | +3.0 | +3.0 | +3.0 |
```

### Vector rotation

The `vec_rot` function allows you to perform vector rotation:
```c
// prototype
vec vec_rot(vec v1);

// example of use
vec r1, r2;
// let's consider that the r1 register is initialized like this:
//                   r1 = | +3.0 | +2.0 | +1.0 | +0.0 |
r2 = vec_rot(r1); // r2 = | +0.0 | +3.0 | +2.0 | +1.0 |
r1 = vec_rot(r2); // r1 = | +1.0 | +0.0 | +3.0 | +2.0 |
r2 = vec_rot(r1); // r2 = | +2.0 | +1.0 | +0.0 | +3.0 |
r1 = vec_rot(r2); // r1 = | +3.0 | +2.0 | +1.0 | +0.0 |
```
