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


#define skip(PTR, N) *PTR += N

#endif // CLASSPARSE_UTILS_H
