#include "TerminalMode.hpp"

Text::Text(const string &content, bool end)
    : Terminal::Mode(content), posOffset{end ? content.rfind('\n') + 1 : 0}, end{end}
{
    set(content);
}

Text::~Text()
{
}

void Text::set(const string &_content)
{
    Terminal::Mode::set(_content);
    while (content.back() == '\n')
        content.pop_back();
    reseted = true;
}

void Text::showInput(const Window &win)
{
    werase(win);
    mvwprintw(win, 0, 0, buffer.c_str());
    wattron(win, A_STANDOUT);
    mvwaddch(win, 0, cursor, mvwinch(win, 0, cursor));
    wattroff(win, A_STANDOUT);
    wrefresh(win);
}

string Text::input(const Window &win, int key)
{
    if (key < KEY_CODE_YES && isprint(key)) // KEY_CODE_YES = 256
    {
        buffer.insert(cursor, 1, (char)key);
        ++cursor;
        showInput(win);
    }
    else
    {
        switch (key)
        {
        
        case ERR:
            break;
        case '\n':
        case '\r':
        case KEY_ENTER:
        {
            if (!buffer.empty())
            {
                string tmp = buffer;
                buffer.clear();
                cursor = 0;
                showInput(win);
                return tmp;
            }
            break;
        }
        case KEY_BACKSPACE:
        case 8:
            if (cursor < 1)
                break;
            --cursor;
            buffer.erase(cursor, 1);
            showInput(win);
            break;
        case KEY_DC: // Delete
        case 127:
            if (cursor >= buffer.size())
                break;
            buffer.erase(cursor, 1);
            showInput(win);
            break;
        case KEY_RIGHT:
            if (cursor < buffer.size())
            {
                ++cursor;
                showInput(win);
            }
            break;
        case KEY_LEFT:
            if (cursor > 0)
            {
                --cursor;
                showInput(win);
            }
            break;
        case KEY_UP:
            posOffset = content.rfind('\n', posOffset != 0 ? posOffset - 2 : 0);
            posOffset = posOffset == string::npos ? 0 : posOffset + 1;
            break;
        case KEY_DOWN:
            posOffset = content.find('\n', posOffset);
            posOffset = posOffset == string::npos ? content.rfind('\n', string::npos) + 1 : posOffset + 1;
            break;
        default:
            showInput(win);
            break;
        }
    }
    return "";
}

void Text::show(const Window &win)
{
    werase(win);
    if (reseted)
    {
        if (!end)
            posOffset = 0;
        else
        {
            int count = 0, maxCount = win.getRows() - 1;
            size_t i = content.size() - 1;
            for (; i > 0 && count < maxCount; --i)
            {
                if (content[i] == '\n')
                    ++count;
            }
            posOffset = i;
        }
        reseted = false;
    }
    string line = content, sub{content.substr(posOffset)};
    int countLine = 0;
    for (size_t i; !line.empty(); ++countLine)
    {
        i = line.find('\n');
        line = line.substr(i == string::npos ? line.size() : i > (size_t)win.getCols() ? win.getCols()
                                                                                       : i + 1);
    }

    istringstream iss{sub};
    int l = static_cast<int>(win.getRows() / 2 - countLine / 2);
    for (l = (l < 0 ? 0 : l); getline(iss, line) && l < win.getRows();)
    {
        if (line.size() > (size_t)win.getCols())
        {
            while (!line.empty())
            {
                string tmp = line.substr(0, win.getCols());
                line = line.substr(line.size() > (size_t)win.getCols() ? win.getCols() : line.size());
                int c = static_cast<int>(win.getCols() / 2 - tmp.size() / 2);
                c = c < 0 ? 0 : c;
                mvwprintw(win, l, c, tmp.c_str());
                ++l;
            }
        }
        else
        {
            int c = static_cast<int>(win.getCols() / 2 - line.size() / 2);
            c = c < 0 ? 0 : c;
            mvwprintw(win, l, c, line.c_str());
            ++l;
        }
    }
    wrefresh(win);
}

Password::Password(const string &content)
    : Text(content)
{
}

Password::~Password()
{
}

void Password::showInput(const Window &win)
{
    werase(win);
    mvwprintw(win, 0, 0, string(buffer.size(), '*').c_str());
    wattron(win, A_STANDOUT);
    mvwaddch(win, 0, cursor, mvwinch(win, 0, cursor));
    wattroff(win, A_STANDOUT);
    wrefresh(win);
}

