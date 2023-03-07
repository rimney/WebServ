/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 20:56:08 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/03/07 02:00:38 by eel-ghan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/parsing.hpp"
#include "includes/servers.hpp"

int main(int argc, char **argv)
{
    (void) argv;
    (void) argc;
    // if(argc == 2)
    // {
        try
        {
            // config_parser conf(argv[1]);
            // config_parser aa = conf;
            // std::cout << conf;
            servers     s;

            s.run();
        }
        catch(const std::string& msg)
        {
            std::cerr << msg << '\n';
        }
    // }
    // else
    //     std::cout << "ERROR: bad args\n";
    
    return 0;
}