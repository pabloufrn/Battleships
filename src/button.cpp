#include "button.h"

/*!
 * \param textstr button text
 * \param position position (x, y) vector
 * \param height height of the button
 * \param on_press fuction that will be called whe button is pressed
 * \param default_color background button color
 */
Button::Button(std::string textstr, const sf::Vector2f &position, size_t height, void (*on_press)(), sf::Color default_color)
{
    
    /// text config
    this->font.loadFromFile("font/arial.ttf");
    this->text.setFont(this->font);
    this->text.setFillColor(sf::Color::White);
    this->text.setCharacterSize(height*0.8);
    this->text.setPosition(position.x+0.5*height, position.y-0.05*height);
    this->text.setString(textstr);
    /// rectangle config
    sf::FloatRect frect = text.getGlobalBounds();
    this->size = sf::Vector2f(frect.width + height, height);
    this->rect.setSize(this->size);
    this->position = position;
    this->rect.setPosition(position);
    this->rect.setFillColor(default_color);
    /// others configs
    this->state = BT_ENABLE;
    this->on_press = on_press;
}
/// draw methode of Button. Check the SFML Documentation
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();
    // draw button
    target.draw(this->rect, states);
    target.draw(this->text, states);
}
/// x_axis maximum of the button area
unsigned int Button::getRightX()
{
   return this->position.x+this->size.x;
}
/// button pressed
void Button::press()
{
    // TODO::change button color
    this->on_press();   
}
/// update state
void Button::update(sf::Event& e)
{
    /// if the button is already desable, return
    if(this->state == BT_DISABLE)return;
    /// check if the mouse is over button
    if(e.type == sf::Event::MouseMoved)
    {
        /// check the distance of the mouse and button 
        int offsetx = e.mouseMove.x - this->position.x;
        int offsety = e.mouseMove.y - this->position.y;
        /// if mouse is outside of button area and button state is BT_HOVER, change its state to ENABLE and return
        if(offsetx < 0 || offsetx > this->size.x || offsety < 0 || offsety > this->size.y) 
        {
            if(this->state == BT_HOVER) /// change color
            {
                this->state = BT_ENABLE;
                sf::Color color = this->rect.getFillColor();
                color.a = 255;
                this->rect.setFillColor(color);
            }
            return;
        }
        // inside button area
        if(this->state == BT_HOVER)return;
        // hover button
        this->state = BT_HOVER;
        sf::Color color = this->rect.getFillColor();
        color.a = 100;
        this->rect.setFillColor(color);
        return;
    }
    if(e.type == sf::Event::MouseButtonPressed and e.mouseButton.button == sf::Mouse::Left)
    {
        int offset = e.mouseButton.x - this->position.x;
        if(offset < 0 || offset > this->size.x)return;
        offset = e.mouseButton.y - this->position.y;
        if(offset < 0 || offset > this->size.y)return;
        this->press();
    }
}
/// relative move
void Button::move(int rx, int ry)
{
     sf::Vector2f pos;
     pos = this->text.getPosition();
     pos.x += rx;
     pos.y += ry;
     this->text.setPosition(pos.x, pos.y);
     pos = this->rect.getPosition();
     pos.x += rx;
     pos.y += ry;
     this->rect.setPosition(pos.x, pos.y);
     this->position = pos;
}













