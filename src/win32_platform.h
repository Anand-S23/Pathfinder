#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFROM_H 

#include <stdint.h> 

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

typedef i32 b32;

#define internal static
#define global static
#define local_persist static

typedef struct platform
{
    void *permanent_storage;
    u32   permanent_storage_size;
    void *transient_storage;
    u32   transient_storage_size;

    b32 initialized;

    int mouse_x; 
    int mouse_y; 
    b32 left_mouse_down;
    b32 right_mouse_down; 
} platform; 

#endif
