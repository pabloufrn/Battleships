#ifndef QUADBOARD_H
#define QUADBOARD_H
#include <SFML/Graphics.hpp>

class QuadBoard : public sf::Drawable, public sf::Transformable
{
public:
    /*! QuadBoard constructor
     * \param quad_size size in pixels of each quad of the board
     * \param width width in number of cols of the board
     * \param height height in number of rows of the board
     * \param magin space in pixel between quad
     * \param default_color color that will be displayed when the quad is not activated
     * \param activate color that will be displayed when the quad is activated
     */
    void init(size_t quad_size, sf::Vector2u dimensions, size_t margin);
    QuadBoard(size_t quad_size, sf::Vector2u dimensions, size_t margin, sf::Color default_color = sf::Color::Green, sf::Color active_color = sf::Color::Yellow);
    QuadBoard(size_t quad_size, sf::Vector2u dimensions, size_t margin, const std::string & tileset_file, sf::Vector2u tileset_dimesions);
    
    /*! Change quad color
     * \param x row of board
     * \param y column of board
     * \param color new color
     */
    void set_quad_color(unsigned int x, unsigned int y, sf::Color color);
    /*! Change default color of the square the mouse is hover.
     * \param color new color
     */
    void update_original_hover_color(sf::Color color);
    /*! Change quad texture
     * \param x row of board
     * \param y column of board
     * \param tileNumber number of sprite on the tileset
     */
    void set_quad_texture(unsigned int & x, unsigned int & y, unsigned int tileNumber);
    /*! Change quad texture
     * \param x row of board
     * \param y column of board
     * \param rotation 1, 2, 3
     */
    void rotate_quad_texture(unsigned int & x, unsigned int & y, unsigned int rotation);
    /*! Change quad state to activated
     * \param x row of board
     * \param y column of board
     */
    void click(unsigned int x, unsigned int y);
    void bind_click(bool (*function) (unsigned int &, unsigned int &));
    /// Update board
    void update(sf::Event & e);
    /// Clear board
    void clear();
    /*! Get board element state
     * \param x row of board
     * \param y column of board
     */
    bool get_active(unsigned int & x, unsigned int & y);
    /// Get size in pixels of each quad
    size_t get_quad_size(){return m_quad_size;}
    /// Get space in pixels between quads
    size_t get_margin(){return m_margin;}
    /// Dont update board
    void disable(){m_is_enable = false;}
    /// Enable board
    void enable(){m_is_enable = true;}
    /// Check if the board is disable
    bool is_disable(){return not m_is_enable;}
    
private:
    /// Draw methode, check SFML docs
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    size_t m_quad_size;
    sf::Vector2u m_dimensions;
    sf::Vector2u m_tile_size;
    size_t m_margin;
    
    sf::Color m_active_color;
    sf::Color m_default_color;
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    sf::Color m_original_hover_color;
    sf::Vector2i m_mouse_hover_index;
    
    bool (*m_on_click) (unsigned int &, unsigned int &);
    bool m_is_enable;
    
};
#endif
