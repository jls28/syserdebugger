/*---------------------------------------------------------------------------+
 |  reg_ld_str.c                                                             |
 |  $Id: reg_ld_str.c,v 1.4 2001/10/06 03:53:46 bdenney Exp $
 |                                                                           |
 | All of the functions which transfer data between user memory and FPU_REGs.|
 |                                                                           |
 | Copyright (C) 1992,1993,1994,1996,1997                                    |
 |                  W. Metzenthen, 22 Parker St, Ormond, Vic 3163, Australia |
 |                  E-mail   billm@suburbia.net                              |
 |                                                                           |
 |                                                                           |
 +---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
 | Note:                                                                     |
 |    The file contains code which accesses user memory.                     |
 |    Emulator static data may change when user memory is accessed, due to   |
 |    other processes using the emulator while swapping is in progress.      |
 +---------------------------------------------------------------------------*/
#include "fpu_i387.h"
#include "fpu_emu.h"

#include <asm/uaccess.h>

#include "fpu_system.h"
#include "exception.h"
#include "reg_constant.h"
#include "control_w.h"
#include "status_w.h"


#define DOUBLE_Emax 1023         /* largest valid exponent */
#define DOUBLE_Ebias 1023
#define DOUBLE_Emin (-1022)      /* smallest valid exponent */

#define SINGLE_Emax 127          /* largest valid exponent */
#define SINGLE_Ebias 127
#define SINGLE_Emin (-126)       /* smallest valid exponent */


static u_char normalize_no_excep(current_i387_definefunyuan_      FPU_REG*r, int exp, int sign)
{
  u_char tag;

  setexponent16(r, exp);

  tag = FPU_normalize_nuo(current_i387_yuan_   r, 0);
  stdexp(r);
  if ( sign )
    setnegative(r);

  return tag;
}


int FPU_tagof(current_i387_definefunyuan_   FPU_REG *ptr)
{
  int exp;

  exp = exponent16(ptr) & 0x7fff;
  if ( exp == 0 )
    {
      if ( !(ptr->sigh | ptr->sigl) )
	{
	  return TAG_Zero;
	}
      /* The number is a de-normal or pseudodenormal. */
      return TAG_Special;
    }

  if ( exp == 0x7fff )
    {
      /* Is an Infinity, a NaN, or an unsupported data type. */
      return TAG_Special;
    }

  if ( !(ptr->sigh & 0x80000000) )
    {
      /* Unsupported data type. */
      /* Valid numbers have the ms bit set to 1. */
      /* Unnormal. */
      return TAG_Special;
    }

  return TAG_Valid;
}


/* Get a long double from user memory */
int FPU_load_extended(current_i387_definefunyuan_   long double *s, int stnr)
{
  FPU_REG *sti_ptr = &st(stnr);

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_READ, s, 10);
#ifndef USE_WITH_CPU_SIM
  __copy_from_user(sti_ptr, s, 10);
#else
  FPU_get_user(sti_ptr->sigl, (u32*)(((u8*)s)+0));
  FPU_get_user(sti_ptr->sigh, (u32*)(((u8*)s)+4));
  FPU_get_user(sti_ptr->exp,  (u16*)(((u8*)s)+8));
#endif
  RE_ENTRANT_CHECK_ON;

  return FPU_tagof(current_i387_yuan_   sti_ptr);
}


/* Get a double from user memory */
int FPU_load_double(current_i387_definefunyuan_   double *dfloat, FPU_REG *loaded_data)
{
  int exp, tag, negative;
  u32 m64, l64;

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_READ, dfloat, 8);
  FPU_get_user(m64, 1 + (u32 *) dfloat);
  FPU_get_user(l64, (u32 *) dfloat);
  RE_ENTRANT_CHECK_ON;

  negative = (m64 & 0x80000000) ? SIGN_Negative : SIGN_Positive;
  exp = ((m64 & 0x7ff00000) >> 20) - DOUBLE_Ebias + EXTENDED_Ebias;
  m64 &= 0xfffff;
  if ( exp > DOUBLE_Emax + EXTENDED_Ebias )
    {
      /* Infinity or NaN */
      if ((m64 == 0) && (l64 == 0))
	{
	  /* +- infinity */
	  loaded_data->sigh = 0x80000000;
	  loaded_data->sigl = 0x00000000;
	  exp = EXP_Infinity + EXTENDED_Ebias;
	  tag = TAG_Special;
	}
      else
	{
	  /* Must be a signaling or quiet NaN */
	  exp = EXP_NaN + EXTENDED_Ebias;
	  loaded_data->sigh = (m64 << 11) | 0x80000000;
	  loaded_data->sigh |= l64 >> 21;
	  loaded_data->sigl = l64 << 11;
	  tag = TAG_Special;    /* The calling function must look for NaNs */
	}
    }
  else if ( exp < DOUBLE_Emin + EXTENDED_Ebias )
    {
      /* Zero or de-normal */
      if ((m64 == 0) && (l64 == 0))
	{
	  /* Zero */
	  reg_copy(&CONST_Z, loaded_data);
	  exp = 0;
	  tag = TAG_Zero;
	}
      else
	{
	  /* De-normal */
	  loaded_data->sigh = m64 << 11;
	  loaded_data->sigh |= l64 >> 21;
	  loaded_data->sigl = l64 << 11;

	  return normalize_no_excep(current_i387_yuan_      loaded_data, DOUBLE_Emin, negative)
	    | (denormal_operand(current_i387_yuan_le   ) < 0 ? FPU_Exception : 0);
	}
    }
  else
    {
      loaded_data->sigh = (m64 << 11) | 0x80000000;
      loaded_data->sigh |= l64 >> 21;
      loaded_data->sigl = l64 << 11;

      tag = TAG_Valid;
    }

  setexponent16(loaded_data, exp | negative);

  return tag;
}


