/*
 * aPLib compression library  -  the smaller the better :)
 *
 * C depacker, header file
 *
 * Copyright (c) 1998-2004 by Joergen Ibsen / Jibz
 * All Rights Reserved
 *
 * http://www.ibsensoftware.com/
 */

#ifndef DEPACK_H_INCLUDED
#define DEPACK_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#ifndef APLIB_ERROR
# define APLIB_ERROR (-1)
#endif

/* function prototype */
unsigned int aP_depack(const void *source, void *destination);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DEPACK_H_INCLUDED */
