#include "../Includes/defuser_wizard.h"

int	chrstr(char c, char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == c)
			return (1);
	}
	return (0);
}

int	left_strcmp(char *str1, char *str2)
{
	while (str1 && str2 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	if (str2 && !*str2)
		return (0);
	return (1);
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
	i = 0;
	while (dest && *dest)
		res[i++] = *dest++;
	while (src && *src)
		res[i++] = *src++;
	res[i] = 0;
	return (res);
}

void	free_str_tab(char **str_tab)
{
	int	i;

	i = 0;
	while (str_tab[i])
	{
		if (str_tab[i])
			free(str_tab[i]);
		i++;
	}
	free(str_tab);
}
