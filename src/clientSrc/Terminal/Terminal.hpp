#ifndef __INTER2_H__
#define __INTER2_H__

#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <signal.h>
#include "../Interface.hpp"
#define ctrl(x) ((x)&0x1f)

class Window
{
private:
    WINDOW *border, *main;
    int rows, cols, x, y;

public:
    constexpr operator WINDOW *() const { return main; }
    Window() : border{nullptr}, main{nullptr}, rows{-1}, cols{-1}, x{-1}, y{-1} {}
    Window(int row, int col, int x, int y)
        : border{newwin(row, col, y, x)}, main{newwin(row - 2, col - 2, y + 1, x + 1)},
          rows{row - 2}, cols{col - 2}, x{x + 1}, y{y + 1}
    {
        refresh();
    }
    Window(const Window &&other) = delete;
    Window(Window &other) = delete;
    Window(const Window &other) = delete;
    Window(Window &&other)
    {
        this->operator=((Window &&) other);
    }
    Window &operator=(const Window &&other) = delete;
    Window &operator=(Window &&other)
    {
        swap(border, other.border);
        swap(main, other.main);
        swap(rows, other.rows);
        swap(cols, other.cols);
        swap(x, other.x);
        swap(y, other.y);
        return *this;
    }
    virtual ~Window()
    {
        if (border && main)
        {
            delwin(border);
            delwin(main);
        }
    }
    void refresh() const
    {
        if (border && main)
        {
            box(border, 0, 0);
            wrefresh(border);
            wrefresh(main);
        }
    }
    void clear() const
    {
        if (border && main)
        {
            werase(border);
            werase(main);
            refresh();
        }
    }
    void resize(int row, int col, int x, int y)
    {
        if (border && main)
        {
            clear();
            mvwin(border, y, x);
            mvwin(main, y + 1, x + 1);
            wresize(border, row, col);
            wresize(main, row - 2, col - 2);
            rows = row - 2;
            cols = col - 2;
            this->x = x + 1;
            this->y = y + 1;
        }
    }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getX() const { return x; }
    int getY() const { return y; }
};

class Terminal : public Interface
{
public:
    class Mode
    {
    protected:
        string buffer = "";
        size_t cursor = 0;
        string content = "";

    public:
        Mode(const string &content) { set(content); }
        virtual ~Mode() {}

        virtual void set(const string &_content)
        {
            buffer.clear();
            cursor = 0;
            content = _content;
        }
        virtual string input(const Window &win, int key) = 0;
        virtual void show(const Window &win) = 0;
    };
    class Menu
    {
    public:
        
    protected:
        unique_ptr<Mode> mode;
        Terminal &terminal;
        mutex Mutex;
        bool deleted = false, inInput = false;

    public:
        Menu(Terminal &terminal) : terminal{terminal} {}
        virtual ~Menu()
        {
            deleted = true;
            while (inInput);
        }

        virtual void run() = 0;
        virtual string getInput(bool any = false);
    };
    friend class Menu;

private:
    Window writeWin, readWin;

    string pseudo = "Not connected yet";
    
    unique_ptr<Menu> currMenu;

    inline static unsigned int initncurses = 0;
    static void siginthandler(int value);

protected:
    virtual void handler() override;

public:
    Terminal();
    Terminal(Client *client);
    virtual ~Terminal();

    virtual const Window &getWriteWin() const { return writeWin; }
    virtual const Window &getReadWin() const { return readWin; }
    virtual Client *getClient() const { return client; }
    virtual void setCurrMenu(Menu *newMenu) { currMenu = unique_ptr<Menu>(newMenu); }
    virtual void resize();
    virtual void setPseudo(const string &pseudo) { this->pseudo = pseudo; }
    virtual void resetPseudo() { pseudo = "Not connected yet"; }
    virtual void showPseudo()
    {
        mvwprintw(writeWin, 0, 0, pseudo.c_str());
        wrefresh(writeWin);
    }
};

#endif // __INTER2_H__