Board::Board(const string &content)
    : Terminal::Mode(content), rotation{0}, rows{9}, cols{9}, currCol{static_cast<int>(cols / 2)}, currRow{static_cast<int>(rows / 2)}, currJoueur{0}, currMode{'p'}, dir{0, 0}
{
    set(content);
}

Board::~Board()
{
}
/**
 * @brief
 *
 * @param _content format: <id joueur qui recoit les infos (1 -> 4)><nb pions joueur1 (0 -> infini)><positions pions joueur1 (si nbPionJoueur1 = 0 -> aucune position)><nb pions joueur2><positions pions joueur2><...><positions pions joueur4><nb murs restants joueur1>n<nb murs restants joueur2>n<...>n<nb murs restants joueur4><nb murs><positions murs>m<nb item1><positions item1><type item1(s ou t)><position item2><type item2><nb items joueur1>p<nb items joueur2>p<...>p<nb items joueur4>p
 */
void Board::set(const string &_content)
{
    string buff = "";

    const char *info = _content.c_str();

    pions[0].clear();
    pions[1].clear();
    pions[2].clear();
    pions[3].clear();
    walls.clear();
    items[0].clear();
    items[1].clear();
    iRemaining[0].clear();
    iRemaining[1].clear();
    iRemaining[2].clear();
    iRemaining[3].clear();

    // recupere les positions des pions de chaque joueur
    for (int i = 0; i < 4; ++i)
        for (int nbPion = *info++ - '0'; nbPion > 0; --nbPion)
            pions[i].push_back({*info++ - '0', *info++ - '0'});

    // recupere le nb de murs pour chaque joueur
    for (int i = 0; i < 4; ++i)
    {
        buff.clear();
        while (*info != 'n')
        {
            buff += *info++;
        }
        ++info;
        wRemaining[i] = stoi(buff);
    }
    buff.clear();

    while (*info != 'm')
        buff += *info++;
    ++info;
    // recupere les positions des murs déjà posé sur le plateau
    for (int nbWall = stoi(buff); nbWall > 0; --nbWall)
    {
        walls.push_back({*info++ - '0', *info++ - '0'});
        walls.push_back({*info++ - '0', *info++ - '0'});
    }

    // recupere les positions des items sur le plateau
    buff.clear();
    while (*info != 'i')
        buff += *info++;
    ++info; // recupere le nb d'items
    for (int nbItem = stoi(buff); nbItem > 0; --nbItem)
    {
        Point<> pos{*info++ - '0', *info++ - '0'};
        items[static_cast<int>(*info++ == 't')].push_back(pos);
    }

    // recupere le nb d'items pour chaque joueur
    for (int i = 0; i < 4; ++i)
    {
        buff.clear();
        while (*info != 'p')
            buff += *info++;
        ++info;
        for (int nbItem = stoi(buff); nbItem > 0; --nbItem)
            iRemaining[i].push_back(item[static_cast<int>(*info++ == 't')]);
    }

    currJoueur = *info++ - '0' - 1;
    modeJeu = *info++ - '0';
    rotation = *info++ - '0'; // id du joueur

    currRow = pions[rotation][0].y;
    currCol = pions[rotation][0].x;
}

void Board::showInput(const Window &win)
{
    werase(win);
    buffer = format("%c%c%d%c%d", currMode, 'a' + currRow, currCol + 1, 'a' + currRow + dir.y, currCol + dir.x + 1);
    mvwprintw(win, 0, 0, buffer.c_str());
    wrefresh(win);
}

