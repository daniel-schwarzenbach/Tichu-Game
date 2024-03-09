//
// Created by Daniel on Spring 2023
//
// those are just some convetions I use to program
#ifndef DSCHWARZE
#define DSCHWARZE

#define RESET   "\033[0m"     //   RESET
#define RED     "\033[31m"    //   RED
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[35m"      /* Blue */

#define ERROR   "\033[31m [Error]: \033[0m"
#define WARNING "\033[35m [Warning]: \033[0m"
#define INFO    "\033[32m [Info]: \033[0m"
#define FORMAT(width) std::left<<std::setw(width)<<std::setfill(' ')

// #define SERVER_DEBUG
// #if defined(SERVER_DEBUG)
//     #define debug_log(message, ...)\
//         fprintf(stderr,"%s", GREEN);\
//         fprintf(stderr, message, ##__VA_ARGS__); \
//         fprintf(stderr, "\n"); \
//         fprintf(stderr, "%s" RESET);
// #else
//     #define debug_log(message, ...)
// #endif

#include <string>
using std::string;

#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::cerr;

#include <vector>
using std::vector;

#endif // DSCHWARZE