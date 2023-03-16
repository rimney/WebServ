/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:42:24 by rimney            #+#    #+#             */
/*   Updated: 2023/03/16 13:59:14 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPOND_HPP
# define RESPOND_HPP

# include "../includes/parsing.hpp"

class respond
{
    private:
        std::string httpVersion; // http/1.1
        std::string statusCode; // 200 404 501
        std::string statusDescription; // OK ERROR
	    std::string ContentLenght; // len of body
        std::string Body; // content
        std::string finalString; //
    public:
        respond(){};
        respond(respond const & r) {*this = r;}
        // respond & operator=(respond const & r)
        // {
        //     *this = r;
        //     return (*this);
        // }
        std::string gethttpVersion(void);
        std::string getstatusCode(void);
        std::string getstatusDescription(void);
        std::string getContentLenght(void);
        std::string getBody(void);
        std::string getfinalString(void);
        void        sethttpVersion(std::string httpVersion);
        void        setstatusCode(std::string statusCode);
        void        setstatusDescription(std::string statusDescription);
        void        setContentLenght(std::string ContentLenght);
        void        setBody(std::string Body);
        void        setFinalString(std::string finalString);
        void    setRespond(server_parser server, std::string method, std::string path, std::string httpVersion, std::string error);

        
};


#endif
