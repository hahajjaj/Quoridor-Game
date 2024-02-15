#ifndef __TEXTBOX_HPP
#define __TEXTBOX_HPP

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

// Define keys:
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class Textbox
{
public:
    Textbox(int size, sf::Color color, bool sel)
    {
        // rectangle.setSize(sf::Vector2f(100, 20));
        // rectangle.setPosition(sf::Vector2f(100, 100));
        // rectangle.setFillColor(sf::Color::White);
        textbox.setCharacterSize(size);
        textbox.setFillColor(color);
        isSelected = sel;

        // Check if the textbox is selected upon creation and display it accordingly:
        if (isSelected)
            textbox.setString("_");
        else
            textbox.setString("");
    }

    Textbox();

    Textbox(float echelle, sf::Vector2f size, sf::Vector2f pos, sf::Color bgcolor, sf::Color textcolor, sf::Texture *texture_search, bool password = false)
    {
        this->echelle = echelle;
        this->texture_search = *texture_search;
        sprite_search.setTexture(this->texture_search);
        sprite_search.scale((echelle * size.x) / this->texture_search.getSize().x, (echelle * size.y) / this->texture_search.getSize().y);
        sprite_search.setPosition(pos);
        this->size = echelle * size;
        this->password = password;
        textbox.setPosition(sf::Vector2f(pos.x + size.x / 20, pos.y));
        textbox.setCharacterSize(((echelle * size.y * 7)) / 10);
        textbox.setFillColor(textcolor);
        textbox.setString("_");
        setSelected(false);
    }

    void setCharText(int size){
        textbox.setCharacterSize(echelle * size);
    }

    void setSize(sf::Vector2f size)
    {
        rectangle.setSize(size);
    }

    void setBackColor(sf::Color color)
    {
        rectangle.setFillColor(color);
    }

    void setTextColor(sf::Color color)
    {
        textbox.setFillColor(color);
    }

    bool isMouseOver(int mouseX, int mouseY)
    {
        int btnPosX = sprite_search.getPosition().x;
        int btnPosY = sprite_search.getPosition().y;

        int btnxPosWidth = sprite_search.getPosition().x + size.x;
        int btnyPosHeight = sprite_search.getPosition().y + size.y;

        if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY)
        {
            return true;
        }
        return false;
    }

    // Make sure font is passed by reference:
    void setFont(sf::Font &fonts)
    {
        textbox.setFont(fonts);
    }

    void setPosition(sf::Vector2f point)
    {
        textbox.setPosition(sf::Vector2f(point.x + size.x / 20, point.y));
        sprite_search.setPosition(point);
    }

    // Set char limits:
    void setLimit(bool ToF)
    {
        hasLimit = ToF;
    }

    void setLimit(bool ToF, int lim)
    {
        hasLimit = ToF;
        limit = lim - 1;
    }

    // Change selected state:
    void setSelected(bool sel)
    {
        isSelected = sel;

        std::string t = text.str();
        std::string newT;
        for (size_t i = 0; i < t.length(); i++)
        {
            if(password){
                newT += "*";
            }else{
                newT += t[i];
            }
        }
        if (!isSelected)
        {
            newT+= "";
        }
        else
        {
            newT += "_";
        }
        textbox.setString(newT);
        
        
    }

    std::string getText()
    {
        return text.str();
    }

    void drawTo(sf::RenderWindow &window)
    {
        // window.draw(rectangle);
        window.draw(sprite_search);
        window.draw(textbox);
    }

    // Function for event loop:
    void typedOn(sf::Event input)
    {
        if (isSelected)
        {
            int charTyped = input.text.unicode;

            // Only allow normal inputs:
            if (charTyped < 128)
            {
                if (hasLimit)
                {
                    // If there's a limit, don't go over it:
                    if (text.str().length() <= limit)
                    {
                        inputLogic(charTyped);
                    }
                    // But allow for char deletions:
                    else if (text.str().length() > limit && charTyped == DELETE_KEY)
                    {
                        deleteLastChar();
                    }
                }
                // If no limit exists, just run the function:
                else
                {
                    inputLogic(charTyped);
                }
            }
        }
    }
    void empty(){
        text.str("");
        textbox.setString("");
    }

private:
    float echelle;
    sf::Text textbox;
    sf::RectangleShape rectangle;
    sf::Texture texture_search;
    sf::Sprite sprite_search;
    std::ostringstream text;
    sf::Vector2f size;
    bool isSelected = false;
    bool hasLimit = false;
    size_t limit = 0;
    bool password = false;

    // Delete the last character of the text:
    void deleteLastChar()
    {
        std::string t = text.str();
        std::string newT = "";
        for (size_t i = 0; i < t.length() - 1; i++)
        {
            newT += t[i];
        }
        text.str("");
        text << newT;
        textbox.setString(text.str() + "_");
    }

    // Get user input:
    void inputLogic(int charTyped)
    {
        // If the key pressed isn't delete, or the two selection keys, then append the text with the char:
        if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY)
        {
            text << static_cast<char>(charTyped);
        }
        // If the key is delete, then delete the char:
        else if (charTyped == DELETE_KEY)
        {
            if (text.str().length() > 0)
            {
                deleteLastChar();
            }
        }
        // Set the textbox text:
        if (password)
        {
            std::string password = "";
            for (size_t i = 0; i < text.str().length(); i++)
                password += "*";
            textbox.setString(password + "_");
        }
        else
        {
            textbox.setString(text.str() + "_");
        }
    }
};

#endif