/* Get a float from user memory */
int FPU_load_single(current_i387_definefunyuan_   float *single, FPU_REG *loaded_data)
{
  u32 m32;
  int exp, tag, negative;

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_READ, single, 4);
  FPU_get_user(m32, (u32 *) single);
  RE_ENTRANT_CHECK_ON;

  negative = (m32 & 0x80000000) ? SIGN_Negative : SIGN_Positive;

  if (!(m32 & 0x7fffffff))
    {
      /* Zero */
      reg_copy(&CONST_Z, loaded_data);
      addexponent(loaded_data, negative);
      return TAG_Zero;
    }
  exp = ((m32 & 0x7f800000) >> 23) - SINGLE_Ebias + EXTENDED_Ebias;
  m32 = (m32 & 0x7fffff) << 8;
  if ( exp < SINGLE_Emin + EXTENDED_Ebias )
    {
      /* De-normals */
      loaded_data->sigh = m32;
      loaded_data->sigl = 0;

      return normalize_no_excep(current_i387_yuan_      loaded_data, SINGLE_Emin, negative)
	| (denormal_operand(current_i387_yuan_le   ) < 0 ? FPU_Exception : 0);
    }
  else if ( exp > SINGLE_Emax + EXTENDED_Ebias )
    {
    /* Infinity or NaN */
      if ( m32 == 0 )
	{
	  /* +- infinity */
	  loaded_data->sigh = 0x80000000;
	  loaded_data->sigl = 0x00000000;
	  exp = EXP_Infinity + EXTENDED_Ebias;
	  tag = TAG_Special;
	}
      else
	{
	  /* Must be a signaling or quiet NaN */
	  exp = EXP_NaN + EXTENDED_Ebias;
	  loaded_data->sigh = m32 | 0x80000000;
	  loaded_data->sigl = 0;
	  tag = TAG_Special;  /* The calling function must look for NaNs */
	}
    }
  else
    {
      loaded_data->sigh = m32 | 0x80000000;
      loaded_data->sigl = 0;
      tag = TAG_Valid;
    }

  setexponent16(loaded_data, exp | negative);  /* Set the sign. */

  return tag;
}


/* Get a 64bit quantity from user memory */
int FPU_load_int64(current_i387_definefunyuan_   s64 *_s)
{
  s64 s;
  int sign;
  FPU_REG *st0_ptr = &st(0);

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area( VERIFY_READ, _s, 8);
#ifndef USE_WITH_CPU_SIM
  copy_from_user(&s,_s,8);
#else
  {
  u32 chunk0, chunk1;
  FPU_get_user(chunk0, (u32*)(((u8*)_s)+0));
  FPU_get_user(chunk1, (u32*)(((u8*)_s)+4));
  s = chunk0;
  s |= (((u64)chunk1) << 32);
  }
#endif
  RE_ENTRANT_CHECK_ON;

  if (s == 0)
    {
      reg_copy(&CONST_Z, st0_ptr);
      return TAG_Zero;
    }

  if (s > 0)
    sign = SIGN_Positive;
  else
  {
    s = -s;
    sign = SIGN_Negative;
  }
	st0_ptr->sigl = (u32)s;
	st0_ptr->sigh = (u32)(s>>32);

 // significand(st0_ptr) = s;

  return normalize_no_excep(current_i387_yuan_      st0_ptr, 63, sign);
}


/* Get a long from user memory */
int FPU_load_int32(current_i387_definefunyuan_   s32 *_s, FPU_REG *loaded_data)
{
  s32 s;
  int negative;

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_READ, _s, 4);
  FPU_get_user(s, _s);
  RE_ENTRANT_CHECK_ON;

  if (s == 0)
    { reg_copy(&CONST_Z, loaded_data); return TAG_Zero; }

  if (s > 0)
    negative = SIGN_Positive;
  else
    {
      s = -s;
      negative = SIGN_Negative;
    }

  loaded_data->sigh = s;
  loaded_data->sigl = 0;

  return normalize_no_excep(current_i387_yuan_      loaded_data, 31, negative);
}


/* Get a short from user memory */
int FPU_load_int16(current_i387_definefunyuan_   s16 *_s, FPU_REG *loaded_data)
{
  int s, negative;

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_READ, _s, 2);
  /* Cast as short to get the sign extended. */
  FPU_get_user(s, _s);
  RE_ENTRANT_CHECK_ON;

  if (s == 0)
    { reg_copy(&CONST_Z, loaded_data); return TAG_Zero; }

  if (s > 0)
    negative = SIGN_Positive;
  else
    {
      s = -s;
      negative = SIGN_Negative;
    }

  loaded_data->sigh = s << 16;
  loaded_data->sigl = 0;

  return normalize_no_excep(current_i387_yuan_      loaded_data, 15, negative);
}


/* Get a packed bcd array from user memory */
int FPU_load_bcd(current_i387_definefunyuan_   u_char *s)
{
  FPU_REG *st0_ptr = &st(0);
  int pos;
  u_char bcd;
  s64 l=0;
  int sign;

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_READ, s, 10);
  RE_ENTRANT_CHECK_ON;
  for ( pos = 8; pos >= 0; pos--)
    {
      l *= 10;
      RE_ENTRANT_CHECK_OFF;
      FPU_get_user(bcd, (u_char *) s+pos);
      RE_ENTRANT_CHECK_ON;
      l += bcd >> 4;
      l *= 10;
      l += bcd & 0x0f;
    }
 
  RE_ENTRANT_CHECK_OFF;
  FPU_get_user(current_i387_yuan_   sign, (u_char *) s+9);
  sign = sign & 0x80 ? SIGN_Negative : SIGN_Positive;
  RE_ENTRANT_CHECK_ON;

  if ( l == 0 )
    {
      reg_copy(&CONST_Z, st0_ptr);
      addexponent(st0_ptr, sign);   /* Set the sign. */
      return TAG_Zero;
    }
  else
    {
      //significand(st0_ptr) = l;
		st0_ptr->sigl = (u32)l;
		st0_ptr->sigh = (u32)(l>>32);
      return normalize_no_excep(current_i387_yuan_      st0_ptr, 63, sign);
    }
}

/*===========================================================================*/

