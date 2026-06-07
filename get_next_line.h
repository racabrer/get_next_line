#ifndef GET_NEXT_LINE
#define GET_NEXT_LINE

# define BUFFER_SIZE 42
#ifndef MAX_FD
# define MAX_FD 1024
#endif

static char     *gnl_update_saved(char *saved);
static void     gnl_free_and_null(char **ptr);
static char     *gnl_get_line_from_saved(char *saved);
static char     *gnl_read_and_join(char *saved, int fd);
char            *get_next_line(int fd);
static char     *ft_strcpy(char *dest, char *src);
char            *ft_strjoin(const char *s1, const char *s2);
size_t          ft_strlen(const char *s);
char            *ft_strchr(const char *s, int c);



#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#endif