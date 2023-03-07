/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghan <eel-ghan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 20:56:08 by eel-ghan          #+#    #+#             */
/*   Updated: 2023/03/07 01:00:26 by eel-ghan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserv.hpp"

int main()
{
    try
    {
        servers     s;

        s.run();
        
        // server s(440, INADDR_ANY);
        // server s1(880, INADDR_ANY);

        // while (1)
        // {
        //     s.accept();
        //     s1.accept();
        //     // s.receive();
        //     std::cout << s.get_request() << '\n';
        //     close(s.get_fd_connection());
        //     close(s1.get_fd_connection());
        // }
    
    }
    catch(const std::string& msg)
    {
        std::cerr << msg << '\n';
    }
    
    return 0;
}