/* Put a long double into user memory */
int FPU_store_extended(current_i387_definefunyuan_   FPU_REG *st0_ptr, u_char st0_tag, long double *d)
{
  /*
    The only exception raised by an attempt to store to an
    extended format is the Invalid Stack exception, i.e.
    attempting to store from an empty register.
   */

  if ( st0_tag != TAG_Empty )
    {
      RE_ENTRANT_CHECK_OFF;
      FPU_verify_area(VERIFY_WRITE, d, 10);

      FPU_put_user(st0_ptr->sigl, (u32 *) d);
      FPU_put_user(st0_ptr->sigh, (u32 *) ((u_char *)d + 4));
      FPU_put_user(exponent16(st0_ptr), (u16 *) ((u_char *)d + 8));
      RE_ENTRANT_CHECK_ON;

      return 1;
    }

  /* Empty register (stack underflow) */
  EXCEPTION(EX_StackUnder);
  if ( control_word & CW_Invalid )
    {
      /* The masked response */
      /* Put out the QNaN indefinite */
      RE_ENTRANT_CHECK_OFF;
      FPU_verify_area(VERIFY_WRITE,d,10);
      FPU_put_user(0, (u32 *) d);
      FPU_put_user(0xc0000000, 1 + (u32 *) d);
      FPU_put_user(0xffff, 4 + (s16 *) d);
      RE_ENTRANT_CHECK_ON;
      return 1;
    }
  else
    return 0;

}


/* Put a double into user memory */
int FPU_store_double(current_i387_definefunyuan_   FPU_REG *st0_ptr, u_char st0_tag, double *dfloat)
{
  u32 l[2];
  u32 increment = 0;	/* avoid gcc warnings */
  int precision_loss;
  int exp;
  FPU_REG tmp;

  if ( st0_tag == TAG_Valid )
    {
      reg_copy(st0_ptr, &tmp);
      exp = exponent(&tmp);

      if ( exp < DOUBLE_Emin )     /* It may be a denormal */
	{
	  addexponent(&tmp, -DOUBLE_Emin + 52);  /* largest exp to be 51 */

	denormal_arg:

	  if ( (precision_loss = FPU_round_to_int(current_i387_yuan_   &tmp, st0_tag)) )
	    {
#ifdef PECULIAR_486
	      /* Did it round to a non-denormal ? */
	      /* This behaviour might be regarded as peculiar, it appears
		 that the 80486 rounds to the dest precision, then
		 converts to decide underflow. */
	      if ( !((tmp.sigh == 0x00100000) && (tmp.sigl == 0) &&
		  (st0_ptr->sigl & 0x000007ff)) )
#endif /* PECULIAR_486 */
		{
		  EXCEPTION(EX_Underflow);
		  /* This is a special case: see sec 16.2.5.1 of
		     the 80486 book */
		  if ( !(control_word & CW_Underflow) )
		    return 0;
		}
	      EXCEPTION(precision_loss);
	      if ( !(control_word & CW_Precision) )
		return 0;
	    }
	  l[0] = tmp.sigl;
	  l[1] = tmp.sigh;
	}
      else
	{
	  if ( tmp.sigl & 0x000007ff )
	    {
	      precision_loss = 1;
	      switch (control_word & CW_RC)
		{
		case RC_RND:
		  /* Rounding can get a little messy.. */
		  increment = ((tmp.sigl & 0x7ff) > 0x400) |  /* nearest */
		    ((tmp.sigl & 0xc00) == 0xc00);            /* odd -> even */
		  break;
		case RC_DOWN:   /* towards -infinity */
		  increment = signpositive(&tmp) ? 0 : tmp.sigl & 0x7ff;
		  break;
		case RC_UP:     /* towards +infinity */
		  increment = signpositive(&tmp) ? tmp.sigl & 0x7ff : 0;
		  break;
		case RC_CHOP:
		  increment = 0;
		  break;
		}
	  
	      /* Truncate the mantissa */
	      tmp.sigl &= 0xfffff800;
	  
	      if ( increment )
		{
		  if ( tmp.sigl >= 0xfffff800 )
		    {
		      /* the sigl part overflows */
		      if ( tmp.sigh == 0xffffffff )
			{
			  /* The sigh part overflows */
			  tmp.sigh = 0x80000000;
			  exp++;
			  if (exp >= EXP_OVER)
			    goto overflow;
			}
		      else
			{
			  tmp.sigh ++;
			}
		      tmp.sigl = 0x00000000;
		    }
		  else
		    {
		      /* We only need to increment sigl */
		      tmp.sigl += 0x00000800;
		    }
		}
	    }
	  else
	    precision_loss = 0;
	  
	  l[0] = (tmp.sigl >> 11) | (tmp.sigh << 21);
	  l[1] = ((tmp.sigh >> 11) & 0xfffff);

	  if ( exp > DOUBLE_Emax )
	    {
	    overflow:
	      EXCEPTION(EX_Overflow);
	      if ( !(control_word & CW_Overflow) )
		return 0;
	      set_precision_flag_up(current_i387_yuan_clono  );
	      if ( !(control_word & CW_Precision) )
		return 0;

	      /* This is a special case: see sec 16.2.5.1 of the 80486 book */
	      /* Overflow to infinity */
	      l[0] = 0x00000000;	/* Set to */
	      l[1] = 0x7ff00000;	/* + INF */
	    }
	  else
	    {
	      if ( precision_loss )
		{
		  if ( increment )
		    set_precision_flag_up(current_i387_yuan_clono  );
		  else
		    set_precision_flag_down(current_i387_yuan_clono  );
		}
	      /* Add the exponent */
	      l[1] |= (((exp+DOUBLE_Ebias) & 0x7ff) << 20);
	    }
	}
    }
  else if (st0_tag == TAG_Zero)
    {
      /* Number is zero */
      l[0] = 0;
      l[1] = 0;
    }
  else if ( st0_tag == TAG_Special )
    {
      st0_tag = FPU_Special(current_i387_yuan_   st0_ptr);
      if ( st0_tag == TW_Denormal )
	{
	  /* A denormal will always underflow. */
#ifndef PECULIAR_486
	  /* An 80486 is supposed to be able to generate
	     a denormal exception here, but... */
	  /* Underflow has priority. */
	  if ( control_word & CW_Underflow )
	    denormal_operand(current_i387_yuan_clono  );
#endif /* PECULIAR_486 */
	  reg_copy(st0_ptr, &tmp);
	  goto denormal_arg;
	}
      else if (st0_tag == TW_Infinity)
	{
	  l[0] = 0;
	  l[1] = 0x7ff00000;
	}
      else if (st0_tag == TW_NaN)
	{
	  /* Is it really a NaN ? */
	  if ( (exponent(st0_ptr) == EXP_OVER)
	       && (st0_ptr->sigh & 0x80000000) )
	    {
	      /* See if we can get a valid NaN from the FPU_REG */
	      l[0] = (st0_ptr->sigl >> 11) | (st0_ptr->sigh << 21);
	      l[1] = ((st0_ptr->sigh >> 11) & 0xfffff);
	      if ( !(st0_ptr->sigh & 0x40000000) )
		{
		  /* It is a signalling NaN */
		  EXCEPTION(EX_Invalid);
		  if ( !(control_word & CW_Invalid) )
		    return 0;
		  l[1] |= (0x40000000 >> 11);
		}
	      l[1] |= 0x7ff00000;
	    }
	  else
	    {
	      /* It is an unsupported data type */
	      EXCEPTION(EX_Invalid);
	      if ( !(control_word & CW_Invalid) )
		return 0;
	      l[0] = 0;
	      l[1] = 0xfff80000;
	    }
	}
    }
  else if ( st0_tag == TAG_Empty )
    {
      /* Empty register (stack underflow) */
      EXCEPTION(EX_StackUnder);
      if ( control_word & CW_Invalid )
	{
	  /* The masked response */
	  /* Put out the QNaN indefinite */
	  RE_ENTRANT_CHECK_OFF;
	  FPU_verify_area(VERIFY_WRITE,(void *)dfloat,8);
	  FPU_put_user(0, (u32 *) dfloat);
	  FPU_put_user(0xfff80000, 1 + (u32 *) dfloat);
	  RE_ENTRANT_CHECK_ON;
	  return 1;
	}
      else
	return 0;
    }
  if ( getsign(st0_ptr) )
    l[1] |= 0x80000000;

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_WRITE,(void *)dfloat,8);
  FPU_put_user(l[0], (u32 *)dfloat);
  FPU_put_user(l[1], 1 + (u32 *)dfloat);
  RE_ENTRANT_CHECK_ON;

  return 1;
}


