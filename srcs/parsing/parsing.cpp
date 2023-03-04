/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 03:28:04 by rimney            #+#    #+#             */
/*   Updated: 2023/03/04 02:00:47 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    if(argc == 2)
    {
        config_parser servers(argv[1]);
        config_parser aa = servers;
        std::cout << aa.getServerCountObject() << " << EEEEEEE\n";
        std::cout << servers.getServerCountObject() << " << EEEE\n";
        // system("leaks a.out");
    }
}
