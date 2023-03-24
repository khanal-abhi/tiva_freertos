#include <stddef.h>
#include <stdint.h>

void *memcpy(void *dst, void *src, size_t len)
{
    uint8_t *_dst = (uint8_t *)dst;
    uint8_t *_src = (uint8_t *)src;
    while (len--)
    {
        *(_src++) = *(_dst++);
    }
    return NULL;
}

void *memset(void *dst, int c, size_t len)
{
    uint8_t _c = c & 0xff;
    uint8_t *_dst = (uint8_t *)dst;
    while (len--)
    {
        *(_dst++) = _c;
    }
    return NULL;
}