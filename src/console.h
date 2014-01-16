/*
 * Author      : Matthew Johnson
 * Date        : 10/Nov/2012
 * Description :
 *   Utilities for debuging using console I/O.
 */
#pragma once

#ifdef USE_CONSOLE

#include <iostream>

// Output
#define Print( msg ) std::cout << msg;

// Input
#define GetConsole( line ) std::get( line, stdin );

#else

#define Print( msg ) ;
#define GetConsole( line ) ;

#endif