/* Put a float into user memory */
int FPU_store_single(current_i387_definefunyuan_   FPU_REG *st0_ptr, u_char st0_tag, float *single)
{
  s32 templ;
  u32 increment = 0;     	/* avoid gcc warnings */
  int precision_loss;
  int exp;
  FPU_REG tmp;

  if ( st0_tag == TAG_Valid )
    {

      reg_copy(st0_ptr, &tmp);
      exp = exponent(&tmp);

      if ( exp < SINGLE_Emin )
	{
	  addexponent(&tmp, -SINGLE_Emin + 23);  /* largest exp to be 22 */

	denormal_arg:

	  if ( (precision_loss = FPU_round_to_int(current_i387_yuan_   &tmp, st0_tag)) )
	    {
#ifdef PECULIAR_486
	      /* Did it round to a non-denormal ? */
	      /* This behaviour might be regarded as peculiar, it appears
		 that the 80486 rounds to the dest precision, then
		 converts to decide underflow. */
	      if ( !((tmp.sigl == 0x00800000) &&
		  ((st0_ptr->sigh & 0x000000ff) || st0_ptr->sigl)) )
#endif /* PECULIAR_486 */
		{
		  EXCEPTION(EX_Underflow);
		  /* This is a special case: see sec 16.2.5.1 of
		     the 80486 book */
		  if ( !(control_word & CW_Underflow) )
		    return 0;
		}
	      EXCEPTION(precision_loss);
	      if ( !(control_word & CW_Precision) )
		return 0;
	    }
	  templ = tmp.sigl;
      }
      else
	{
	  if ( tmp.sigl | (tmp.sigh & 0x000000ff) )
	    {
	      u32 sigh = tmp.sigh;
	      u32 sigl = tmp.sigl;
	      
	      precision_loss = 1;
	      switch (control_word & CW_RC)
		{
		case RC_RND:
		  increment = ((sigh & 0xff) > 0x80)       /* more than half */
		    || (((sigh & 0xff) == 0x80) && sigl)   /* more than half */
		    || ((sigh & 0x180) == 0x180);        /* round to even */
		  break;
		case RC_DOWN:   /* towards -infinity */
		  increment = signpositive(&tmp)
		    ? 0 : (sigl | (sigh & 0xff));
		  break;
		case RC_UP:     /* towards +infinity */
		  increment = signpositive(&tmp)
		    ? (sigl | (sigh & 0xff)) : 0;
		  break;
		case RC_CHOP:
		  increment = 0;
		  break;
		}
	  
	      /* Truncate part of the mantissa */
	      tmp.sigl = 0;
	  
	      if (increment)
		{
		  if ( sigh >= 0xffffff00 )
		    {
		      /* The sigh part overflows */
		      tmp.sigh = 0x80000000;
		      exp++;
		      if ( exp >= EXP_OVER )
			goto overflow;
		    }
		  else
		    {
		      tmp.sigh &= 0xffffff00;
		      tmp.sigh += 0x100;
		    }
		}
	      else
		{
		  tmp.sigh &= 0xffffff00;  /* Finish the truncation */
		}
	    }
	  else
	    precision_loss = 0;
      
	  templ = (tmp.sigh >> 8) & 0x007fffff;

	  if ( exp > SINGLE_Emax )
	    {
	    overflow:
	      EXCEPTION(EX_Overflow);
	      if ( !(control_word & CW_Overflow) )
		return 0;
	      set_precision_flag_up(current_i387_yuan_clono  );
	      if ( !(control_word & CW_Precision) )
		return 0;

	      /* This is a special case: see sec 16.2.5.1 of the 80486 book. */
	      /* Masked response is overflow to infinity. */
	      templ = 0x7f800000;
	    }
	  else
	    {
	      if ( precision_loss )
		{
		  if ( increment )
		    set_precision_flag_up(current_i387_yuan_clono  );
		  else
		    set_precision_flag_down(current_i387_yuan_clono  );
		}
	      /* Add the exponent */
	      templ |= ((exp+SINGLE_Ebias) & 0xff) << 23;
	    }
	}
    }
  else if (st0_tag == TAG_Zero)
    {
      templ = 0;
    }
  else if ( st0_tag == TAG_Special )
    {
      st0_tag = FPU_Special(current_i387_yuan_   st0_ptr);
      if (st0_tag == TW_Denormal)
	{
	  reg_copy(st0_ptr, &tmp);

	  /* A denormal will always underflow. */
#ifndef PECULIAR_486
	  /* An 80486 is supposed to be able to generate
	     a denormal exception here, but... */
	  /* Underflow has priority. */
	  if ( control_word & CW_Underflow )
	    denormal_operand(current_i387_yuan_clono  );
#endif /* PECULIAR_486 */
	  goto denormal_arg;
	}
      else if (st0_tag == TW_Infinity)
	{
	  templ = 0x7f800000;
	}
      else if (st0_tag == TW_NaN)
	{
	  /* Is it really a NaN ? */
	  if ( (exponent(st0_ptr) == EXP_OVER) && (st0_ptr->sigh & 0x80000000) )
	    {
	      /* See if we can get a valid NaN from the FPU_REG */
	      templ = st0_ptr->sigh >> 8;
	      if ( !(st0_ptr->sigh & 0x40000000) )
		{
		  /* It is a signalling NaN */
		  EXCEPTION(EX_Invalid);
		  if ( !(control_word & CW_Invalid) )
		    return 0;
		  templ |= (0x40000000 >> 8);
		}
	      templ |= 0x7f800000;
	    }
	  else
	    {
	      /* It is an unsupported data type */
	      EXCEPTION(EX_Invalid);
	      if ( !(control_word & CW_Invalid) )
		return 0;
	      templ = 0xffc00000;
	    }
	}
#ifdef PARANOID
      else
	{
	  EXCEPTION(EX_INTERNAL|0x164);
	  return 0;
	}
#endif
    }
  else if ( st0_tag == TAG_Empty )
    {
      /* Empty register (stack underflow) */
      EXCEPTION(EX_StackUnder);
      if ( control_word & EX_Invalid )
	{
	  /* The masked response */
	  /* Put out the QNaN indefinite */
	  RE_ENTRANT_CHECK_OFF;
	  FPU_verify_area(VERIFY_WRITE,(void *)single,4);
	  FPU_put_user(0xffc00000, (u32 *) single);
	  RE_ENTRANT_CHECK_ON;
	  return 1;
	}
      else
	return 0;
    }
#ifdef PARANOID
  else
    {
      EXCEPTION(EX_INTERNAL|0x163);
      return 0;
    }
#endif
  if ( getsign(st0_ptr) )
    templ |= 0x80000000;

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_WRITE,(void *)single,4);
  FPU_put_user(templ,(u32 *) single);
  RE_ENTRANT_CHECK_ON;

  return 1;
}


