#pragma once

#include "../common/precomp.h"

constexpr COLORREF ARGB(const BYTE a, const BYTE r, const BYTE g,
                        const BYTE b) noexcept {
  return (a << 24) | (b << 16) | (g << 8) | (r);
}

#define RGB(r, g, b) (ARGB(255, (r), (g), (b)))

#define LOBYTE(w) ((BYTE)((DWORD)(w)&0xFF))
