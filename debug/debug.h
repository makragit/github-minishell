// DEBUG_H 
// DEBUG_H 
// DEBUG_H 

#ifndef DEBUG_H

# define DEBUG_H

# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdarg.h>

#define CLR_RED     "\x1b[31m"
#define CLR_GREEN   "\x1b[32m"
#define CLR_YELLOW  "\x1b[33m"
#define CLR_BLUE    "\x1b[34m"
#define CLR_MAGENTA "\x1b[35m"
#define CLR_CYAN    "\x1b[36m"
#define CLR_END   	"\x1b[0m"

#define LOG_RED(X) printf("%s %s %s",CLR_RED,X,CLR_END)
#define LOG_BLUE(X) printf("%s %s %s",CLR_BLUE,X,CLR_END)
#define LOG_GREEN(X) printf("%s %s %s",CLR_GREEN,X,CLR_END)
#define LOG_YELLOW(X) printf("%s %s %s",CLR_YELLOW,X,CLR_END)
#define LOG_MAGENTA(X) printf("%s %s %s",CLR_MAGENTA,X,CLR_END)
#define LOG_CYAN(X) printf("%s %s %s",CLR_CYAN,X,CLR_END)
//printf(CLR_BLUE"\nget_next: %s\n"CLR_END, ret);

// DEBUG Flag comment out
//# define DEBUG

// Define the DEBUG_printf macro to handle variable arguments
#ifdef DEBUG
    #define DEBUG_printf(fmt, ...) \
        do { \
            printf("%s", CLR_RED); \
            printf(fmt, ##__VA_ARGS__); \
            printf("%s\n", CLR_END); \
        } while (0)
#else
    // If DEBUG is not defined, the macro does nothing
    #define DEBUG_printf(fmt, ...) (void)0
#endif

#ifdef DEBUG
    #define DEBUG_printf2(color, fmt, ...) \
        do { \
            printf("%s", color); \
            printf(fmt, ##__VA_ARGS__); \
            printf("%s\n", CLR_END); \
        } while (0)
#else
    // If DEBUG is not defined, the macro does nothing
    #define DEBUG_printf2(color, fmt, ...) (void)0
#endif



void DEBUG_msg(char *msg);

char *DEBUG_NULL(char *msg); // return(DEBUG_NULL("bla"));
int DEBUG_0(char *msg);
void DEBUG_VOID(char *msg);

void DEBUG_arr(char *msg, char **arr); // only string-arrays
void DEBUG_int(char *msg, int d);
void DEBUG_str(char *msg, char *str);

// DEBUG_new using void* use like this:
// return (int)DEBUG_new("Returning NULL\n");
// ??? return (int)(intptr_t)DEBUG_ret("Returning NULL\n", NULL);new
void *DEBUG_new(char *msg);

// DEBUG and return a value using void *
void *DEBUG_value(char *msg, void *value);


// Helper Functions KP
int free_0(int count, ...);

// PRINT OPEN FDs DEBUG
void print_fd_info(int fd);
void debug_open_fds();


#endif
