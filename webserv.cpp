/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 20:56:08 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/03/07 01:19:09 by eel-ghan         ###   ########.fr       */
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
            servers     s;

            s.run();
        }
        catch(const std::string& msg)
        {
            std::cerr << msg << '\n';
        }
    }
    else
        std::cout << "bad args\n";
    
    return 0;
}