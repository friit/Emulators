/**
	Macros.h
	Purpose: Include basic data types and debug macros into code

	@author Joseph Stevens
	@version 1 9/10/16
*/


#pragma once
#ifndef DATATYPES_H
#define DATATYPES_H
#include <iostream>

//Debug Macros
#define DEBUG 1

#if DEBUG
	#define LOG(x) std::cout << x << std::endl
#else 
	#define LOG(x)
#endif

//Set up some basic types for ease of use and readability
typedef unsigned char BYTE;
typedef unsigned short int WORD;

#endif // !DATATYPES_H
