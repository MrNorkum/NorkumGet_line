#ifndef GET_LINE_BONUS_H
# define GET_LINE_BONUS_H

# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 13
# endif

size_t	my_strlen(const char *s);
char	*my_strdup(const char *s1);
char	*my_strjoin(char const *s1, char const *s2);
char	*my_substr(char const *s, unsigned int start, size_t len);
void	*my_free_stash(char **stash, int create_line);
char	*get_line(int fd);

#endif
