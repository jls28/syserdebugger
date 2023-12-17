/*
 * Unicode string manipulation functions
 *
 * Copyright 2000 Alexandre Julliard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "StdAfx.h"
//#include <limits.h>

#include "unicode.h"

int strcmpiW( const WCHAR *str1, const WCHAR *str2 )
{
    for (;;)
    {
        int ret = toupperW(*str1) - toupperW(*str2);
        if (ret || !*str1) return ret;
        str1++;
        str2++;
    }
}

int strncmpiW( const WCHAR *str1, const WCHAR *str2, int n )
{
    int ret = 0;
    for ( ; n > 0; n--, str1++, str2++)
        if ((ret = toupperW(*str1) - toupperW(*str2)) || !*str1) break;
    return ret;
}

WCHAR *strstrW( const WCHAR *str, const WCHAR *sub )
{
    while (*str)
    {
        const WCHAR *p1 = str, *p2 = sub;
        while (*p1 && *p2 && *p1 == *p2) { p1++; p2++; }
        if (!*p2) return (WCHAR *)str;
        str++;
    }
    return NULL;
}

/* strtolW and strtoulW implementation based on the GNU C library code */
/* Copyright (C) 1991,92,94,95,96,97,98,99,2000,2001 Free Software Foundation, Inc. */

long int strtolW( const WCHAR *nptr, WCHAR **endptr, int base )
{
  int negative;
  register unsigned long int cutoff;
  register unsigned int cutlim;
  register unsigned long int i;
  register const WCHAR *s;
  register WCHAR c;
  const WCHAR *save, *end;
  int overflow;

  if (base < 0 || base == 1 || base > 36) return 0;

  save = s = nptr;

  /* Skip white space.  */
  while (isspaceW (*s))
    ++s;
  if (!*s) goto noconv;

  /* Check for a sign.  */
  negative = 0;
  if (*s == '-')
    {
      negative = 1;
      ++s;
    }
  else if (*s == '+')
    ++s;

  /* Recognize number prefix and if BASE is zero, figure it out ourselves.  */
  if (*s == '0')
    {
      if ((base == 0 || base == 16) && toupperW(s[1]) == 'X')
	{
	  s += 2;
	  base = 16;
	}
      else if (base == 0)
	base = 8;
    }
  else if (base == 0)
    base = 10;

  /* Save the pointer so we can check later if anything happened.  */
  save = s;
  end = NULL;

  cutoff = ULONG_MAX / (unsigned long int) base;
  cutlim = ULONG_MAX % (unsigned long int) base;

  overflow = 0;
  i = 0;
  c = *s;
  for (;c != '\0'; c = *++s)
  {
      if (s == end)
          break;
      if (c >= '0' && c <= '9')
          c -= '0';
      else if (isalphaW (c))
          c = toupperW (c) - 'A' + 10;
      else
          break;
      if ((int) c >= base)
          break;
      /* Check for overflow.  */
      if (i > cutoff || (i == cutoff && c > cutlim))
          overflow = 1;
      else
      {
          i *= (unsigned long int) base;
          i += c;
      }
  }

  /* Check if anything actually happened.  */
  if (s == save)
    goto noconv;

  /* Store in ENDPTR the address of one character
     past the last character we converted.  */
  if (endptr != NULL)
    *endptr = (WCHAR *)s;

  /* Check for a value that is within the range of
     `unsigned LONG int', but outside the range of `LONG int'.  */
  if (overflow == 0
      && i > (negative
	      ?1 -((unsigned long int) (LONG_MIN + 1))
	      : (unsigned long int) LONG_MAX))
    overflow = 1;

  if (overflow)
    {
      return negative ? LONG_MIN : LONG_MAX;
    }

  /* Return the result of the appropriate sign.  */
  return negative ? 0-i : i;

noconv:
  /* We must handle a special case here: the base is 0 or 16 and the
     first two characters are '0' and 'x', but the rest are no
     hexadecimal digits.  This is no error case.  We return 0 and
     ENDPTR points to the `x`.  */
  if (endptr != NULL)
    {
      if (save - nptr >= 2 && toupperW (save[-1]) == 'X'
	  && save[-2] == '0')
	*endptr = (WCHAR *)&save[-1];
      else
	/*  There was no number to convert.  */
	*endptr = (WCHAR *)nptr;
    }

  return 0L;
}


unsigned long int strtoulW( const WCHAR *nptr, WCHAR **endptr, int base )
{
  int negative;
  register unsigned long int cutoff;
  register unsigned int cutlim;
  register unsigned long int i;
  register const WCHAR *s;
  register WCHAR c;
  const WCHAR *save, *end;
  int overflow;

  if (base < 0 || base == 1 || base > 36) return 0;

  save = s = nptr;

  /* Skip white space.  */
  while (isspaceW (*s))
    ++s;
  if (!*s) goto noconv;

  /* Check for a sign.  */
  negative = 0;
  if (*s == '-')
    {
      negative = 1;
      ++s;
    }
  else if (*s == '+')
    ++s;

  /* Recognize number prefix and if BASE is zero, figure it out ourselves.  */
  if (*s == '0')
    {
      if ((base == 0 || base == 16) && toupperW(s[1]) == 'X')
	{
	  s += 2;
	  base = 16;
	}
      else if (base == 0)
	base = 8;
    }
  else if (base == 0)
    base = 10;

  /* Save the pointer so we can check later if anything happened.  */
  save = s;
  end = NULL;

  cutoff = ULONG_MAX / (unsigned long int) base;
  cutlim = ULONG_MAX % (unsigned long int) base;

  overflow = 0;
  i = 0;
  c = *s;
  for (;c != '\0'; c = *++s)
  {
      if (s == end)
          break;
      if (c >= '0' && c <= '9')
          c -= '0';
      else if (isalphaW (c))
          c = toupperW (c) - 'A' + 10;
      else
          break;
      if ((int) c >= base)
          break;
      /* Check for overflow.  */
      if (i > cutoff || (i == cutoff && c > cutlim))
          overflow = 1;
      else
      {
          i *= (unsigned long int) base;
          i += c;
      }
  }

  /* Check if anything actually happened.  */
  if (s == save)
    goto noconv;

  /* Store in ENDPTR the address of one character
     past the last character we converted.  */
  if (endptr != NULL)
    *endptr = (WCHAR *)s;

  if (overflow)
    {
      return ULONG_MAX;
    }

  /* Return the result of the appropriate sign.  */
  return negative ? 0-i : i;

noconv:
  /* We must handle a special case here: the base is 0 or 16 and the
     first two characters are '0' and 'x', but the rest are no
     hexadecimal digits.  This is no error case.  We return 0 and
     ENDPTR points to the `x`.  */
  if (endptr != NULL)
    {
      if (save - nptr >= 2 && toupperW (save[-1]) == 'X'
	  && save[-2] == '0')
	*endptr = (WCHAR *)&save[-1];
      else
	/*  There was no number to convert.  */
	*endptr = (WCHAR *)nptr;
    }

  return 0L;
}