/* Put a 64bit quantity into user memory */
int FPU_store_int64(current_i387_definefunyuan_   FPU_REG *st0_ptr, u_char st0_tag, s64 *d)
{
  FPU_REG t;
  s64 tll;
  int precision_loss;

  if ( st0_tag == TAG_Empty )
    {
      /* Empty register (stack underflow) */
      EXCEPTION(EX_StackUnder);
      goto invalid_operand;
    }
  else if ( st0_tag == TAG_Special )
    {
      st0_tag = FPU_Special(current_i387_yuan_   st0_ptr);
      if ( (st0_tag == TW_Infinity) ||
	   (st0_tag == TW_NaN) )
	{
	  EXCEPTION(EX_Invalid);
	  goto invalid_operand;
	}
    }

  reg_copy(st0_ptr, &t);
  precision_loss = FPU_round_to_int(current_i387_yuan_   &t, st0_tag);
#ifndef EMU_BIG_ENDIAN
  ((u32 *)&tll)[0] = t.sigl;
  ((u32 *)&tll)[1] = t.sigh;
#else
  ((u32 *)&tll)[0] = t.sigh;
  ((u32 *)&tll)[1] = t.sigl;
#endif
  if ( (precision_loss == 1) ||
      ((t.sigh & 0x80000000) &&
       !((t.sigh == 0x80000000) && (t.sigl == 0) &&
	 signnegative(&t))) )
    {
      EXCEPTION(EX_Invalid);
      /* This is a special case: see sec 16.2.5.1 of the 80486 book */
    invalid_operand:
      if ( control_word & EX_Invalid )
	{
	  /* Produce something like QNaN "indefinite" */
	  tll = BX_CONST64(0x8000000000000000);
	}
      else
	return 0;
    }
  else
    {
      if ( precision_loss )
	set_precision_flag(current_i387_yuan_   precision_loss);
      if ( signnegative(&t) )
	tll = - tll;
    }

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_WRITE,(void *)d,8);
#ifndef USE_WITH_CPU_SIM
  copy_to_user(d, &tll, 8);
#else
  FPU_put_user((u32) tll,       (u32*)(((u8 *)d)+0));
  FPU_put_user((u32) (tll>>32), (u32*)(((u8 *)d)+4));
#endif
  RE_ENTRANT_CHECK_ON;

  return 1;
}


