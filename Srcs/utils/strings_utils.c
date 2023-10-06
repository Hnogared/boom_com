#include "../Includes/defuser_wizard.h"

char	*safer_strncpy(char *dest, char *src, size_t dest_len, size_t len)
{
	if (!dest || !src || !dest_len || !len)
		return (dest);
	len = (len < dest_len) * len + (dest_len < len) * (dest_len - 1);
	dest[len] = 0;
	return (strncpy(dest, src, len));
}

char	*ft_strtrim(char *str, char *set)
{
	char	*start;
	char	*end;
	char	*res;

	if (!str || !set)
		return (NULL);
	start = str;
	while (*start && strchr(set, *start))
		start++;
	if (!*start)
		return (strdup(""));
	end = start;
	while (*(end + 1))
		end++;
	while (strchr(set, *end))
		end--;
	res = strndup(start, end - start + 1);
	if (!res)
		return (NULL);
	res[end - start + 1] = 0;
	return (res);
}

char	*crop(char *str)
{
	char	*start;

	start = str;
	while (str && *str && *str != '$')
		str++;
	if (str && *str == '$' && *(str + 1))
		*str++ = 0;
	while (str && *str)
		*str++ = 0;
	return (start);
}

char	*ft_strjoin(char *dest, char *src)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	while (dest && dest[i])
		i++;
	while (src && src[j])
		j++;
	res = (char *) malloc((i + j + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = -1;
	while (dest && dest[++i])
		res[i] = dest[i];
	j = -1;
	while (src && src[++j])
		res[i + j] = src[j];
	res[i + j] = 0;
	return (res);
}

void	free_str_tab(char **str_tab)
{
	int	i;

	i = 0;
	while (str_tab[i])
	{
		free(str_tab[i]);
		i++;
	}
	free(str_tab);
	str_tab = NULL;
}
