/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 20:56:08 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/02/25 21:00:22 by eel-ghan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserv.hpp"
#include "includes/parsing.hpp"
#include "srcs/server/server.hpp"
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    if(argc == 2)
    {
        config_parser servers(argv[1]);
        server_parser *p = servers.getServersObject();
        // server_location *s  =  p[0].getServerLocationsObject();
        
        // // config_parser aa = servers;
        // // std::cout << servers;
        // std::cout << p[0].getCmbsObject() << std::endl;
        // std::cout << p[0].getHostObject() << std::endl;
        // std::cout << s[0].getLocationNameObject() << std::endl;
        // std::cout << s[0].getLocationRootObject() << std::endl;
        // std::cout << s[1].getLocationNameObject() << std::endl;
        // std::cout << s[1].getLocationRootObject() << std::endl;


        try
        {
            server  s(p[0].getPortObject(),p[0].getHostObject(),servers);
            s.run();
        }
        catch(std::string const & msg)
        {
            std::cout << msg << '\n';
        }

        
        // config_parser aa = servers;

        
        // // system("leaks webserv");

    }
}