string Board::input(const Window &win, int key)
{
    switch (key)
    {
    case KEY_UP:
        if (currMode == 'P' && contains(currCol + dir.x, currRow + dir.y - 1))
            dir.y -= 1;
        else if ((currMode != 'P') && currRow > 0)
            --currRow;
        showInput(win);
        break;
    case KEY_RIGHT:

        if (currMode == 'P' && contains(currCol + dir.x + 1, currRow + dir.y))
            dir.x += 1;
        else if (((currMode == 'p' || currMode == 's') && contains(currCol + 1, currRow)) || (currMode == 'm' && currCol < cols - 2))
            ++currCol;
        showInput(win);
        break;
    case KEY_DOWN:
        if (currMode == 'P' && contains(currCol + dir.x, currRow + dir.y + 1))
            dir.y += 1;
        else if ((currMode == 'p' && contains(currCol, currRow + 1)) || ((currMode == 'm' || currMode == 's') && currRow < rows - 2))
            ++currRow;
        showInput(win);
        break;
    case KEY_LEFT:

        if (currMode == 'P' && contains(currCol + dir.x - 1, currRow + dir.y))
            dir.x -= 1;
        else if ((currMode != 'P') && currCol > 0)
            --currCol;
        showInput(win);
        break;
    case 'p':
    case 'm':
    case 's':
        currMode = key;
        currRow = currMode == 'p' ? pions[rotation][0].y : static_cast<int>(rows / 2);
        currCol = currMode == 'p' ? pions[rotation][0].x : static_cast<int>(rows / 2);
        dir.x = 0;
        dir.y = (currMode == 'm' || currMode == 's') ? 1 : 0;
        showInput(win);
        break;
    case 'r':
        if (currMode == 'm' || currMode == 's')
        {
            dir.x = dir.x == 1 ? 0 : 1;
            dir.y = dir.y == 1 ? 0 : 1;
            showInput(win);
        }
        break;
    case 'x':
        return "pause";
    case 'f':
        return "forfait";
    case KEY_ENTER:
    case '\n':
    {
        if (currMode == 'p')
        {
            for (auto &pos : pions[rotation])
            {
                if (pos.x == currCol && pos.y == currRow)
                {
                    currMode = 'P';
                    break;
                }
            }
            break;
        }
        currMode = currMode == 'P' ? 'p' : currMode; // change pour le message tmp
        string tmp = format("%c%c%d%c%d", currMode, 'a' + currRow, currCol + 1, 'a' + currRow + dir.y, currCol + dir.x + 1);
        currMode = 'p';
        currRow = pions[rotation][0].y; // static_cast<int>(rows / 2);
        currCol = pions[rotation][0].x; // static_cast<int>(cols / 2);
        dir.x = 0;
        dir.y = 0;
        showInput(win);
        return tmp;
    }
    default:
        showInput(win);
        break;
    }
    return "";
}

void Board::show(const Window &win)
{
    werase(win);
    printGrid(win);

    for (int i = 0; i < 4; ++i)
        for (auto &coord : pions[i])
            printPion(win, coord.x, coord.y, ((i + 4 - rotation) % 4) + 1);

    for (size_t i = 0; i < walls.size(); i += 2)
        printWall(win, walls[i].x, walls[i].y, walls[i + 1].x, walls[i + 1].y);

    for (int i = 0; i < 2; ++i)
        for (auto &coord : items[i])
            printItem(win, coord.x, coord.y, item[i]);

    int row = currRow, col = currCol;
    if (currMode == 'p' || currMode == 'P')
    {

        axeToCoord(win, col, row);
        wchar_t *buff = (wchar_t *)malloc(3);
        mvwinnwstr(win, row, col, buff, 3);
        chtype var = mvwinch(win, row, col);
        wattron(win, currMode == 'P' ? A_NORMAL : A_REVERSE);
        mvwaddwstr(win, row, col, buff);
        wattroff(win, currMode == 'P' ? A_NORMAL : A_REVERSE);
        if (currMode == 'P')
        {
            row = currRow + dir.y;
            col = currCol + dir.x;

            axeToCoord(win, col, row);
            wattron(win, var & A_COLOR);
            mvwaddwstr(win, row, col, buff);
            wattroff(win, var & A_COLOR);
        }
        delete buff;
    }
    else
    {
        wattron(win, A_REVERSE);

        col = col * 2 + dir.x + 1, row = row * 2 + dir.y + 1;
        if (row % 2 == 0 && col % 2 == 1)
        {
            gridCoordToCoord(win, col, row);
            mvwaddwstr(win, row, col, L"═══");
            if (currMode != 's')
                mvwaddwstr(win, row, col + 4, L"═══");
        }
        else
        {

            gridCoordToCoord(win, col, row);
            mvwaddwstr(win, row, col, L"║");
            if (currMode != 's')
                mvwaddwstr(win, row + 2, col, L"║");
        }
        wattroff(win, A_REVERSE);
    }
    for (int i = 0; i < 4; ++i)
    {
        if (pions[i].size() == 0)
            continue;
        int rrow = i, ccol = cols + 1;
        axeToCoord(win, ccol, rrow);
        string b = format("walls: %d%s", wRemaining[i], modeJeu ? " | items: " : "");
        printPion(win, cols + 1, i, ((i + 4 - rotation) % 4) + 1);
        mvwprintw(win, rrow, ccol + 4, b.c_str());
        if (modeJeu)
            for (size_t j = 0; j < iRemaining[i].size(); ++j)
                mvwaddwstr(win, rrow, ccol + 4 + b.size() + (j * 2), iRemaining[i][j]);
        if (i == rotation)
            mvwprintw(win, rrow, ccol + b.size() + 4 + (iRemaining[i].size() * 2) + 1, "<= vous");
        if (i == currJoueur)
            mvwprintw(win, rrow, ccol - 2, "=>");
    }
    mvwprintw(win, win.getRows() - 2, 0, "p = mode pion | m = mode mur (r = rotation mur) | f = déclarer forfait/quitter la partie | x = mettre la partie en pause (| s = supprimer un mur si vous avez l'item pour) curseur blanc = position départ | curseur bleu = position arrivée");
    wrefresh(win);
}

