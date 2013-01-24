/*
  
  m2gfxutl.c
  
  common procedures build on the gfx-api

  m2tklib = Mini Interative Interface Toolkit Library
  
  Copyright (C) 2011  olikraus@gmail.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


*/

#include "m2.h"

void m2_gfx_draw_text_add_normal_border_offset(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font, const char *s)
{
  m2_gfx_text(m2_gfx_add_normal_border_x(font, x0), m2_gfx_add_normal_border_y(font, y0), w, h, font, s);
}

void m2_gfx_draw_icon_add_normal_border_offset(uint8_t x0, uint8_t y0, uint8_t font, uint8_t icon_number)
{
  m2_gfx_draw_icon(m2_gfx_add_normal_border_x(font, x0), m2_gfx_add_normal_border_y(font, y0), font, icon_number);
}

void m2_gfx_draw_text_add_small_border_offset(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font, const char *s)
{
  m2_gfx_text(m2_gfx_add_small_border_x(font, x0), m2_gfx_add_small_border_y(font, y0), w, h, font, s);
}

void m2_gfx_draw_text_add_readonly_border_offset(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font, const char *s)
{
  m2_gfx_text(m2_gfx_add_readonly_border_x(font, x0), m2_gfx_add_readonly_border_y(font, y0), w, h, font, s);
}

void m2_gfx_draw_text_p_add_readonly_border_offset(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font, const char *s)
{
  m2_gfx_text_p(m2_gfx_add_readonly_border_x(font, x0), m2_gfx_add_readonly_border_y(font, y0), w, h, font, s);
}

uint8_t m2_gfx_get_char_height_with_small_border(uint8_t font)
{
  return m2_gfx_add_small_border_height(font, m2_gfx_get_char_height(font));
}
uint8_t m2_gfx_get_char_width_with_small_border(uint8_t font)
{
  return m2_gfx_add_small_border_width(font, m2_gfx_get_char_width(font));
}

uint8_t m2_gfx_get_char_height_with_normal_border(uint8_t font)
{
  return m2_gfx_add_normal_border_height(font, m2_gfx_get_char_height(font));
}
uint8_t m2_gfx_get_char_width_with_normal_border(uint8_t font)
{
  return m2_gfx_add_normal_border_width(font, m2_gfx_get_char_width(font));
}
