/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wrapper.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mraasvel <mraasvel@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/19 17:03:55 by mraasvel      #+#    #+#                 */
/*   Updated: 2021/12/15 21:47:02 by mraasvel      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

void	exit_perror(const char *str)
{
	if (str != NULL)
		perror(str);
	exit(EXIT_FAILURE);
}

void	*oom_guard(void *malloc_return)
{
	if (malloc_return == NULL)
		exit_perror("malloc");
	return (malloc_return);
}

void	*ft_malloc(size_t size)
{
	return (oom_guard(malloc(size)));
}
