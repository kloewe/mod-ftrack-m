/*----------------------------------------------------------------------------
 *  File    : macros.h
 *  Contents: Macros
 *  Author  : Kristian Loewe
 * -------------------------------------------------------------------------*/
#ifndef MACROS_H
#define MACROS_H

/*----------------------------------------------------------------------------
  Preprocessor Definitions
----------------------------------------------------------------------------*/
#ifndef NDEBUG                      /* if debug version */
#  ifndef __STRING
#    define __STRING(x) #x
#  endif
#  define TOSTRING(x) __STRING(x)
#  define ASSERT(cond) \
   ((cond) ? (void)0 : (void) mexErrMsgTxt( \
           __FILE__ ":" TOSTRING(__LINE__) ": "\
           "Assertion `" __STRING(cond) "' failed."))
#  define ERROR(s) mexErrMsgTxt(__FILE__ ":" TOSTRING(__LINE__) ": "s)
#else
#  define ASSERT(cond) ((void) 0)
#  define ERROR(s)      mexErrMsgTxt((s))
#endif

#endif
