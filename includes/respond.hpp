/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney <rimney@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:42:24 by rimney            #+#    #+#             */
/*   Updated: 2023/03/14 20:40:18 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPOND_HPP
# define REDPOND_HPP
# include "../includes/parsing.hpp"

typedef struct res
{
    int			error_code;
    std::string	file_buffer;
	int			Content_Length;
	


} t_respond;

#endif
