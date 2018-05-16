// Copyright (C) 2013       Hykem <hykem@hotmail.com>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/gpl-2.0.txt

#include "ps4.h"
#include "pkg.h"

#define EOF '\00'

// Helper functions.
static inline uint16_t bswap_16(uint16_t val)
{
  return ((val & (uint16_t)0x00ffU) << 8)
    | ((val & (uint16_t)0xff00U) >> 8);
}

static inline uint32_t bswap_32(uint32_t val)
{
  return ((val & (uint32_t)0x000000ffUL) << 24)
    | ((val & (uint32_t)0x0000ff00UL) <<  8)
    | ((val & (uint32_t)0x00ff0000UL) >>  8)
    | ((val & (uint32_t)0xff000000UL) >> 24);
}

int isfpkg(char *pkgfn) {
    int result = 0;

    FILE *in = NULL;
    struct cnt_pkg_main_header m_header;
    struct cnt_pkg_content_header c_header;
    memset(&m_header, 0, sizeof(struct cnt_pkg_main_header));
    memset(&c_header, 0, sizeof(struct cnt_pkg_content_header));

    if ((in = fopen(pkgfn, "rb")) == NULL)
    {
        result = 1;
        goto exit;
    }

    fseek(in, 0, SEEK_SET);
    fread(&m_header, 1,  0x180, in);

    if (m_header.magic != PS4_PKG_MAGIC)
    {
        result = 2;
        goto exit;
    }

    if (bswap_32(m_header.type) != 1)
    {
        result = 3;
        goto exit;
    }

    exit:
    fclose(in);
    return result;
}
