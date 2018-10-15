#include "quadboard.h"

void QuadBoard::init(size_t quad_size, sf::Vector2u dimensions, size_t margin)
{
    m_quad_size = quad_size;
    m_dimensions= dimensions;
    m_margin = margin;
    m_is_enable = true;
    m_original_hover_color = sf::Color::White;
    m_mouse_hover_index = {-1, -1};
    // resize the vertex array to fit the board size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(dimensions.x * dimensions.y * 4); // 4 vertices * width cols * height rows
    // populate the vertex array
    for (unsigned int i = 0; i < dimensions.y; ++i)
    {
        for (unsigned int j = 0; j < dimensions.x; ++j)
        {
            // get a pointer to the current board's quad
            int index = (i * dimensions.x + j) * 4;
            sf::Vertex* quad = &m_vertices[index];
            // define its 4 corners
            quad[0].position = sf::Vector2f(j * quad_size + j * margin, i * quad_size + i * margin);
            quad[1].position = sf::Vector2f((j + 1) * quad_size + j * margin, i * quad_size + i * margin);
            quad[2].position = sf::Vector2f((j + 1) * quad_size + j * margin, (i + 1) * quad_size + i * margin);
            quad[3].position = sf::Vector2f(j * quad_size + j * margin, (i + 1) * quad_size + i * margin);
        }
    }
    return;
}
QuadBoard::QuadBoard(size_t quad_size, sf::Vector2u dimensions, size_t margin, sf::Color default_color, sf::Color active_color)
{
    init(quad_size, dimensions, margin);
    // set proprieters
    m_default_color = default_color;
    m_active_color = active_color;
    // populate the vertex array
    for (unsigned int i = 0; i < dimensions.y; ++i)
    {
        for (unsigned int j = 0; j < dimensions.x; ++j)
        {
            // get a pointer to the current board's quad
            int index = (i * dimensions.x + j) * 4;
            sf::Vertex* quad = &m_vertices[index];
            // define vertice colors
            quad[0].color = default_color;
            quad[1].color = default_color;
            quad[2].color = default_color;
            quad[3].color = default_color;
        }
    }
}
QuadBoard::QuadBoard(size_t quad_size, sf::Vector2u dimensions, size_t margin, const std::string & tileset_file, sf::Vector2u tileset_dimesions)
{
    // initialize board
    init(quad_size, dimensions, margin);
    // load texture
    if (!m_tileset.loadFromFile(tileset_file))
            throw std::runtime_error("Nao foi possivel criar a textura.");
    // proprieters
    m_tileset.setSmooth(true);
    m_tile_size.x = m_tileset.getSize().x / tileset_dimesions.x;
    m_tile_size.y = m_tileset.getSize().y / tileset_dimesions.y;
    m_default_color = sf::Color::White;
    // populate the vertex array
    for (unsigned int i = 0; i < dimensions.y; ++i)
    {
        for (unsigned int j = 0; j < dimensions.x; ++j)
        {
            // get a pointer to the current board's quad
            int index = (i * dimensions.x + j) * 4;
            sf::Vertex* quad = &m_vertices[index];
            // define vertice texture
            quad[0].texCoords = sf::Vector2f(m_tile_size.x, m_tile_size.y);
            quad[1].texCoords = sf::Vector2f(2 * m_tile_size.x, m_tile_size.y);
            quad[2].texCoords = sf::Vector2f(2 * m_tile_size.x, 2 * m_tile_size.y);
            quad[3].texCoords = sf::Vector2f(m_tile_size.x, 2 * m_tile_size.y);
        }
    }
}

void QuadBoard::set_quad_color(unsigned int x, unsigned int y, sf::Color color)
{
    // get a pointer to the quad
    sf::Vertex* quad = &m_vertices[(x * m_dimensions.x + y) * 4];
    // define vertice colors
    quad[0].color = color;
    quad[1].color = color;
    quad[2].color = color;
    quad[3].color = color;
}
void QuadBoard::update_original_hover_color(sf::Color color)
{
    m_original_hover_color = color;
    return;
}
void QuadBoard::set_quad_texture(unsigned int & x, unsigned int & y, unsigned int tileNumber)
{
    /* coordenadas relativas no tileset
     * Em um tileset 2x2 as coordenadas sao:
     * tilenumber | tu | tv
     *     0      | 0  | 0
     *     1      | 1  | 0
     *     2      | 0  | 1
     *     3      | 1  | 1
     */
    unsigned int tu = tileNumber / (m_tileset.getSize().x / m_tile_size.x);
    unsigned int tv = tileNumber % (m_tileset.getSize().x / m_tile_size.x);

    // get a pointer to the current tile's quad
    sf::Vertex* quad = &m_vertices[(x * m_dimensions.x + y) * 4];

    // define its 4 texture coordinates
    quad[0].texCoords = sf::Vector2f(tv * m_tile_size.x, tu * m_tile_size.y);
    quad[1].texCoords = sf::Vector2f((tv + 1) * m_tile_size.x, tu * m_tile_size.y);
    quad[2].texCoords = sf::Vector2f((tv + 1) * m_tile_size.x, (tu + 1) * m_tile_size.y);
    quad[3].texCoords = sf::Vector2f(tv * m_tile_size.x, (tu + 1) * m_tile_size.y);
    
}

