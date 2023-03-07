/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 20:56:08 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/03/07 20:37:19 by eel-ghan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/parsing.hpp"
#include "includes/servers.hpp"

int main(int argc, char **argv)
{

    if(argc == 2)
    {
        try
        {
            config_parser parsing(argv[1]);
            // server_parser *p = parsing.getServersObject();
            // config_parser aa = servers;
            std::cout << parsing;
            // std::cout << p[0].getPortObject();

            servers     s;
            s.run();

         // system("leaks webserv");
        }
        catch(const std::string& msg)
        {
            std::cerr << msg << '\n';
        }
    }
    else
        std::cout << "ERROR: bad args\n";
    
    return 0;
}