/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney <rimney@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 20:56:08 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/04/02 03:29:11 by rimney           ###   ########.fr       */
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
            servers     s;
            if (s.setup(config.getServersObject()))
            {
                return 1;
            }
            s.run();
        }
        catch(const std::string& msg)
        {
            std::cerr << msg << '\n';
        }
    }
    else
        std::cerr << "ERROR: bad args number\n";
    
    return 0;
}