void QuadBoard::rotate_quad_texture(unsigned int & x, unsigned int & y, unsigned int rotation)
{
    sf::Vertex* quad = &m_vertices[(x * m_dimensions.x + y) * 4];
    sf::Vertex aux_quad[4];
    aux_quad[0] = quad[0];
    aux_quad[1] = quad[1];
    aux_quad[2] = quad[2];
    aux_quad[3] = quad[3];
    
    for(auto i(0u); i < 4; ++i, ++rotation)
    {
        if(rotation > 3)
        {
            quad[i].texCoords = aux_quad[rotation-4].texCoords;
        }
        else
        {
            quad[i].texCoords = aux_quad[rotation].texCoords;
        }
    }
}

void QuadBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();
    // apply texture
    states.texture = &m_tileset;
    // draw the vertex array
    target.draw(m_vertices, states);

}

void QuadBoard::update(sf::Event& e)
{
    if(!m_is_enable)return;
    
    if (e.type == sf::Event::MouseMoved){
        unsigned int j = e.mouseMove.x / (m_quad_size+m_margin);
        unsigned int i = e.mouseMove.y / (m_quad_size+m_margin); 
        
        if(i >= m_dimensions.y or j >= m_dimensions.x)
        {
            if(m_mouse_hover_index.x == -1)
                return;
            set_quad_color(m_mouse_hover_index.x, m_mouse_hover_index.y, m_original_hover_color);
            m_mouse_hover_index = {-1, -1};
            return;
        }
        
        if((int) i != m_mouse_hover_index.y or (int) j != m_mouse_hover_index.x)
        {
            if(m_mouse_hover_index.x != -1)
                set_quad_color(m_mouse_hover_index.x, m_mouse_hover_index.y, m_original_hover_color);
            m_mouse_hover_index = {static_cast <int> (i), static_cast <int> (j)};
            m_original_hover_color = m_vertices[(i * m_dimensions.x + j) * 4].color;
            set_quad_color(i, j, sf::Color(200, 200, 200));
        }
        
    }
    else if (e.type == sf::Event::MouseButtonPressed and e.mouseButton.button == sf::Mouse::Left){
        click(e.mouseButton.x, e.mouseButton.y);
    }
    return;
}

void QuadBoard::clear()
{
    if(!m_is_enable)return;
    m_original_hover_color = m_default_color;
    m_mouse_hover_index = {0, 0};
    for (unsigned int i = 0; i < m_dimensions.y; ++i)
            for (unsigned int j = 0; j < m_dimensions.x; ++j)
            {
                sf::Vertex* quad = &m_vertices[(i * m_dimensions.x + j) * 4];
                quad[0].color = m_default_color;
                quad[1].color = m_default_color;
                quad[2].color = m_default_color;
                quad[3].color = m_default_color;
                if(m_tile_size.x > 0)
                {
                    int tu = 1;
                    int tv = 1;
                    quad[0].texCoords = sf::Vector2f(m_tile_size.x, m_tile_size.y);
                    quad[1].texCoords = sf::Vector2f(2 * m_tile_size.x, tv * m_tile_size.y);
                    quad[2].texCoords = sf::Vector2f(2 * m_tile_size.x, 2 * m_tile_size.y);
                    quad[3].texCoords = sf::Vector2f(tu * m_tile_size.x, 2 * m_tile_size.y);
                }
    }
}

bool QuadBoard::get_active(unsigned int & x, unsigned int & y)
{
    // get a pointer to the quad
    sf::Vertex* quad = &m_vertices[(x * m_dimensions.x + y) * 4];
    // check color
    if(quad[0].color == m_default_color)return false;
    return true;
}
void QuadBoard::bind_click(bool (*function) (unsigned int &, unsigned int &))
{
    m_on_click = function;
}
void QuadBoard::click(unsigned int x, unsigned int y)
{
    unsigned int j = x / (m_quad_size+m_margin); // !
    unsigned int i = y / (m_quad_size+m_margin); // | get the quad index. The column depends of x_axis of mouse
    if(m_on_click)
        m_on_click(i, j);
    return;
}

/*
 * TODO:
 * - metodo para mudar textura do quadrado, que tambem recebe o angulo (0, 90, 180, 270)
 */



