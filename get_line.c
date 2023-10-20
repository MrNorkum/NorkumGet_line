#include "get_line.h"
#include <unistd.h>

static char	*my_copy_to_stash(char *stash, char *buf)
{
	char	*res;

	res = 0;
	if (!stash && buf)
	{
		res = my_strdup(buf);
		if (!res)
			return (NULL);
		return (res);
	}
	res = my_strjoin(stash, buf);
	my_free_stash(&stash, 0);
	return (res);
}

static int	my_have_nl(char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = -1;
	while (s[++i] != '\0')
		if (s[i] == '\n')
			return (1);
	return (0);
}

static char	*my_extract_line(char *stash)
{
	char	*line;
	size_t	i;
	size_t	j;

	i = 0;
	if (!stash)
		return (my_free_stash(&stash, 0));
	while (stash[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (my_free_stash(&line, 0));
	j = 0;
	while (j < i + 1)
	{
		line[j] = stash[j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

static char	*my_recreate_stash(char *stash)
{
	size_t	i;
	char	*res;

	i = 0;
	if (!stash)
		return (NULL);
	while (stash[i] != '\n')
		i++;
	if (stash[i + 1] == '\0')
		return (my_free_stash(&stash, 0));
	res = my_substr(stash, i + 1, my_strlen(stash));
	if (!res)
	{
		my_free_stash(&stash, 0);
		return (NULL);
	}
	my_free_stash(&stash, 0);
	return (res);
}

char	*get_line(int fd)
{
	char		buf[BUFFER_SIZE + 1];
	long		ret;
	static char	*stash = NULL;
	char		*line;

	line = 0;
	ret = BUFFER_SIZE;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (my_free_stash(&stash, 0));
	while (ret > 0)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if ((ret <= 0 && !stash) || ret == -1)
			return (my_free_stash(&stash, 0));
		buf[ret] = '\0';
		stash = my_copy_to_stash(stash, buf);
		if (my_have_nl(stash))
		{
			line = my_extract_line(stash);
			if (!line)
				return (my_free_stash(&stash, 0));
			return (stash = my_recreate_stash(stash), line);
		}
	}
	return (my_free_stash(&stash, 1));
}
