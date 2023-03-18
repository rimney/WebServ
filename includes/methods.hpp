/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 18:28:10 by rimney            #+#    #+#             */
/*   Updated: 2023/03/18 19:13:28 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODS_HPP
#define METHODS_HPP
#include "webserv.hpp"

class Get
{
    private :
        std::string path;
        server_parser _server;
        server_location _location;
        int location_index;
        respond _respond;
    public :
        Get(){}
        Get(server_parser s, respond Respond, int location_index , std::string path) 
        {
            this->_respond = Respond;
            this->location_index = location_index;
            this->_server = s;
            this->_location = this->_server.getOneLocationObject(location_index);
            std::string isFOrD = isFileOrDirectory(path);
            std::cout << _respond.getstatusCode() << " code\n";
            if(isFOrD == "file")
            {
                if()
                std::cout << "IS A FILE\n";
            }
            else if(isFOrD == "directory")
            {
                std::cout << "IS A DIRECTORY\n";
            }
            exit(0);
        }
};

#endif