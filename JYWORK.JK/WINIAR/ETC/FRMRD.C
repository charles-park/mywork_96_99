/*			- FRMRD.C -

   Common formatter to "sscanf" and "scanf".

   It conforms to the ANSI specifications with the following exceptions:
   -------------------------------------------------------------
   1.	No overflow checking for integer types
   -------------------------------------------------------------
   2.	"scanf" reads complete lines which means that
	"getchar" and "scanf" cannot be combined (e.g.
	first reading a double with "scanf" and then
	read something else on that line with "getchar").
   -------------------------------------------------------------
	Should be compiled with:
	  -DFLOAT_SUPPORT		Full formatter
	  -DNO_FLOATS			All but floats
	  -DREDUCED_SUPPORT		Basic 'int' type of converter

   Version: 
    3.00	[IANR+IMIO] 
    3.00A	[ICLM] return value was wrong with %[]. Bugentry 1017
*/

#include "stdarg.h"
#include "stdio.h"
#include "ctype.h"
#include "limits.h"
#include "float.h"
#include "icclbutl.h"	/* Contains low-level declarations */


#ifdef FLOAT_SUPPORT
#define WANT_LONG_TYPES
#define WANT_FLOAT_TYPES
#else
#ifdef NO_FLOATS
#define WANT_LONG_TYPES
#else
#ifndef REDUCED_SUPPORT
#error -DFLOAT_SUPPORT, -DNO_FLOATS or -DREDUCED_SUPPORT missing
#endif
#endif
#endif


#define ILL_BASE		100	/* ILL_BASE indicator		*/
#define MAX_FIELD_ALLOWED	512	/* Limit to one conversion	*/


static int get_num_and_scan (const char **ptr, int *wptr)
  {
    int n = 0;

    while (isdigit (**ptr) && *wptr)
      {
	if ((n = 10 * n + *(*ptr)++ - '0') > 1000)
	  {
	    n = 1000;
	  }
        (*wptr)--;
      }
    return (n);
  }


static char get_non_space (const char **strptr)
  {
    char c;

    c = **strptr;
    while (isspace (c))
      {
        c = *++(*strptr);
      }
    return (c);
  }


static char get_num_base (char c, char b)
  {
    c = toupper (c);

    if ('A' <= c && c <= 'Z')
      c = (c - 'A') + 10;
    else if (isdigit (c) )
      c -= '0';	
    else
      return (ILL_BASE);
    if (c >= b)
      return (ILL_BASE);
    return (c);
  }


static char get_sign_etc (const char **str_ptr, char *sign_ptr, int *wptr)
  {
    char c;

    if ((c = get_non_space (str_ptr)) == '+' || c == '-')
      {
	if (*wptr)
	  {
	    (*wptr)--;
	    if (c == '-')
	      {
	        *sign_ptr = 1;
	      }
	    c = *++(*str_ptr);
          }
      }
    return (c);
  }


#ifndef REDUCED_SUPPORT
static char get_valid_char (char ch, char standard_compare, const char *str) 
  {
    do
      {
	if (ch == *str)
	  {
	    return (standard_compare);
	  }
      }
    while (*++str && *str != ']');
    return (!standard_compare);
  }
#endif


#ifdef FLOAT_SUPPORT
static double pow10 (int i)
  {
    double factor, temp;

    factor = 10;
    temp = 1;
    while (i)
      {
	if (i & 1)
	  {
	    temp *= factor;
	  }
        if (i >>= 1)
	  {
	    factor *= factor;
          }
      }
    return (temp);
  }
#endif


/*======================================================================*/
/*									*/
/*   	Formatter for [scanf] function. Reads data from "line".		*/
/*									*/
/*	Returns: the number of parameters successfully scanned. 	*/
/*									*/
/*======================================================================*/

