/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 20:56:08 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/03/09 21:49:45 by eel-ghan         ###   ########.fr       */
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
            config_parser config(argv[1]);
            
            // std::cout << config;

            servers     s(config);
            if (s.setup(config.getServersObject()))
            {
                // system("leaks webserv");
                return 1;
            }
            s.run();

            // system("leaks webserv");
        }
        catch(const std::string& msg)
        {
            std::cerr << msg << '\n';
        }
    }
    else
        std::cout << "ERROR: bad args number\n";
    
    return 0;
}