/* Put a long into user memory */
int FPU_store_int32(current_i387_definefunyuan_   FPU_REG *st0_ptr, u_char st0_tag, s32 *d)
{
  FPU_REG t;
  int precision_loss;

  if ( st0_tag == TAG_Empty )
    {
      /* Empty register (stack underflow) */
      EXCEPTION(EX_StackUnder);
      goto invalid_operand;
    }
  else if ( st0_tag == TAG_Special )
    {
      st0_tag = FPU_Special(current_i387_yuan_   st0_ptr);
      if ( (st0_tag == TW_Infinity) ||
	   (st0_tag == TW_NaN) )
	{
	  EXCEPTION(EX_Invalid);
	  goto invalid_operand;
	}
    }

  reg_copy(st0_ptr, &t);
  precision_loss = FPU_round_to_int(current_i387_yuan_   &t, st0_tag);
  if (t.sigh ||
      ((t.sigl & 0x80000000) &&
       !((t.sigl == 0x80000000) && signnegative(&t))) )
    {
      EXCEPTION(EX_Invalid);
      /* This is a special case: see sec 16.2.5.1 of the 80486 book */
    invalid_operand:
      if ( control_word & EX_Invalid )
	{
	  /* Produce something like QNaN "indefinite" */
	  t.sigl = 0x80000000;
	}
      else
	return 0;
    }
  else
    {
      if ( precision_loss )
	set_precision_flag(current_i387_yuan_   precision_loss);
      if ( signnegative(&t) )
	t.sigl = -(s32)t.sigl;
    }

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_WRITE,d,4);
  FPU_put_user(t.sigl, (u32 *) d);
  RE_ENTRANT_CHECK_ON;

  return 1;
}


/* Put a short into user memory */
int FPU_store_int16(current_i387_definefunyuan_   FPU_REG *st0_ptr, u_char st0_tag, s16 *d)
{
  FPU_REG t;
  int precision_loss;

  if ( st0_tag == TAG_Empty )
    {
      /* Empty register (stack underflow) */
      EXCEPTION(EX_StackUnder);
      goto invalid_operand;
    }
  else if ( st0_tag == TAG_Special )
    {
      st0_tag = FPU_Special(current_i387_yuan_   st0_ptr);
      if ( (st0_tag == TW_Infinity) ||
	   (st0_tag == TW_NaN) )
	{
	  EXCEPTION(EX_Invalid);
	  goto invalid_operand;
	}
    }

  reg_copy(st0_ptr, &t);
  precision_loss = FPU_round_to_int(current_i387_yuan_   &t, st0_tag);
  if (t.sigh ||
      ((t.sigl & 0xffff8000) &&
       !((t.sigl == 0x8000) && signnegative(&t))) )
    {
      EXCEPTION(EX_Invalid);
      /* This is a special case: see sec 16.2.5.1 of the 80486 book */
    invalid_operand:
      if ( control_word & EX_Invalid )
	{
	  /* Produce something like QNaN "indefinite" */
	  t.sigl = 0x8000;
	}
      else
	return 0;
    }
  else
    {
      if ( precision_loss )
	set_precision_flag(current_i387_yuan_   precision_loss);
      if ( signnegative(&t) )
	t.sigl = (0-t.sigl);
    }

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_WRITE,d,2);
  FPU_put_user((s16)t.sigl,(s16 *) d);
  RE_ENTRANT_CHECK_ON;

  return 1;
}


/* Put a packed bcd array into user memory */
int FPU_store_bcd(current_i387_definefunyuan_   FPU_REG *st0_ptr, u_char st0_tag, u_char *d)
{
  FPU_REG t;
  u64 ll;
  u_char b;
  int i, precision_loss;
  u_char sign = (getsign(st0_ptr) == SIGN_NEG) ? 0x80 : 0;

  if ( st0_tag == TAG_Empty )
    {
      /* Empty register (stack underflow) */
      EXCEPTION(EX_StackUnder);
      goto invalid_operand;
    }
  else if ( st0_tag == TAG_Special )
    {
      st0_tag = FPU_Special(current_i387_yuan_   st0_ptr);
      if ( (st0_tag == TW_Infinity) ||
	   (st0_tag == TW_NaN) )
	{
	  EXCEPTION(EX_Invalid);
	  goto invalid_operand;
	}
    }

  reg_copy(st0_ptr, &t);
  precision_loss = FPU_round_to_int(current_i387_yuan_   &t, st0_tag);
  ll = significand(&t);

  /* Check for overflow, by comparing with 999999999999999999 decimal. */
  if ( (t.sigh > 0x0de0b6b3) ||
      ((t.sigh == 0x0de0b6b3) && (t.sigl > 0xa763ffff)) )
    {
      EXCEPTION(EX_Invalid);
      /* This is a special case: see sec 16.2.5.1 of the 80486 book */
    invalid_operand:
      if ( control_word & CW_Invalid )
	{
	  /* Produce the QNaN "indefinite" */
	  RE_ENTRANT_CHECK_OFF;
	  FPU_verify_area(VERIFY_WRITE,d,10);
	  for ( i = 0; i < 7; i++)
	    FPU_put_user(0, (u_char *) d+i); /* These bytes "undefined" */
	  FPU_put_user(0xc0, (u_char *) d+7); /* This byte "undefined" */
	  FPU_put_user(0xff, (u_char *) d+8);
	  FPU_put_user(0xff, (u_char *) d+9);
	  RE_ENTRANT_CHECK_ON;
	  return 1;
	}
      else
	return 0;
    }
  else if ( precision_loss )
    {
      /* Precision loss doesn't stop the data transfer */
      set_precision_flag(current_i387_yuan_   precision_loss);
    }

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_WRITE,d,10);
  RE_ENTRANT_CHECK_ON;
  for ( i = 0; i < 9; i++)
    {
      b = FPU_div_small(current_i387_yuan_   &ll, 10);
      b |= (FPU_div_small(current_i387_yuan_   &ll, 10)) << 4;
      RE_ENTRANT_CHECK_OFF;
      FPU_put_user(b,(u_char *) d+i);
      RE_ENTRANT_CHECK_ON;
    }
  RE_ENTRANT_CHECK_OFF;
  FPU_put_user(sign,(u_char *) d+9);
  RE_ENTRANT_CHECK_ON;

  return 1;
}

/*===========================================================================*/

/* r gets mangled such that sig is int, sign: 
   it is NOT normalized */
/* The return value (in eax) is zero if the result is exact,
   if bits are changed due to rounding, truncation, etc, then
   a non-zero value is returned */
/* Overflow is signalled by a non-zero return value (in eax).
   In the case of overflow, the returned significand always has the
   largest possible value */
