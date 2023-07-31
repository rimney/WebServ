# Webserv - A C++ HTTP Server

## Summary
Webserv is a C++ project that involves writing your own HTTP server. This server allows you to serve web pages and handle HTTP requests from clients, including browsers. Understanding the HTTP protocol is crucial for web development and networking.

## Version
Version: 20.6

## Introduction
The Hypertext Transfer Protocol (HTTP) is an application protocol used in distributed, collaborative, hypermedia information systems. It forms the foundation for data communication on the World Wide Web, enabling hyperlinks that users can access with a simple click or tap.

## General Rules
- The program should be robust and not crash unexpectedly, even when facing memory issues.
- A Makefile is required to compile the source files with the flags `-Wall -Wextra -Werror`.
- The code must comply with the C++ 98 standard and should use C++ features wherever possible.
- External libraries, including Boost, are not allowed.
- Fork should only be used for CGI (Common Gateway Interface) implementations and not for other purposes like PHP or Python.
- The server must be non-blocking and use only one `poll()` or equivalent for all I/O operations.
- Requests to the server should never hang indefinitely.

## Mandatory Part
The program "webserv" takes a configuration file as an argument, or it uses a default path. The server should support multiple ports and hosts, with the first server acting as the default for unassigned requests. The configuration file allows users to define routes, set default error pages, limit client body size, and enable/disable directory listing. The server should handle HTTP methods like GET, POST, and DELETE. Additionally, the program must be able to serve a fully static website and handle uploaded files.

## Bonus Part
If the mandatory part is perfect, you may implement additional features like supporting cookies and session management or handling multiple CGI (Common Gateway Interface).

## Submission
To submit your assignment, use your Git repository. Ensure that your project works correctly and meets all the mandatory requirements before attempting the bonus part.

Happy coding! 🚀

(Note: This is a simplified version of the subject for the README, focusing on the key points. Be sure to refer to the full subject document for detailed instructions.)
