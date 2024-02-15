#ifndef __TERMINALMODE_H__
#define __TERMINALMODE_H__

#include "Terminal.hpp"

class Text : public Terminal::Mode
{
private:
    size_t posOffset;
    bool end, reseted = false;

protected:
    virtual void showInput(const Window &win);

public:
    Text(const string &content, bool end = false);
    virtual ~Text();

    virtual void set(const string &_content) override;
    virtual string input(const Window &win, int key) override;
    virtual void show(const Window &win) override;
};

class Password : public Text
{
protected:
    virtual void showInput(const Window &win) override;

public:
    Password(const string &content);
    virtual ~Password();
};

class Board : public Terminal::Mode
{
private:
    inline static const wchar_t *pion = L" ⬤";
    inline static const array<const wchar_t *, 2> wall = {L"━━━", L"┃"};
    inline static const array<const wchar_t *, 2> item = {L"💣", L"⚡"};
    // inline static const array<const wchar_t *, 11> grid = {L"┌", L"┐", L"└", L"┘", L"├", L"┤", L"┬", L"┴", L"┼", L"───", L"│"};
    inline static const array<const wchar_t *, 11> grid = {L"┌", L"┐", L"└", L"┘", L"├", L"┤", L"┬", L"┴", L"┼", L"───", L"│"};
    int rotation, rows, cols, currCol, currRow, currJoueur, modeJeu;
    char currMode;
    Point<> dir;
    array<vector<Point<>>, 4> pions;
    array<int, 4> wRemaining;                     // murs restant pour chaque joueur
    array<vector<const wchar_t *>, 4> iRemaining; // items restant pour chaque joueur
    vector<Point<>> walls;
    array<vector<Point<>>, 2> items;

    bool contains(int col, int row) { return 0 <= col && col < cols && 0 <= row && row < rows; }
    void axeToCoord(const Window &win, int &col, int &row);
    void printGrid(const Window &win);
    const wchar_t *axeToGrid(int row, int col);
    void gridCoordToCoord(const Window &win, int &col, int &row);
    void printWall(const Window &win, int col1, int row1, int col2, int row2);
    void printPion(const Window &win, int col, int row, int color);
    void printItem(const Window &win, int col, int row, const wchar_t *c);

    void showInput(const Window &win);

public:
    Board(const string &content);
    virtual ~Board();

    virtual void set(const string &_content) override;
    virtual string input(const Window &win, int key) override;
    virtual void show(const Window &win) override;
};

class Classement : public Terminal::Mode
{
private:
    size_t posOffset, rank, maxRank;

public:
    Classement(const string &content);
    virtual ~Classement();

    virtual void set(const string &_content) override;
    virtual string input(const Window &win, int key) override;
    virtual void show(const Window &win) override;
};

/**
 * @brief content format: [<message>\x1f\n]<option 1 menu>\n<option 2 menu>\n[<message>\x1f\n]<suite menu>\n...
 *
 * Les messages sont optionnels mais il faut ajouter \x1f après chaque lignes de message
 * pour qu'ils ne soient pas pris en compte dans les options du menu
 *
 */
class MMenu : public Terminal::Mode
{
private:
    int index, maxIndex;

public:
    MMenu(const string &content);
    virtual ~MMenu();

    virtual void set(const string &_content) override;
    virtual string input(const Window &win, int key) override;
    virtual void show(const Window &win) override;
};

#endif // __TERMINALMODE_H__
