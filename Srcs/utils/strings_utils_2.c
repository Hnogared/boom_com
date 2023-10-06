#include "../../Includes/defuser_wizard.h"

// Calculates the cursor column for the readline window in a way that supports
// multibyte, multi-column and combining characters. readline itself calculates
// this as part of its default redisplay function and does not export the
// cursor column.
//
// Returns the total width (in columns) of the characters in the 'n'-byte
// prefix of the null-terminated multibyte string 's'. If 'n' is larger than
// 's', returns the total width of the string. Tries to emulate how readline
// prints some special characters.
//
// 'offset' is the current horizontal offset within the line. This is used to
// get tab stops right.
//
// Makes a guess for malformed strings.
size_t	strnwidth(const char *s, size_t n, size_t offset)
{
	mbstate_t shift_state;
	wchar_t wc;
	size_t wc_len;
	size_t width = 0;

	// Start in the initial shift state
	memset(&shift_state, '\0', sizeof shift_state);

	for (size_t i = 0; i < n; i += wc_len) {
		// Extract the next multibyte character
		wc_len = mbrtowc(&wc, s + i, MB_CUR_MAX, &shift_state);
		switch (wc_len) {
		case 0:
			// Reached the end of the string
			goto done;

		case (size_t)-1: case (size_t)-2:
			// Failed to extract character. Guess that each character is one
			// byte/column wide each starting from the invalid character to
			// keep things simple.
			width += strnlen(s + i, n - i);
			goto done;
		}

		if (wc == '\t')
			width = ((width + offset + 8) & ~7) - offset;
		else
			// TODO: readline also outputs ~<letter> and the like for some
			// non-printable characters
			width += iswcntrl(wc) ? 2 : max(0, wcwidth(wc));
	}

done:
	return width;
}

// Like strnwidth, but calculates the width of the entire string
size_t	strwidth(const char *s, size_t offset)
{
	return strnwidth(s, SIZE_MAX, offset);
}
