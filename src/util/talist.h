#pragma once

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#define TAL_DEFAULT_SIZE 16

/*
 * Defines a new typed-list type, or directly instantiate a typed-list variable
 *
 * Example a, declare a variable (list of integers):
 *  tal(int) my_list;
 *
 * Example b, declare a type, and then use the type:
 *   tal(int, my_list_type);
 *   struct my_list_type my_list;
 */
#define tal(TYPE)                       \
        struct {                        \
                size_t length;          \
                size_t capicity;        \
                TYPE *array;            \
        }

/* Initializer: tal(int) my_list = tal_init(); */
#define tal_init() {.length = 0, .capicity = 0, .array = NULL}

/* Length/size of list: printf("size: %zu\n", tll_length(my_list)); */
#define tll_length(list) (list).length

/* Adds a new item to the front of the list */
#define tal_allocate(list, size)                                        \
        do {                                                            \
                if ((size) > (list).capicity) {                         \
                        size_t new_capacity = (list).capicity;          \
                        if (new_capacity == 0)                          \
                                new_capacity = 16;                      \
                        while (new_capacity < (size)) {                 \
                                new_capacity *= 2;                      \
                        }                                               \
                        (list).array = realloc((list).array,            \
                                sizeof(*(list).array) * new_capacity);  \
                        assert((list).array);                           \
                        (list).capicity = new_capacity;                 \
                }                                                       \
        } while (0)

/* Adds a new item to the back of the list */
#define tal_add(list, new_item)                                 \
        do {                                                    \
                tal_allocate(list, (list).length + 1);          \
                (list).array[(list).length] = new_item;         \
                (list).length += 1;                             \
        } while (0)

/* Insert item at index, shifting the following items by one spot */
#define tal_insert(list, index, new_item)                       \
        do {                                                    \
                assert((size_t)(index) < (list).length + 1);    \
                tal_allocate(list, (list).length + 1);          \
                memmove(                                        \
                        (list).array + (index) + 1,             \
                        (list).array + (index),                 \
                        ((list).length - (index)) *             \
                        sizeof(*(list).array)                   \
                );                                              \
                (list).array[(index)] = new_item;               \
                (list).length += 1;                             \
        } while (0)

/* Remove item at index, shifting the following items back by one spot */
#define tal_remove(list, index)                                 \
        do {                                                    \
                assert((size_t)(index) < (list).length);        \
                memmove(                                        \
                        (list).array + (index),                 \
                        (list).array + (index) + 1,             \
                        ((list).length - (index)) *             \
                        sizeof(*(list).array)                   \
                );                                              \
                (list).length -= 1;                             \
        } while (0)

#define tal_front(list) (list).array[0];

#define tal_back(list) (list).array[(list).length - 1];

// #define tal_get(list, index) (list).array[(index)];
#define tal_get(list, index) __extension__                              \
    ({                                                                  \
        assert((size_t)(index) < (list).length);                        \
        __typeof__(*(list).array) __ret = (list).array[(index)];        \
        __ret;                                                          \
    })

#define tal_set(list, index, item)                              \
        do {                                                    \
                assert((size_t)(index) < (list).length);        \
                (list).array[(index)] = item;                   \
        } while (0)

/* Removes the first element from the list, and returns it */
#define tal_pop_front(list) __extension__                  \
    ({                                                     \
        __typeof__(*(list).array) __ret = (list).array[0]; \
        tal_remove((list), 0);                             \
        __ret;                                             \
    })

/* Same as tll_pop_front(), but returns/removes the *last* element */
#define tal_pop_back(list) __extension__                                 \
    ({                                                                   \
        __typeof__(*(list).array) __ret = (list).array[(list).length-1]; \
        tll_remove((list), (list).length - 1);                           \
        __ret;                                                           \
    })

#define tal_clear(list) (list).length = 0;

/* Frees the list. */
#define tal_destroy(list)               \
        do {                            \
                (list).length = 0;      \
                (list).capicity = 0;    \
                free((list).array);     \
                (list).array = NULL;    \
        } while (0)

