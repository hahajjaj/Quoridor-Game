
#include "Client.hpp"
#include "Terminal/Terminal.hpp"
#include "Interface_GUI.hpp"
// #include <X11/Xlib.h>

int main(int argc, const char *argv[])
{
    if (argc < 3 || argc > 5)
    {
        cerr << "Bad usage:\n\t./client <address ip> <port> [terminal/gui : default = terminal]\n";
        return EXIT_FAILURE;
    }
    else if (!atoi(argv[2]))
    {
        cerr << "Bad argument: port need to be a number\n";
        return EXIT_FAILURE;
    }
    else if (argc == 3 || !strcmp(argv[3], "terminal"))
    {

        Client c{argv[1], atoi(argv[2])};
        if (!c.connect())
        {
            cerr << "Aucun serveur connecté à l'addresse '" << argv[1] << "' sur le port " << argv[2] << "\n";
            return EXIT_FAILURE;
        }
        c.sendMessage("gui");
        Terminal t(&c);
        c.run(&t);
    }
    else if (argc == 4 && !strcmp(argv[3], "gui"))
    {
        // XInitThreads();
        Client c{argv[1], atoi(argv[2])};
        if (!c.connect())
        {
            cerr << "Aucun serveur connecté à l'addresse '" << argv[1] << "' sur le port " << argv[2] << "\n";
            return EXIT_FAILURE;
        }
        c.sendMessage("gui");
        GUI i(&c);
        c.run(&i);
    }
    else
    {
        cerr << "Bad usage:\n\t./client <address ip> <port> [terminal/gui : default = terminal]\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
