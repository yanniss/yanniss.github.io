//
// Copyright (c) 2000,2001 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CONFIG_DOT_H
#define FCPP_CONFIG_DOT_H

// Here we just do the kludges necessary to make stuff work with some
// compilers.

#ifdef __GNUC__
   // __GNUC_MINOR__ is also available

#   if __GNUC__ == 2

       // g++2.95.2 has a non-conforming <iterator> library
#      define FCPP_NO_STD_ITER

       // g++2.95.2 has bugs that prevent namespace use in some cases
#      define FCPP_NO_USE_NAMESPACE

#   endif

#endif



#endif