void Board::axeToCoord(const Window &win, int &col, int &row)
{
    col = col * 2 + 1;
    row = row * 2 + 1;
    gridCoordToCoord(win, col, row);
}

void Board::gridCoordToCoord(const Window &win, int &col, int &row)
{
    col = col % 2 == 0 ? static_cast<int>(col / 2) * 4 : static_cast<int>((col - 1) / 2) * 4 + 1;
    int rowP = row, colP = col;
    row = static_cast<int>(static_cast<float>(win.getRows()) / 2.0 - (static_cast<float>(rows) * 2.0 + 1.0) / 2.0 + static_cast<float>(row));
    col = static_cast<int>(static_cast<int>(static_cast<float>(win.getCols()) / 2.0) - static_cast<int>((static_cast<float>(cols + 1) + static_cast<float>(cols) * 4.0) / 2.0) + static_cast<float>(col));
    row = row < 0 ? rowP : row;
    col = col < 0 ? colP : col;
}

const wchar_t *Board::axeToGrid(int row, int col)
{

    if (row == 0 && col == 0)
        return grid[0];
    else if (row == 0 && col == cols * 2)
        return grid[1];
    else if (row == rows * 2 && col == 0)
        return grid[2];
    else if (row == rows * 2 && col == cols * 2)
        return grid[3];
    else if (row % 2 == 0 && col == 0)
        return grid[4];
    else if (row % 2 == 0 && col == cols * 2)
        return grid[5];
    else if (row == 0 && col % 2 == 0)
        return grid[6];
    else if (row == rows * 2 && col % 2 == 0)
        return grid[7];
    else if (row % 2 == 0 && col % 2 == 0)
        return grid[8];
    else if (row % 2 == 0 && col % 2 == 1)
    {
        if (row == 0 || row == rows * 2)
            return grid[9];
        else
            return L"   ";
    }
    else if (row % 2 == 1 && col % 2 == 0)
    {
        if (col == 0 || col == cols * 2)
            return grid[10];
        else
            return L" ";
    }
    return L" "; // case
}

void Board::printGrid(const Window &win)
{
    for (int row = 0; row < ((rows * 2) + 1); ++row)
    {
        for (int col = 0; col < ((cols * 2) + 1); ++col)
        {
            int x = col, y = row;

            gridCoordToCoord(win, x, y);
            mvwaddwstr(win, y, x, axeToGrid(row, col));
        }
    }
}

void Board::printWall(const Window &win, int col1, int row1, int col2, int row2)
{
    int col = col1 + col2 + 1, row = row1 + row2 + 1;

    if (row % 2 == 0 && col % 2 == 1)
    {
        gridCoordToCoord(win, col, row);
        mvwaddwstr(win, row, col, wall[0]);
    }
    else
    {
        gridCoordToCoord(win, col, row);
        mvwaddwstr(win, row, col, wall[1]);
    }
}

void Board::printItem(const Window &win, int col, int row, const wchar_t *c)
{
    axeToCoord(win, col, row);
    mvwaddwstr(win, row, col, c);
}

void Board::printPion(const Window &win, int col, int row, int color)
{

    axeToCoord(win, col, row);
    wattron(win, COLOR_PAIR(color));
    mvwaddwstr(win, row, col, pion);
    wattroff(win, COLOR_PAIR(color));
}

