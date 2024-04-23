
#include "utils.h"

int	ft_min(int num1, int num2)
{
	if (num1 < num2)
	{
		return (num1);
	}
	else
	{
		return (num2);
	}
}

int	ft_max(int num1, int num2)
{
	if (num1 > num2)
	{
		return (num1);
	}
	else
	{
		return (num2);
	}
}

void	*ft_realloc(void *ptr, size_t new_size, size_t old_size)
{
	void	*new;

	new = malloc(new_size);
	if (!new)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr)
	{
		ft_memcpy(new, ptr, ft_min(new_size, old_size));
	}
	free(ptr);
	return (new);
}
