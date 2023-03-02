/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 03:28:04 by rimney            #+#    #+#             */
/*   Updated: 2023/03/02 01:45:30 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    if(argc == 2)
    {
        config_parser servers(argv[1]);
        // system("leaks a.out");
    }
}