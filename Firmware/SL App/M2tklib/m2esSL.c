/*

  mm2esSL.c

  event source handler for SL environment


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

#include "utility/m2.h"

#include "p24FJ256GA106.h"

#define ENC_SW_I                (PORTBbits.RB11)
#define M2_KEY_SELECT_I ENC_SW_I
#define M2_KEY_EXIT_O (((unsigned char*)&NVMKEY)[1])
#define M2_KEY_NEXT_O (((unsigned char*)&NVMKEY)[1])
#define M2_KEY_PREV_O (((unsigned char*)&NVMKEY)[1])


static void m2_SL_setup_key(uint8_t key) M2_NOINLINE;
static void m2_SL_setup(void) M2_NOINLINE;
static uint8_t m2_SL_check_key(uint8_t key) M2_NOINLINE;
static uint8_t m2_SL_get_key(void) M2_NOINLINE;

static void m2_SL_setup_key(uint8_t key)
{
/*
  uint8_t pin;
  if ( m2_IsPinAssigned(key) != 0 )
  {
    pin = m2_GetPin(key);
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
  }
*/
}

static void m2_SL_setup(void)
{
  m2_SL_setup_key(M2_KEY_SELECT);
  m2_SL_setup_key(M2_KEY_EXIT);
  m2_SL_setup_key(M2_KEY_NEXT);
  m2_SL_setup_key(M2_KEY_PREV);
}

static uint8_t m2_SL_check_key(uint8_t key)
{
    uint8_t pin;
    switch(key){
        case M2_KEY_SELECT:
            pin = M2_KEY_SELECT_I;
            break;
/*
        case M2_KEY_EXIT:
            pin = M2_KEY_EXIT_O;
            break;
        case M2_KEY_NEXT:
            pin = M2_KEY_NEXT_O;
            break;
        case M2_KEY_PREV:
            pin = M2_KEY_PREV_O;
            break;
*/
    }
    if (pin == 0)
        return 1;
    else
        return 0;
}

static uint8_t m2_SL_get_key(void)
{
  if ( m2_SL_check_key(M2_KEY_SELECT) ) return M2_KEY_SELECT;
  if ( m2_SL_check_key(M2_KEY_EXIT) ) return M2_KEY_EXIT;
  if ( m2_SL_check_key(M2_KEY_NEXT) ) return M2_KEY_NEXT;
  if ( m2_SL_check_key(M2_KEY_PREV) ) return M2_KEY_PREV;
  return M2_KEY_NONE;
}

uint8_t m2_es_SL(m2_p ep, uint8_t msg)
{

  switch(msg)
  {
    case M2_ES_MSG_GET_KEY:
      return m2_SL_get_key();
    case M2_ES_MSG_INIT:
      m2_SL_setup();
      return 0;
  }
  return 0;
}

