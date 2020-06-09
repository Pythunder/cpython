/* Atomic functions: similar to pycore_atomic.h, but don't need
   to declare variables as atomic.

   _Py_atomic_int_get() uses __ATOMIC_SEQ_CST memory order.
*/
#ifndef Py_ATOMIC_FUNC_H
#define Py_ATOMIC_FUNC_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_BUILD_CORE
#  error "this header requires Py_BUILD_CORE define"
#endif

/* Use builtin atomic operations in GCC >= 4.7 */
#ifdef HAVE_BUILTIN_ATOMIC

static inline int _Py_atomic_int_get(int *var)
{
    return __atomic_load_n(var, __ATOMIC_SEQ_CST);
}

static inline void _Py_atomic_int_set(int *var, int value)
{
    __atomic_store_n(var, value, __ATOMIC_SEQ_CST);
}

#elif defined(_MSC_VER)

static inline int _Py_atomic_int_get(int *var)
{
    Py_BUILD_ASSERT(sizeof(int) == sizeof(long));
    volatile long *lvar = (volatile long *)var;
    long old;
    do {
        old = *lvar;
    } while(_InterlockedCompareExchange(lvar, old, old) != old);
}

static inline void _Py_atomic_int_set(volatile int *var, int value)
{
    Py_BUILD_ASSERT(sizeof(int) == sizeof(long));
    volatile long *lvar = (volatile long *)var;
    _InterlockedExchange(lvar, value);
}

#else
// Fallback implementation using volatile

static inline int _Py_atomic_int_get(volatile int *var)
{
    return *var;
}

static inline void _Py_atomic_int_set(volatile int *var, int value)
{
    *var = value;
}
#endif

#ifdef __cplusplus
}
#endif
#endif  /* Py_ATOMIC_FUNC_H */
