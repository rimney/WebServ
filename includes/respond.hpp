/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:42:24 by rimney            #+#    #+#             */
/*   Updated: 2023/03/15 03:20:50 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPOND_HPP
# define REDPOND_HPP
# include "../includes/parsing.hpp"

typedef struct res
{
    std::string httpVersion; // http/1.1
    std::string statusCode; // 200 404 501
    std::string statusDescription; // OK ERROR
	std::string ContentLenght; // len of body
    std::string Body; // content
    std::string finalString; // 
} t_respond;

#endif