Classement::Classement(const string &content)

    : Terminal::Mode(content), posOffset{0}, rank{1}, maxRank{0}
{
    set(content);
}

Classement::~Classement()
{
}

void Classement::set(const string &_content)
{
    while (content.back() == '\n')
        content.pop_back();
    maxRank = count(content.begin(), content.end(), '\n');
}

string Classement::input(const Window &win, int key)
{
    werase(win);
    switch (key)
    {
    case KEY_UP:
        posOffset = content.rfind('\n', posOffset != 0 ? posOffset - 2 : 0);
        posOffset = posOffset == string::npos ? 0 : posOffset + 1;
        if (rank > 1)
            --rank;
        break;
    case KEY_DOWN:
        posOffset = content.find('\n', posOffset);
        posOffset = posOffset == string::npos ? content.rfind('\n', string::npos) + 1 : posOffset + 1;
        if (rank <= maxRank)
            ++rank;
        break;

    case KEY_ENTER:
    case '\n':
        wrefresh(win);
        return "0";
    default:
        break;
    }
    wrefresh(win);
    return "";
}

void Classement::show(const Window &win)
{
    werase(win);
    int spaces = (win.getCols() - 7) / 3;
    int lenght = spaces * 3 + 7 - 2;
    mvwprintw(win, 0, 0, " %s ", string(lenght, '-').c_str());
    wattron(win, A_STANDOUT);
    mvwprintw(win, 1, 0, "| %-*s| %-*s| %-*s|", spaces, "Rank", spaces, "Pseudo", spaces, "Score");
    wattroff(win, A_STANDOUT);
    istringstream inClss{content.substr(posOffset), ios::in};
    string pseudo = "";
    int score, timestamp;
    int line = 2;
    for (int r = rank; line < win.getRows() - 1; ++line)
    {
        if (line % 2 == 0)
            mvwprintw(win, line, 0, " %s ", string(lenght, '-').c_str());
        else
        {
            if (inClss >> pseudo >> score >> timestamp)
            {
                mvwprintw(win, line, 0, "| %-*d| %-*s| %-*d|", spaces, r, spaces, pseudo.c_str(), spaces, score);
                ++r;
            }
            else
                break;
        }
    }
    if (line % 2 == 0)
        mvwprintw(win, line, 0, " %s ", string(lenght, '-').c_str());

    mvwprintw(win, win.getRows() - 1, 0, "(Utilisez les flèches pour faire défiler ou appuyer sur enter pour revenir en arrière)");
    wrefresh(win);
}

MMenu::MMenu(const string &content)
    : Terminal::Mode(content), index{1}, maxIndex{1}
{
    set(content);
}

MMenu::~MMenu()
{
}

void MMenu::set(const string &_content)
{
    Terminal::Mode::set(_content);
    while (content.back() == '\n')
        content.pop_back();
    index = 1;
    maxIndex = count(content.begin(), content.end(), '\n') - count(content.begin(), content.end(), '\x1f') + 1;
}

string MMenu::input(const Window &win, int key)
{
    werase(win);
    switch (key)
    {
    case KEY_UP:
        if (index > 1)
            --index;
        break;
    case KEY_DOWN:
        if (index < maxIndex)
            ++index;
        break;
    case KEY_ENTER:
    case '\n':
    case '\r':
        wrefresh(win);
        return to_string(index);
    }
    wrefresh(win);
    return "";
}

void MMenu::show(const Window &win)
{
    werase(win);
    wattroff(win, A_STANDOUT);
    istringstream iss{content, ios::in};
    string line = "";

    int l = static_cast<int>(win.getRows() / 2 - count(content.begin(), content.end(), '\n') / 2);
    l = l < 0 ? 0 : l;
    for (int i = 1; getline(iss, line) && l < win.getRows(); ++l)
    {
        if (line.back() != '\x1f')
        {
            if (i == index)
                wattron(win, A_STANDOUT);
            ++i;
        }
        else
            line.pop_back();

        int r = static_cast<int>(win.getCols() / 2.0f - line.size() / 2.0f);
        r = r < 0 ? 0 : r;
        mvwprintw(win, l, r, line.c_str());
        wattroff(win, A_STANDOUT);
    }
    wrefresh(win);
}
