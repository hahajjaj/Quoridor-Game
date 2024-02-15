#include "Terminal.hpp"
#include "TerminalMenu.hpp"

string Terminal::Menu::getInput(bool any)
{
    inInput = true;
    if (deleted || !terminal.isRunning())
    {
        inInput = false;
        return "";
    }
    int key = 0;
    string buff = mode->input(terminal.getReadWin(), key);
    mode->show(terminal.getWriteWin());
    do
    {
        if (deleted)
            break;
        key = wgetch(terminal.getReadWin());
        switch (key)
        {
        case ERR:
            continue;
        case KEY_RESIZE:
            terminal.resize();
            continue;
        case ctrl('d'):
            terminal.getClient()->disconnect();
            inInput = false;
            return "";
        case ctrl('u'):
            terminal.showPseudo();
            continue;
        case 27: // ESCAPE
            if (wgetch(terminal.getReadWin()) == ERR) // ESCAPE
            {
                inInput = false;
                return "";
            }
            break; // ALT + [key]
        }
        buff = mode->input(terminal.getReadWin(), key);
        mode->show(terminal.getWriteWin());
        if (any)
            break;
    } while (terminal.isRunning() && buff.empty());
    inInput = false;
    return buff;
}

void Terminal::siginthandler(int value)
{
    if (value == SIGINT)
    {
        clear();
        endwin();
    }
    initncurses = 0;
    exit(value);
}

Terminal::Terminal(Client *client)
    : Interface(client)
{
    
}

Terminal::~Terminal()
{
    if (initncurses && !isendwin())
    {
        clear();
        endwin();
        --initncurses;
    }
}

void Terminal::resize()
{
    writeWin.resize(LINES - 3, COLS, writeWin.getX(), writeWin.getX());
    readWin.resize(readWin.getRows(), COLS, readWin.getX(), LINES - readWin.getRows());
}

void Terminal::handler()
{
    if (!initncurses)
    {
        // cout << "initscr" << endl;
        initscr();
        signal(SIGINT, Terminal::siginthandler);
        ++initncurses;
    }
    
    setlocale(LC_ALL, "");
    writeWin = Window{LINES - 3, COLS, 0, 0};
    readWin = Window{3, COLS, 0, LINES - 3};
    keypad(readWin, true); // Ctrl+D == 4
    cbreak();
    
    wtimeout(readWin, 20); // surcharge pas le cpu
    noecho();
    curs_set(false);
    set_escdelay(0);

    use_default_colors();
    start_color();
    init_pair(1, COLOR_BLUE, -1);
    init_pair(2, COLOR_RED, -1);
    init_pair(3, COLOR_GREEN, -1);
    init_pair(4, COLOR_YELLOW, -1);
    init_pair(5, COLOR_BLACK, COLOR_BLUE);
    init_pair(6, COLOR_BLACK, COLOR_WHITE);
    currMenu = make_unique<MenuLogin>(*this);
    while (status && currMenu)
    {
        currMenu->run();
    }
    writeWin = Window{};
    readWin = Window{};
    if (initncurses && !isendwin())
    {
        clear();
        endwin();
        --initncurses;
    }
}
