/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 03:28:04 by rimney            #+#    #+#             */
/*   Updated: 2023/03/05 18:07:31 by rimney           ###   ########.fr       */
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
        std::cout << servers;
        // system("leaks a.out");
    }
}
