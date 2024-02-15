#ifndef __TERMINALMENU_H__
#define __TERMINALMENU_H__

#include "Terminal.hpp"

class MenuLogin : public Terminal::Menu
{
private:
public:
    MenuLogin(Terminal &terminal);
    virtual ~MenuLogin();

    virtual void run() override;
};

class MenuPrincipal : public Terminal::Menu
{
private:
public:
    MenuPrincipal(Terminal &terminal);
    virtual ~MenuPrincipal();

    virtual void run() override;
};

class MenuAmis : public Terminal::Menu
{
private:
public:
    MenuAmis(Terminal &terminal);
    virtual ~MenuAmis();

    virtual void run() override;
};

class MenuJoue : public Terminal::Menu
{
private:
    inline static string vecToStrInvite(const vector<string> &invite)
    {
        if (invite.empty())
            return "";
        string ret = invite[0];
        for (size_t i = 1; i < invite.size(); ++i)
        {
            ret += ", " + invite[i];
        }
        return ret;
    }
public:
    MenuJoue(Terminal &terminal);
    virtual ~MenuJoue();

    virtual void run() override;
};

class Game : public Terminal::Menu
{
private:
public:
    Game(Terminal &terminal);
    virtual ~Game();

    virtual void run() override;
};

#endif // __TERMINALMENU_H__