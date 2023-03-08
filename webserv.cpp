/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 20:56:08 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/03/08 16:19:01 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserv.hpp"
#include "includes/parsing.hpp"



int main(int argc, char **argv)
{
    if(argc == 2)
    {
        config_parser servers(argv[1]);
        // server_parser *p = servers.getServersObject();
        // config_parser aa = servers;
        std::cout << servers;
        // std::cout << p[0].getPortObject();
        // system("leaks websrv");

    }
}