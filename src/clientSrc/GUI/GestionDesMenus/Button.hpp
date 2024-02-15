#ifndef __BUTTON_HPP
#define __BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class Button
{
private:
    float echelle;
    sf::Texture *texture_bouton;
    sf::Texture *texture_bouton_selec;
    
    sf::Sprite sprite_boutton;
    sf::Sprite sprite_boutton_selec;
    sf::Sprite *currentSprite;
    sf::Text text;
    sf::Font *font;
    std::string nom_bouton = "";
    int id = -1;

    int btnWidth;
    int btnHeight;
    sf::Vector2f bouton_size;


public:
    Button(float echelle, std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Font *font, sf::Texture *texture_bouton, sf::Texture *texture_bouton_selec): echelle(echelle), texture_bouton(texture_bouton), texture_bouton_selec(texture_bouton_selec)
    {
        // sprite_boutton.setTexture(*texture_bouton);
        // sprite_boutton.scale(sf::Vector2f(buttonSize.x/texture_bouton->getSize().x, buttonSize.y/texture_bouton->getSize().y));

        this->bouton_size.x = echelle * buttonSize.x;
        this->bouton_size.y = echelle * buttonSize.y;

        sprite_boutton.setTexture(*texture_bouton);
        sprite_boutton.scale((echelle *buttonSize.x)/ texture_bouton->getSize().x, (echelle *buttonSize.y)/texture_bouton->getSize().y);

        sprite_boutton_selec.setTexture(*texture_bouton_selec);
        sprite_boutton_selec.scale((echelle *buttonSize.x)/ texture_bouton_selec->getSize().x, (echelle *buttonSize.y)/texture_bouton_selec->getSize().y);

        // Get these for later use:
        btnWidth = (echelle *buttonSize.x);
        btnHeight = (echelle *buttonSize.y);

        text.setFont(*font);
        text.setString(btnText);
        text.setCharacterSize((echelle *charSize));
        text.setFillColor(sf::Color::White);
        currentSprite = &sprite_boutton;
    }

    sf::Vector2f getSize(){
        return bouton_size;
    }

    std::string getNomCase(){
        return text.getString();
    }

    sf::Vector2f getPosition(){
        return currentSprite->getPosition();
    }

    void setNomBouton(std::string nom){
        nom_bouton = nom;
    }

    void setTexture(sf::Texture *text, sf::Texture *text2, sf::Vector2f size){
        btnWidth = echelle * size.x;
        btnHeight = echelle * size.y;
        sprite_boutton = sf::Sprite();
        sprite_boutton.setTexture(*text);
        sprite_boutton.setScale((float)(echelle *size.x)/(float)text->getSize().x, (float)(echelle *size.y)/(float)text->getSize().y);
        sprite_boutton_selec = sf::Sprite();
        sprite_boutton_selec.setTexture(*text2);
        sprite_boutton_selec.setScale((float)btnWidth/(float)text2->getSize().x, (float)btnHeight/(float)text2->getSize().y);
    }

    std::string getNomBouton(){
        return nom_bouton;
    }

    void setText(std::string str)
    {
        text.setString(str);
    }

    void setSize(sf::Vector2f size)
    {
    }

    void setCharSize(int charSize)
    {
        text.setCharacterSize(charSize);
    }

    // Pass font by reference:
    void setFont(sf::Font &fonts)
    {
        text.setFont(fonts);
    }

    void setBackColor(sf::Color color)
    {
    }

    void setTextColor(sf::Color color)
    {
        text.setFillColor(color);
    }

    void setPosition(sf::Vector2f point)
    {
        sprite_boutton.setPosition((point.x), (point.y));
        sprite_boutton_selec.setPosition((point.x), (point.y));

        // Center text on button:
        float xPos = ((point.x) + btnWidth / 2) - (text.getLocalBounds().width / 2);
        float yPos = ((point.y) + btnHeight / 3) - (text.getLocalBounds().height / 2);
        text.setPosition(xPos, yPos);
    }

    void drawTo(sf::RenderWindow &window)
    {
        // window.draw(button);
        window.draw(*currentSprite);
        window.draw(text);
    }
    int getId(){
        return id;
    }
    void setId(int new_id){
        id = new_id;
    }

    // Check if the mouse is within the bounds of the button:
    bool isMouseOver(int mouseX, int mouseY)
    {
        if(contains(mouseX, mouseY)){
            currentSprite = &sprite_boutton_selec;
            return true;
        }else{
            currentSprite = &sprite_boutton;
            return false;
        }
    }

    bool contains(int mouseX, int mouseY){

        int btnPosX = currentSprite->getPosition().x;
        int btnPosY = currentSprite->getPosition().y;

        int btnxPosWidth = currentSprite->getPosition().x + currentSprite->getGlobalBounds().width;
        int btnyPosHeight = currentSprite->getPosition().y + currentSprite->getGlobalBounds().height;

        if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY)
        {
            return true;
        }
        return false;
    }
};

#endif