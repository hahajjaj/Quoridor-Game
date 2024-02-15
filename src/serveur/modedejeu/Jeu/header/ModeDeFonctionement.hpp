#ifndef __MODE_SERVEUR_HPP
# define __MODE_SERVEUR_HPP


class ModeDeFonctionement
{
private:
    /* data */
public:
    ModeDeFonctionement();
    ~ModeDeFonctionement();
    void getMessage();
    void sendMessage();
    void modifieDataBase();
    void getFromDataBase();
};

#endif

