#include "../../include/libft.h"

int	ft_isspace(char c)
{
	return ((c == '\t') || (c == '\f')
		|| (c == '\v') || (c == '\r') || (c == ' '));
}