int _formatted_read (const char **line, const char **format, va_list ap)
  {
    char assign_flag, c, sign_flag, *temp_string_ptr;
    int	 num_base, assigned_items, i, field_width, item_flag;

#ifdef  WANT_LONG_TYPES
    char long_flag;
    unsigned long value;
#else
    unsigned int value;
#endif

#ifndef REDUCED_SUPPORT
    const char *save_line = *line;
#endif

#ifdef  FLOAT_SUPPORT
    long double fvalue, factor;
    char exp_sign;
    int main_digs;
#endif

    assigned_items = 0;

    while (c = *(*format))
      {
        if (isspace (c))
	  {
	    while (isspace (**line))
	      {
	        (*line)++;
	      }
	    (*format)++;
	    continue; 
	  }

	if (!**line)
	  {
	    goto BAD_CONVERSION;
	  }
	  
        if (c != '%')
	  {
	    (*format)++;
	    if (c != get_non_space (line))
	      {
	        break;
	      }
	    else
	      {
	        (*line)++;
	      }
            continue;
	  }

/*======================================================================*/
/*      Process a conversion specification				*/
/*======================================================================*/
        sign_flag = 0;		

        if ((c = *++(*format)) == '*')
	  {
	    c = *++(*format);
	    assign_flag = 0;
	  }
	else
	  {
            assign_flag = 1;
	  }
        if (isdigit (c))
	  {
	    i = MAX_FIELD_ALLOWED;
	    field_width = get_num_and_scan (format,&i);
	    c = **format;
	  }
	else
	  {
            field_width = MAX_FIELD_ALLOWED;
	  }

#ifndef  REDUCED_SUPPORT
        if (c == 'l' || c == 'L')
	  {
	    long_flag = 1;
	    c = *++(*format);
	  }
        else
          {
	    if (c == 'h')
	      {
	        c = *++(*format);
	      }
	    long_flag = 0;
	  }
#endif

	(*format)++;
        switch (c)
          {

#ifdef  FLOAT_SUPPORT
/*======================================================================*/
/* 	  Convert a double						*/
/*======================================================================*/
	    case 'f':
	    case 'g':
	    case 'e':
	    case 'G':
	    case 'E':
	      c = get_sign_etc (line, &sign_flag, &field_width);

	      fvalue = 0;
	      i = field_width;
	      main_digs = -1;

	      while (field_width && c == '0')
	        {
		  field_width--;
		  c = *++(*line);
		}
		
	      while (field_width && isdigit (c))
	        {
		  if (sizeof(float) == sizeof(double))
		    {
		      if (++main_digs == FLT_MAX_10_EXP)
		        {
			  goto BAD_CONVERSION;
			}
		    }
		  if (sizeof(float) != sizeof(double))
		    {
		      if (++main_digs == (long_flag ?
		      			DBL_MAX_10_EXP : FLT_MAX_10_EXP))
		        {
			  goto BAD_CONVERSION;
			}
		    }
		  field_width--;
		  fvalue = fvalue * 10 + (c - '0');
		  c = *++(*line);
		}
		
	      if (c == '.' && field_width)
	        {
		  factor = 1;
		  field_width--;
		  i--;
		  while (field_width && isdigit (c = *++(*line)))
		    {
		      field_width--;
		      if ((fvalue += (c - '0') * (factor *= 0.1)) &&
		      	  !factor)
		        {
			  goto BAD_CONVERSION;
			}
		    }
		}

	      if (i == field_width)
	        {
		  goto BAD_CONVERSION;
		}

	      if (field_width-- && tolower(c) == 'e')
		{
		  (*line)++;
		  exp_sign = 0;
		  if (!isdigit (get_sign_etc (line, &exp_sign, &field_width)))
		    {
		      goto BAD_CONVERSION;
		    }
		  i = get_num_and_scan (line, &field_width);
		  if (exp_sign)
		    {
		      factor = fvalue;
		      if (i > DBL_MAX_10_EXP)
		        {
			  goto BAD_CONVERSION;
			}
		      fvalue /= pow10 (i);
		      if (factor && !fvalue)
		        {
			  goto BAD_CONVERSION;
			}
		      if (sizeof(float) != sizeof(double) &&
		          !long_flag && fvalue < FLT_MIN)
		        {
			  goto BAD_CONVERSION;
			}
		    }
		  else
		    {
		      if (sizeof(float)==sizeof(double))
		        {
			  factor = FLT_MAX;
			  num_base = FLT_MAX_10_EXP;
			}
		      if (sizeof(float)!=sizeof(double))
		        {
			  factor = long_flag ? DBL_MAX : FLT_MAX;
			  num_base = long_flag ?
				  DBL_MAX_10_EXP : FLT_MAX_10_EXP;
			}
		      if ((main_digs + i) > num_base)
		        {
			  goto BAD_CONVERSION;
	   	        }
		      if ((main_digs + i) == num_base)
		        {
			  if (fvalue > (factor / pow10(num_base - main_digs)))
			    {
			      goto BAD_CONVERSION;
			    }
		        }
		      fvalue *= pow10 (i);
		    }
		}

	      if (assign_flag)
		{
		  if (sizeof (float) != sizeof (double))
		    {
		      if (long_flag)
		        {
		          *va_arg (ap, double *) = sign_flag ?
			  			    - fvalue : fvalue;
		        }
		      else
		        {
		          *va_arg (ap, float *) = sign_flag ?
			  			   - fvalue : fvalue;
		        }
		    }
		  else
		    {
		      *va_arg (ap, double *) = sign_flag ? - fvalue : fvalue;
		    }
		  ++assigned_items;
		}
	      continue;
#endif

/*======================================================================*/    
/* 	Convert an octal integer			 		*/
/*======================================================================*/    
	   case 'o':
	     num_base = 8;
             goto ANY_CONVERT;

/*======================================================================*/
/*	 Convert a hexadecimal integer					*/
/*======================================================================*/
	   case 'x':
	   case 'X':
	     num_base = 16;

#ifdef REDUCED_SUPPORT
             goto ANY_CONVERT;
#else
             goto HEX_TEST;

/*======================================================================*/
/*	 Convert any base integer					*/
/*======================================================================*/
	   case 'i':
	     num_base = 10;
	   HEX_TEST:
	     c = get_sign_etc (line, &sign_flag, &field_width);
	     if (c == '0' && field_width)
	       {
		 if (tolower ((*line)[1]) == 'x' && field_width > 1)
		   {
		     c = *((*line) += 2);
		     num_base = 16;
		     field_width -= 2;
		   }
		 else if (num_base == 10)
	           {
		     num_base = 8;
		   }
	       }
	     goto SPECIAL_CONVERT;

/*======================================================================*/    
/* 	Convert a * (pointer) value					*/
/*======================================================================*/
	    case 'p':
	      if (sizeof (int) != sizeof (void *))
	        {
		  long_flag = 1;
		}
#endif

/*======================================================================*/    
/* 	Convert an 'unsigned int'					*/
/*======================================================================*/    
	   case 'u':

/*======================================================================*/    
/*	Convert a decimal 'int'						*/
/*======================================================================*/    
	    case 'd':
	    case 'h':
	      num_base = 10;

/*======================================================================*/    
/*	Major conversion loop						*/
/*======================================================================*/    
	    ANY_CONVERT:
	      c = get_sign_etc (line, &sign_flag, &field_width);

	    SPECIAL_CONVERT:
	      if (!field_width || get_num_base (c, num_base) == ILL_BASE)
	        {
		  goto BAD_CONVERSION;
	        }

	      value = 0;
	      while ((i = get_num_base (**line, num_base)) != ILL_BASE &&
	             field_width--)
	        {
		  value = (value * num_base) + i;
		  (*line)++;
	        }

              if (assign_flag)
	        {
	          if (sign_flag)
	            {
	              value = -value;
	            }

#ifdef  WANT_LONG_TYPES
	          if (long_flag)
	            {
	              *va_arg (ap, long *) = value;
	            }
	          else
	            {
	      	      *va_arg (ap, int *) = value;
		    }
#else
		  *va_arg(ap, int *) = value;
#endif
	
		  ++assigned_items;
	        }
	      continue;

#ifndef REDUCED_SUPPORT
/*======================================================================*/    
/*	Scan for pattern matching string 				*/
/*======================================================================*/    
	    case '[':
	      if (assign_flag)
		{
		  temp_string_ptr = va_arg (ap, char *);
		}

	      if ((c = **format) == '^')
	        {
		  sign_flag = 0;
		  c = *++(*format);
		}
	      else
	        {
		  sign_flag = 1;
		}
	      if (c)
	        {
		  item_flag = 1;     /* ICLM  is it a new item */
		  while (field_width-- && **line)
		    {
		      if (get_valid_char (**line, sign_flag, *format))
		        {
			  if (item_flag)    /* if a new item */
		            {
			      assigned_items++;
			      item_flag = 0;
		            }
			  if (assign_flag)
		            {
		              *temp_string_ptr++ = **line;
		            }
	                  (*line)++;
			}
		      else
		        {
			  break;
			}
		    }
		  if (!item_flag)
		    {
		      *temp_string_ptr = '\0';
		      item_flag = 1;
		    }
		  assign_flag = 0; /* this will pass the PUT_VALUE_IN_STRING*/

		  
		  while (c = *++(*format))
		    {
		       if (c == ']')
		         {
			   (*format)++;
			   break;
			 }
	            }
	        }
	      goto PUT_VALUE_IN_STRING;
#endif

/*======================================================================*/    
/* 	Convert a character sequence				 	*/
/*======================================================================*/    
	    case 'c':
	      if (assign_flag)
		{
		  temp_string_ptr = va_arg (ap, char *);
		}

	      if (field_width == MAX_FIELD_ALLOWED)
	        {
		  field_width = 1;
		}

	      while (field_width-- && **line)
	        {
		  if (assign_flag)
		    {
		      *temp_string_ptr++ = **line;
		    }
	          (*line)++;
	        }

	      assigned_items += assign_flag;
	      continue;

/*======================================================================*/    
/* 	Scan a string						 	*/
/*======================================================================*/    
	    case 's':
	      if (assign_flag)
		{
		  temp_string_ptr = va_arg (ap, char *);
		}

	      get_non_space (line);

              while (field_width-- && **line && !isspace (**line))
	        {
		  if (assign_flag)
		    {
		      *temp_string_ptr++ = **line;
		    }
	          (*line)++;
	        }

	    PUT_VALUE_IN_STRING:
	      if (assign_flag)
	        {
		  ++assigned_items;
		  *temp_string_ptr = '\0';
	        }
	      continue;

#ifndef REDUCED_SUPPORT
/*======================================================================*/    
/* 	Tell how many character we have eaten so far		 	*/
/*======================================================================*/    
	    case 'n':
	      *va_arg (ap, int *) = *line - save_line;
	      continue;

/*======================================================================*/    
/* 	'%' Test						 	*/
/*======================================================================*/    
	    case '%':
	      if (*(*line)++ == '%')
	        {
	          continue;
		}
#endif

/*======================================================================*/    
/* 	Not a good conversion char, return				*/
/*======================================================================*/    
	    BAD_CONVERSION:
	      if (!**line) while (isspace(**format))
	        {
		  (*format)++;
		}
	    default:
	      return (assigned_items);
	  }
      }
    return (assigned_items);
  }
