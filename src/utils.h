#ifndef CLASSPARSE_UTILS_H
#define CLASSPARSE_UTILS_H

#include <stdint.h>
#include <stddef.h>

#if defined(__GNUC__) || defined(__clang__)
    #define HAS_BUILTIN_BSWAP
#elif defined(_MSC_VER)
    #define HAS_INTRINSIC_BSWAP
#endif

static inline uint16_t swap_u16(uint16_t x) {
#ifdef HAS_BUILTIN_BSWAP
    return __builtin_bswap16(x);
#elif defined(HAS_INTRINSIC_BSWAP)
    return _byteswap_ushort(x);
#else
    return ((x & 0x00FF) << 8) | ((x & 0xFF00) >> 8);
#endif
}

static inline uint32_t swap_u32(uint32_t x) {
#ifdef HAS_BUILTIN_BSWAP
    return __builtin_bswap32(x);
#elif defined(HAS_INTRINSIC_BSWAP)
    return _byteswap_ulong(x);
#else
    return ((x & 0x000000FF) << 24) |
           ((x & 0x0000FF00) << 8)  |
           ((x & 0x00FF0000) >> 8)  |
           ((x & 0xFF000000) >> 24);
#endif
}

static inline uint16_t read_u16_ptr(uint8_t **stream)
{
    uint16_t value = *((uint16_t*)*stream);
    *stream += sizeof(uint16_t);
    return swap_u16(value);
}

static inline uint32_t read_u32_ptr(uint8_t **stream)
{
    uint32_t value = *((uint32_t*)*stream);
    *stream += sizeof(uint32_t);
    return swap_u32(value);
}

#if !defined(__freestanding__) || defined(CLASSPARSE_ALLOW_STRING_H)
#include <string.h>
#define classparse_strcmp strcmp
#else

static inline int classparse_strcmp(const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    while (*p1 && *p1 == *p2) {
        p1++;
        p2++;
    }
    return *p1 - *p2;
}

static char *strchr(const char *s, int c)
{
    do {
        if (*s == (char)c) {

            return (char *)s;
        }
    } while (*s++);
    return NULL;
}

static char *strrchr(const char *s, int c)
{
    const char *last = NULL;
    do {
        if (*s == (char)c) {
            last = s;
        }
    } while (*s++);
    return (char *)last;
}

static size_t strlen(const char *s)
{
    const char *p = s;
    while (*p) {
        p++;
    }
    return p - s;
}

#endif

#define skip(PTR, N) *PTR += N

#endif // CLASSPARSE_UTILS_H
