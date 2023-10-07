#include "../../Includes/defuser_wizard.h"

noreturn void	fail_exit(const char *msg)
{
	// Make sure endwin() is only called in visual mode. As a note, calling it
	// twice does not seem to be supported and messed with the cursor position.
	if (g_data_s.rlncurses_s.visual_mode)
		endwin();
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

void	save_error(char *dest, size_t dest_buff, const char *prefix,
			const char *prefix2)
{
	size_t	prefix_len;
	size_t	prefix2_len;
	char	*colon;
	char	*error;

	if (!dest || !dest_buff)
		return ;
	colon = ": ";
	prefix_len = 0;
	if (prefix)
	{
		prefix_len = strlen(prefix) + 2;
		safer_strncpy(dest, (char *) prefix, dest_buff, prefix_len - 2);
		safer_strncpy(dest + prefix_len - 2, colon, dest_buff - prefix_len - 2, 2);
	}
	prefix2_len = 0;
	if (prefix2)
	{
		prefix2_len = strlen(prefix2) + 2;
		safer_strncpy(dest + prefix_len, (char *) prefix2, dest_buff - prefix_len,
			prefix2_len - 2);
		safer_strncpy(dest + prefix_len + prefix2_len - 2, colon,
			dest_buff - prefix_len - prefix2_len - 2, 2);
	}
	error = strerror(errno);
	safer_strncpy(dest + prefix_len + prefix2_len, error,
		dest_buff - prefix_len - prefix2_len, strlen(error));
}
