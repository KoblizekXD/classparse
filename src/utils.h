#ifndef CLASSPARSE_UTILS_H
#define CLASSPARSE_UTILS_H

#include <stdint.h>
#include <stddef.h>

#ifndef INPUT_IS_LITTLE_ENDIAN
#define INPUT_IS_LITTLE_ENDIAN 0
#endif

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define HOST_IS_LITTLE_ENDIAN 1
#else
    #define HOST_IS_LITTLE_ENDIAN 0
#endif

#define SWAP16(x) ((uint16_t)( \
    (((x) & 0x00FFU) << 8) | \
    (((x) & 0xFF00U) >> 8)))

#define SWAP32(x) ((uint32_t)( \
    (((x) & 0x000000FFU) << 24) | \
    (((x) & 0x0000FF00U) << 8)  | \
    (((x) & 0x00FF0000U) >> 8)  | \
    (((x) & 0xFF000000U) >> 24)))

#define SWAP64(x) ((uint64_t)( \
    (((x) & 0x00000000000000FFULL) << 56) | \
    (((x) & 0x000000000000FF00ULL) << 40) | \
    (((x) & 0x0000000000FF0000ULL) << 24) | \
    (((x) & 0x00000000FF000000ULL) << 8)  | \
    (((x) & 0x000000FF00000000ULL) >> 8)  | \
    (((x) & 0x0000FF0000000000ULL) >> 24) | \
    (((x) & 0x00FF000000000000ULL) >> 40) | \
    (((x) & 0xFF00000000000000ULL) >> 56)))

#if INPUT_IS_LITTLE_ENDIAN == HOST_IS_LITTLE_ENDIAN
    #define TO_HOST_16(x) (x)
    #define TO_HOST_32(x) (x)
    #define TO_HOST_64(x) (x)
#else
    #define TO_HOST_16(x) SWAP16(x)
    #define TO_HOST_32(x) SWAP32(x)
    #define TO_HOST_64(x) SWAP64(x)
#endif


static void *classparse_memcpy(void* dest, const void* src, size_t n)
{
    char* d = (char*)dest;
    const char* s = (const char*)src;
    
    if (dest == NULL || src == NULL) {
        return dest;
    }
    
    if (d > s && d < s + n) {
        for (size_t i = n; i > 0; i--) {
            d[i - 1] = s[i - 1];
        }
    } else {
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    }
    
    return dest;
}

static int classparse_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

static inline uint16_t read_u16_ptr(uint8_t **stream)
{
    uint16_t val;
    classparse_memcpy(&val, *stream, sizeof(uint16_t));
    *stream += sizeof(uint16_t);
    return TO_HOST_16(val);
}

static inline uint32_t read_u32_ptr(uint8_t **stream) 
{
    uint32_t val;
    classparse_memcpy(&val, *stream, sizeof(uint32_t));
    *stream += sizeof(uint32_t);
    return TO_HOST_32(val);
}

static inline uint64_t read_u64_ptr(uint8_t **stream)
{
    uint64_t val;
    classparse_memcpy(&val, *stream, sizeof(uint64_t));
    *stream += sizeof(uint64_t);
    return TO_HOST_64(val);
}

#define skip(PTR, N) *PTR += N

#endif // CLASSPARSE_UTILS_H
