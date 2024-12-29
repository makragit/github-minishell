// DEBUG.C
// DEBUG.C
// DEBUG.C

# include "debug.h"

void DEBUG_msg(char *msg)
{
	#ifdef DEBUG
		LOG_RED(msg);
	#else
	    (void)msg;
	#endif
}


//void *DEBUG_ret(char *msg, void *value)
void *DEBUG_value(char *msg, void *value)
{
    #ifdef DEBUG
        LOG_RED(msg);  // Display the debug message
	#else
	    (void)msg;
    #endif
    return value;  // Return the value as a generic void *
}

void *DEBUG_new(char *msg)
{
    #ifdef DEBUG
        LOG_RED(msg);
	#else
	    (void)msg;
    #endif
    return (void *)0;
}



char *DEBUG_NULL(char *msg)
{
    DEBUG_msg(msg);
	return NULL;
}

int DEBUG_0(char *msg)
{
    DEBUG_msg(msg);
	return 0;
}

void DEBUG_VOID(char *msg)
{
    DEBUG_msg(msg);
	return ;
}

void DEBUG_str(char *msg, char *str)
{
	#ifdef DEBUG
		printf("\n%s %s %s %s\n", CLR_BLUE, msg, str, CLR_END);
	#else
	    (void)msg;
	    (void)str;
	#endif
}

void DEBUG_int(char *msg, int d)
{
	#ifdef DEBUG
		printf("\n%s %s %d %s\n", CLR_BLUE, msg, d, CLR_END);
	#else
	    (void)msg;
	    (void)d;
	#endif
}

void DEBUG_arr(char* msg, char **arr) // only string-arrays
{
	int i;
	i = 0;

	#ifdef DEBUG
		printf("\n%s %s %s\n", CLR_BLUE, msg, CLR_END);
		while (arr[i])
		{
			printf("%s - arr[%d]: %s %s\n", CLR_BLUE, i, arr[i], CLR_END);
			i++;
		}
		printf("\n");
	#else
	    (void)i;
	    (void)msg;
	    (void)arr;
	#endif
}

//Rewrite with 5 args that can be free_0(arg1, NULL, NULL, etc)
int free_0(int count, ...) {
    va_list args;
    void *ptr;

    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        ptr = va_arg(args, void *);
        if (ptr != NULL) {
            free(ptr);
        }
    }

    va_end(args);
	return(0);
}


// PRINT OPEN FDs DEBUG

void print_fd_info(int fd) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/self/fd/%d", fd);
    
    char buf[256];
    ssize_t len = readlink(path, buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        printf("FD %d: %s\n", fd, buf);
    } else {
        printf("FD %d: (unknown or closed)\n", fd);
    }
}

void debug_open_fds() {
    printf("Debugging open file descriptors:\n");
    for (int fd = 0; fd < getdtablesize(); fd++) {
        if (fcntl(fd, F_GETFD) != -1) { // Check if fd is open
            print_fd_info(fd);
        }
    }
}