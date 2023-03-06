/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 20:56:08 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/03/06 23:50:28 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserv.hpp"
#include "includes/parsing.hpp"

int main(int argc, char **argv)
{
    if(argc == 2)
    {
        config_parser servers(argv[1]);
        // config_parser aa = servers;
        std::cout << servers;
        // system("leaks webserv");

    }
}