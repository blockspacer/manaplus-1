/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2017  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTILS_BUILDHEX_H
#define UTILS_BUILDHEX_H

#ifdef USE_SDL2
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#endif  // USE_SDL2
#include <SDL_stdinc.h>
_SDL_stdinc_h
#ifdef USE_SDL2
#pragma GCC diagnostic pop
#endif  // USE_SDL2

#ifndef SDL_BIG_ENDIAN
#include <SDL_endian.h>
#endif  // SDL_BYTEORDER

#include "localconsts.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define buildHex(a, b, c, d) \
    (d) * 16777216U + (c) * 65536U + (b) * 256U + CAST_U32(a)
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN
#define buildHex(a, b, c, d) \
    (a) * 16777216U + (b) * 65536U + (c) * 256U + CAST_U32(d)
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

#define buildHexOgl(a, b, c, d) \
    (a) * 16777216U + (b) * 65536U + (c) * 256U + CAST_U32(d)

#endif  // UTILS_BUILDHEX_H