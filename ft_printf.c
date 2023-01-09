/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shinckel <shinckel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 16:50:23 by shinckel          #+#    #+#             */
/*   Updated: 2023/01/09 19:58:42 by shinckel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_putstr(char *s, char c, int n)
{
	int	i;

	i = 0;
	if (n)
		return (write(1, &c, 1));
	if (!s)
		return (write(1, "(null)", 6));
	while (s[i])
		write(1, &s[i++], 1);
	return (i);
}

static int	ft_nbr(unsigned long int n, unsigned long int b, char *base, int ox)
{
	int		res;

	res = 0;
	if (ox)
	{
		if (!n)
			return (ft_putstr("(nil)", 0, 0));
		res += ft_putstr("0x", 0, 0);
	}
	if (n >= b)
		res += ft_nbr(n / b, b, base, 0);
	res += ft_putstr(NULL, base[n % b], 1);
	return (res);
}

static void	ft_placeholder(char c, va_list args, int *len);

int	ft_printf(const char *fmt, ...)
{
	int		i;
	int		len;
	va_list	args;

	va_start(args, fmt);
	i = 0;
	len = 0;
	while (fmt[i])
	{
		if (fmt[i] == '%')
		{
			i++;
			if (fmt[i] == '%')
				len += ft_putstr(NULL, '%', 1);
			ft_placeholder(fmt[i], args, &len);
		}
		else
			len += ft_putstr(NULL, fmt[i], 1);
		i++;
	}
	va_end(args);
	return (len);
}

static void	ft_placeholder(char c, va_list args, int *len)
{
	long int	aux;

	if (c == 'c')
		*len += ft_putstr(NULL, va_arg(args, int), 1);
	else if (c == 's')
		*len += ft_putstr(va_arg(args, char *), 0, 0);
	else if (c == 'p')
		*len += ft_nbr(va_arg(args, long int), 16, HEX_L, 1);
	else if (c == 'd' || c == 'i')
	{
		aux = va_arg(args, int);
		if (aux < 0)
		{
			*len += write(1, "-", 1);
			aux *= -1;
		}
		*len += ft_nbr(aux, 10, DEC, 0);
	}
	else if (c == 'u')
		*len += ft_nbr(va_arg(args, unsigned int), 10, DEC, 0);
	else if (c == 'x')
		*len += ft_nbr(va_arg(args, unsigned int), 16, HEX_L, 0);
	else if (c == 'X')
		*len += ft_nbr(va_arg(args, unsigned int), 16, HEX_U, 0);
}

/* <variadic function> output formatting, writes the string pointed by format;
 * one argument that specifies the output formatting + any number of arguments;
 *...that provide the values to be formatted;
 * variadic function= indefinite arity, i.e.accepts a variable num of arguments;
 * format string attacks= the user may use the %s and %x format tokens to print; 
 * ...data from the call stack or possibly other locations in memory...; 
 * ...or write arbitrary data to arbitrary locations using %n format token;
 * 
 * structure types
 * this is the syntax= typedef existing_data_type new_data_type;
 * typedef enables the user to create their own types(old type replaced by new);
 * 
 * a format specifier(type of argument) follows this prototype...;
 * %[flags][width][.precision][length]specifier...it remains in each state...;
 * ...as long exists one, otherwise it moves on to the next field;
 * ...e.g.(printf("%3d %6d\n", fahr, celsius)) print temperature in two collumns;
 * ...so, '3' and '6' are [width];
 * 
 * first, solve how to pass a variable number of parameters(ellipsis operator);
 * here, you are using macros(va_start, va_arg, va_copy, va_end) from <stdarg.h>;
 * va_start(access variadic function arguments), va_arg(access the next arg)...; 
 * va_copy(makes a copy), va_end(ends traversal of the function arg);
 * va_list holds the information needed by va_start, va_arg, va_copy, va_end;
 * va_list is a structure... using pointers for accessing values in stack;
 * (52)...therefore, you must create it!
 * (54)sets a pointer(call stack) where variables were stored, starting the list;
 * va_arg will pull the arguments one by one, in the order they are listed...;
 * ...va_arg syntax= va_arg(va_list, datatype), e.g.datatype could be 'int';
 * each va_arg invocation modifies va_list to point the next variable argument;
 * 
 * 0xFF= '0x' prefix just marks the literal as hexadecimal in C(doesn't count);
 * 
 * printf must return the quantity of printable characters(len)...;
 * this is a variable that works aside of fmt index 'i'(as ellipsis is unknown);
 * therefore, you must create a pointer for collecting the size of each va_arg;
 * 
 * for solving printf succinctly, you can create two generic functions...;
 * ...one when working with char and strings(ft_putstr)...;
 * ...and other for numbers(ft_nbr)...;
 * for the last one, solve the negatives inside ft_placeholder and...;
 * ...work only with positives. In the case of pointers, add a flag(ox);
 * ...for treating them differently! :)
 * 
 */

// int main(void)
// {
// 	int i;
// 	int c = 5455;
// 	char *aux = "sofia";

// 	i = ft_printf("%i %s %%\n", c, aux);
// 	ft_printf("Total len -> %d", i);
// 	return (0);
// }