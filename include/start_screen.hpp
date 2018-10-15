#ifndef START_SCREEN_H
#define START_SCREEN_H

class StartScreen : Screen
{
    Button start_game; // vai muda a tela
    Button exit; // muda a tela
    Button about;  // muda a tela
//    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
//    virtual void update(sf::Event& event) const;
    bind_start
};


#endif

void StartScreen()
{
    
}

void QuadBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    std::cout << "\n tela inicial desenhada.\n";
    // apply the transform
    states.transform *= getTransform();
    // apply texture
    states.texture = &m_tileset;
    // draw the vertex array
    target.draw(m_vertices, states);

}