int FPU_round_to_int(current_i387_definefunyuan_   FPU_REG *r, u_char tag)
{
  u_char     very_big;
  unsigned eax;

  if (tag == TAG_Zero)
    {
      /* Make sure that zero is returned */
      significand(r) = 0;
      return 0;        /* o.k. */
    }

  if (exponent(r) > 63)
    {
      r->sigl = r->sigh = ~0;      /* The largest representable number */
      return 1;        /* overflow */
    }

#ifndef EMU_BIG_ENDIAN
  eax = FPU_shrxs(current_i387_yuan_   &r->sigl, 63 - exponent(r));
#else
  eax = FPU_shrxs(current_i387_yuan_   &r->sigh, 63 - exponent(r));
#endif
  very_big = !(~(r->sigh) | ~(r->sigl));  /* test for 0xfff...fff */
#define	half_or_more	(eax & 0x80000000)
#define	frac_part	(eax)
#define more_than_half  ((eax & 0x80000001) == 0x80000001)
  switch (control_word & CW_RC)
    {
    case RC_RND:
      if ( more_than_half               	/* nearest */
	  || (half_or_more && (r->sigl & 1)) )	/* odd -> even */
	{
	  if ( very_big ) return 1;        /* overflow */
	  significand(r) ++;
	  return PRECISION_LOST_UP;
	}
      break;
    case RC_DOWN:
      if (frac_part && getsign(r))
	{
	  if ( very_big ) return 1;        /* overflow */
	  significand(r) ++;
	  return PRECISION_LOST_UP;
	}
      break;
    case RC_UP:
      if (frac_part && !getsign(r))
	{
	  if ( very_big ) return 1;        /* overflow */
	  significand(r) ++;
	  return PRECISION_LOST_UP;
	}
      break;
    case RC_CHOP:
      break;
    }

  return eax ? PRECISION_LOST_DOWN : 0;

}

/*===========================================================================*/

u_char *fldenv(current_i387_definefunyuan_   fpu_addr_modes addr_modes, u_char *s)
{
  u16 tag_word = 0;
  u_char tag;
  int i;

  if ( (addr_modes.default_mode == VM86) ||
      ((addr_modes.default_mode == PM16)
      ^ (addr_modes.override.operand_size == OP_SIZE_PREFIX)) )
    {
      RE_ENTRANT_CHECK_OFF;
      FPU_verify_area(VERIFY_READ, s, 0x0e);
      FPU_get_user(control_word, (u16 *) s);
      FPU_get_user(partial_status, (u16 *) (s+2));
      FPU_get_user(tag_word, (u16 *) (s+4));
      FPU_get_user(instruction_address.offset, (u16 *) (s+6));
      FPU_get_user(instruction_address.selector, (u16 *) (s+8));
      FPU_get_user(operand_address.offset, (u16 *) (s+0x0a));
      FPU_get_user(operand_address.selector, (u16 *) (s+0x0c));
      RE_ENTRANT_CHECK_ON;
      s += 0x0e;
      if ( addr_modes.default_mode == VM86 )
	{
	  instruction_address.offset
	    += (instruction_address.selector & 0xf000) << 4;
	  operand_address.offset += (operand_address.selector & 0xf000) << 4;
	}
    }
  else
    {
      RE_ENTRANT_CHECK_OFF;
      FPU_verify_area(VERIFY_READ, s, 0x1c);
      FPU_get_user(control_word, (u16 *) s);
      FPU_get_user(partial_status, (u16 *) (s+4));
      FPU_get_user(tag_word, (u16 *) (s+8));
      FPU_get_user(instruction_address.offset, (u32 *) (s+0x0c));
      FPU_get_user(instruction_address.selector, (u16 *) (s+0x10));
      FPU_get_user(instruction_address.opcode, (u16 *) (s+0x12));
      FPU_get_user(operand_address.offset, (u32 *) (s+0x14));
      FPU_get_user(operand_address.selector, (u32 *) (s+0x18));
      RE_ENTRANT_CHECK_ON;
      s += 0x1c;
    }

#ifdef PECULIAR_486
  control_word &= ~0xe080;
#endif /* PECULIAR_486 */

  top = (partial_status >> SW_Top_Shift) & 7;

  if ( partial_status & ~control_word & CW_Exceptions )
    partial_status |= (SW_Summary | SW_Backward);
  else
    partial_status &= ~(SW_Summary | SW_Backward);

  for ( i = 0; i < 8; i++ )
    {
      tag = tag_word & 3;
      tag_word >>= 2;

      if ( tag == TAG_Empty )
	/* New tag is empty.  Accept it */
	FPU_settag(current_i387_yuan_   i, TAG_Empty);
      else if ( FPU_gettag(current_i387_yuan_   i) == TAG_Empty )
	{
	  /* Old tag is empty and new tag is not empty.  New tag is determined
	     by old reg contents */
	  if ( exponent(&fpu_register(i)) == - EXTENDED_Ebias )
	    {
	      if ( !(fpu_register(i).sigl | fpu_register(i).sigh) )
		FPU_settag(current_i387_yuan_   i, TAG_Zero);
	      else
		FPU_settag(current_i387_yuan_   i, TAG_Special);
	    }
	  else if ( exponent(&fpu_register(i)) == 0x7fff - EXTENDED_Ebias )
	    {
	      FPU_settag(current_i387_yuan_   i, TAG_Special);
	    }
	  else if ( fpu_register(i).sigh & 0x80000000 )
	    FPU_settag(current_i387_yuan_   i, TAG_Valid);
	  else
	    FPU_settag(current_i387_yuan_   i, TAG_Special);   /* An Un-normal */
  	}
      /* Else old tag is not empty and new tag is not empty.  Old tag
	 remains correct */
    }

  return s;
}


void frstor(current_i387_definefunyuan_   fpu_addr_modes addr_modes, u_char *data_address)
{
  int i, regnr;
  u_char *s = fldenv(current_i387_yuan_   addr_modes, data_address);
  int offset = (top & 7) * sizeof(FPU_REG), other = 8*sizeof(FPU_REG) - offset;

  /* Copy all registers in stack order. */
  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_READ,s,80);
#ifndef USE_WITH_CPU_SIM
  __copy_from_user(register_base+offset, s, other);
  if ( offset )
    __copy_from_user(register_base, s+other, offset);
