// Copyright (C) 2013  Manuel Jelinek
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <stdint.h>

#ifndef STDLIB_H
#define STDLIB_H


#define NULL 0

extern uintptr_t _heap_start;  // start address where we can put our data
extern const void* _heap_end;  // in standard configuration the GPU uses the last 64MB of the 512MB RAM


extern void *malloc(uint32_t size);
extern void free(void *ptr);
extern void *realloc(void *ptr, uint32_t size);

#endif /* STDLIB_H */

