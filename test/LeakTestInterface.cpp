#include "../src/clientSrc/Interface.hpp"
#include "../src/clientSrc/Client.hpp"
#include "../src/serveur/Serveur.hpp"
// #include "../src/clientSrc/Inter2.hpp"
#include <unistd.h>

// struct Client {
// void Client::sendMessage(std::string m){return;}
// };
void test_interface1()
{
    unique_ptr<Interface> i = make_unique<Interface>();

    i->showBoard("150158002415148582345", 0);
    i->showBoard("150158002415148582345", 1);
    i->showClass("a");
    i->showClass("b");

    i->showMenu("hello\n1.hello\n2.test\n3.je ais pas\n4.tt");
    i->showMenu("hello2\n1.hello\n2.test\n3.je ais pas\n4.tt");
    i->showPassword("fghjkl");
    i->showPassword(";lkjhgf");
    i->showText("hello");
    i->showText("hello2");
}

// void test_interface2(){
//     unique_ptr<Interface> i = make_unique<Interface2>(nullptr);

// }

void test_Client1(){
    unique_ptr<Client> i = make_unique<Client>("127.0.0.1", 8080);
    i->connect();
    i->run(nullptr);
}

void runServeur(shared_ptr<Serveur> s){
    s->run(8080);
}

void runClient(){
    cout << "rc" << endl;

    sleep(1);
    unique_ptr<Client> i = make_unique<Client>("127.0.0.1", 8080);
    i->connect();
    sleep(1);

    i->sendMessage("1");
    cout << "connecting" << endl;
    sleep(1);

    i->sendMessage("Test");
    sleep(1);

    i->sendMessage("Test");
    cout << "connected" << endl;
    // i->sendMessage("\0");
    // i->sendMessage("")
    cout << "fc" << endl;

}


void test_Serveur(){
    cout << "t" << endl;
    shared_ptr<Serveur> s = make_shared<Serveur>();

    thread Tserveur, Tclient;
    Tserveur = std::thread(&runServeur, s);
    Tclient = std::thread(&runClient);
    Tclient.join();
    // exit();
    s->stop();
    Tserveur.join();

    // cout << "s" << endl;

}

int main(){
    // test_Client1();
    cout << "?" << endl;
    // runServeur();
    test_Serveur();

}