#else
  {
  FPU_REG *fpu_reg_p;

  fpu_reg_p = (FPU_REG *) (register_base+offset);
  while (other>0) {
    FPU_get_user(fpu_reg_p->sigl, (u32*)(s+0));
    FPU_get_user(fpu_reg_p->sigh, (u32*)(s+4));
    FPU_get_user(fpu_reg_p->exp,  (u16*)(s+8));
    fpu_reg_p++;
    s += 10;
    other -= sizeof(FPU_REG);
    }
  fpu_reg_p = (FPU_REG *) register_base;
  while (offset>0) {
    FPU_get_user(fpu_reg_p->sigl, (u32*)(s+0));
    FPU_get_user(fpu_reg_p->sigh, (u32*)(s+4));
    FPU_get_user(fpu_reg_p->exp,  (u16*)(s+8));
    fpu_reg_p++;
    s += 10;
    offset -= sizeof(FPU_REG);
    }
  }
#endif
  RE_ENTRANT_CHECK_ON;

  for ( i = 0; i < 8; i++ )
    {
      regnr = (i+top) & 7;
      if ( FPU_gettag(current_i387_yuan_   regnr) != TAG_Empty )
	/* The loaded data over-rides all other cases. */
	FPU_settag(current_i387_yuan_   regnr, FPU_tagof(current_i387_yuan_   &st(i)));
    }

}


u_char *fstenv(current_i387_definefunyuan_   fpu_addr_modes addr_modes, u_char *d)
{
  if ( (addr_modes.default_mode == VM86) ||
      ((addr_modes.default_mode == PM16)
      ^ (addr_modes.override.operand_size == OP_SIZE_PREFIX)) )
    {
      RE_ENTRANT_CHECK_OFF;
      FPU_verify_area(VERIFY_WRITE,d,14);
#ifdef PECULIAR_486
      FPU_put_user(control_word & ~0xe080, (u32 *) d);
#else
      FPU_put_user(control_word, (u16 *) d);
#endif /* PECULIAR_486 */
      FPU_put_user(status_word(), (u16 *) (d+2));
      FPU_put_user(fpu_tag_word, (u16 *) (d+4));
      FPU_put_user(instruction_address.offset, (u16 *) (d+6));
      FPU_put_user(operand_address.offset, (u16 *) (d+0x0a));
      if ( addr_modes.default_mode == VM86 )
	{
	  FPU_put_user((instruction_address.offset & 0xf0000) >> 4,
		      (u16 *) (d+8));
	  FPU_put_user((operand_address.offset & 0xf0000) >> 4,
		      (u16 *) (d+0x0c));
	}
      else
	{
	  FPU_put_user(instruction_address.selector, (u16 *) (d+8));
	  FPU_put_user(operand_address.selector, (u16 *) (d+0x0c));
	}
      RE_ENTRANT_CHECK_ON;
      d += 0x0e;
    }
  else
    {
      RE_ENTRANT_CHECK_OFF;
      FPU_verify_area(VERIFY_WRITE, d, 7*4);
#ifdef PECULIAR_486
      control_word &= ~0xe080;
      /* An 80486 sets nearly all of the reserved bits to 1. */
      control_word |= 0xffff0040;
      partial_status = status_word() | 0xffff0000;
      fpu_tag_word |= 0xffff0000;
      I387.soft.fcs &= ~0xf8000000;
      I387.soft.fos |= 0xffff0000;
#endif /* PECULIAR_486 */
#ifndef USE_WITH_CPU_SIM
      __copy_to_user(d, &control_word, 7*4);
#else
      FPU_put_user((u32) I387.soft.cwd, (u32*)(((u8 *)d)+0));
      FPU_put_user((u32) I387.soft.swd, (u32*)(((u8 *)d)+4));
      FPU_put_user((u32) I387.soft.twd, (u32*)(((u8 *)d)+8));
      FPU_put_user((u32) I387.soft.fip, (u32*)(((u8 *)d)+12));
      FPU_put_user((u32) I387.soft.fcs, (u32*)(((u8 *)d)+16));
      FPU_put_user((u32) I387.soft.foo, (u32*)(((u8 *)d)+20));
      FPU_put_user((u32) I387.soft.fos, (u32*)(((u8 *)d)+24));
#endif
      RE_ENTRANT_CHECK_ON;
      d += 0x1c;
    }
  
  control_word |= CW_Exceptions;
  partial_status &= ~(SW_Summary | SW_Backward);

  return d;
}


void fsave(current_i387_definefunyuan_   fpu_addr_modes addr_modes, u_char *data_address)
{
  u_char *d;
  int offset = (top & 7) * sizeof(FPU_REG), other = 8*sizeof(FPU_REG) - offset;

  d = fstenv(current_i387_yuan_   addr_modes, data_address);

  RE_ENTRANT_CHECK_OFF;
  FPU_verify_area(VERIFY_WRITE,d,80);

  /* Copy all registers in stack order. */
#ifndef USE_WITH_CPU_SIM
  __copy_to_user(d, register_base+offset, other);
  if ( offset )
    __copy_to_user(d+other, register_base, offset);
#else
  {
  FPU_REG *fpu_reg_p;

  fpu_reg_p = (FPU_REG *) (register_base+offset);
  while (other>0) {
    FPU_put_user(fpu_reg_p->sigl, (u32*)(d+0));
    FPU_put_user(fpu_reg_p->sigh, (u32*)(d+4));
    FPU_put_user(fpu_reg_p->exp,  (u16*)(d+8));
    fpu_reg_p++;
    d += 10;
    other -= sizeof(FPU_REG);
    }
  fpu_reg_p = (FPU_REG *) register_base;
  while (offset>0) {
    FPU_put_user(fpu_reg_p->sigl, (u32*)(d+0));
    FPU_put_user(fpu_reg_p->sigh, (u32*)(d+4));
    FPU_put_user(fpu_reg_p->exp,  (u16*)(d+8));
    fpu_reg_p++;
    d += 10;
    offset -= sizeof(FPU_REG);
    }
  }
#endif
  RE_ENTRANT_CHECK_ON;

  finit(current_i387_yuan_clono  );
}

/*===========================================================================*/
