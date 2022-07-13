/*-
* SPDX-License-Identifier: BSD-2-Clause
* Copyright (c) 2022 Aymeric Wibo <obiwac@gmail.com>
*/

#include <ctype.h>
#include <stddef.h>

int
strverscmp(const char *s1, const char *s2)
{
	/* if pointers are aliased, no need to go through to process of comparing them */

	if (s1 == s2)
		return 0;

	const unsigned char *u1 = (const void*) s1;
	const unsigned char *u2 = (const void*) s2;

	for (; *u1 && *u2; u1++, u2++) {
		/* leading zeros; we're dealing with the fractional part of a number */

		if (*u1 == '0' || *u2 == '0') {
			/* count leading zeros (more zeros == smaller number) */

			size_t n1 = 0;
			size_t n2 = 0;

			for (; *u1 == '0'; u1++)
				n1++;

			for (; *u2 == '0'; u2++)
				n2++;

			if (n1 != n2)
				return (n2 - n1);

			/* handle the case where 000 < 09 */

			if (*u1 == '\0' && isdigit(*u2))
				return (1);

			if (*u2 == '\0' && isdigit(*u1))
				return (-1);

			/* for all other cases, compare each digit until there are none left */

			for (; isdigit(*u1) && isdigit(*u2); u1++, u2++) {
				if (*u1 != *u2)
					return (*u1 - *u2);
			}

			u1--;
			u2--;

			/* no leading zeros; we're simply comparing two numbers */

		} else if (isdigit(*u1) && isdigit(*u2)) {
			const unsigned char *o1 = u1;
			const unsigned char *o2 = u2;

			/* count digits (more digits == larger number) */

			size_t n1 = 0;
			size_t n2 = 0;

			for (; isdigit(*u1); u1++)
				n1++;

			for (; isdigit(*u2); u2++)
				n2++;

			if (n1 != n2)
				return (n1 - n2);

			/*
			 * if there're the same number of digits,
			 * go back and compare each digit until there are none left
			 */

			u1 = o1;
			u2 = o2;

			for (; isdigit(*u1) && isdigit(*u2); u1++, u2++) {
				if (*u1 != *u2)
					return (*u1 - *u2);
			}

			u1--;
			u2--;
		}

		/* for the rest, we can just fallback to a regular strcmp */

		if (*u1 != *u2)
			return (*u1 - *u2);
	}

	return (*u1 - *u2);
}
