/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 20:56:08 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/03/14 15:38:53 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/parsing.hpp"
#include "includes/servers.hpp"

int main(int argc, char **argv)
{
    // (void)argc;
    // config_parser s(argv[1]);
    // std::cout << s;
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
