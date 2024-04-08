/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 11:33:15 by eagranat          #+#    #+#             */
/*   Updated: 2024/04/08 12:49:58 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_str;
	size_t	str_len;
	size_t	sub_len;

	if (!s)
		return (NULL);
	str_len = ft_strlen(s);
	if (str_len <= start)
		return (ft_strdup(""));
	else if (str_len > start + len)
		sub_len = len + 1;
	else
		sub_len = str_len - start + 1;
	sub_str = (char *)malloc(sizeof(char) * (sub_len));
	if (!sub_str)
		return (NULL);
	ft_strlcpy(sub_str, s + start, sub_len);
	return (sub_str);
}
