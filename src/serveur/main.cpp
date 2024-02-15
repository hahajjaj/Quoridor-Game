#include "Serveur.hpp"

int main(int argc, char **argv)
{
	if (argc > 2 || argc < 2)
	{
		cerr << "Bad usage:\n\t./serveur <port>\n"; 
        return EXIT_FAILURE;
	}
	else if (!atoi(argv[1])) 
    { 
        cerr << "Bad argument: port need to be a number\n"; 
        return EXIT_FAILURE;
    }
	Serveur s;
	s.run(atoi(argv[1]));
	return EXIT_SUCCESS;
}