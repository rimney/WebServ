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
        int listenn = socket(AF_INET, SOCK_STREAM, 0);
        if(listen == -1)
        {
            std::cerr << "Error cannot create Socket !\n";
            exit(0);
        }
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &addr);
        if (bind(listenn, (sockaddr *)&addr, sizeof(addr)) == -1)
        {
            std::cerr << "Error Cannot Bind To IP/PORT";
            exit(0);
        }
        if(listen(listenn, SOMAXCONN) == -1)
        {
            std::cerr << "Can't listen ";
            exit(0);
        }
        
        
        // std::cout << p[0].getPortObject();
        // system("leaks websrv");

    }
}