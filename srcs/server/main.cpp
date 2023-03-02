
#include "server.hpp"


int main()
{
    try
    {
        server  s;

        s.run();
    }
    catch(std::string const & msg)
    {
        std::cout << msg << '\n';
    }

    return 0;
}