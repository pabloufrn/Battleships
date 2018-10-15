#ifndef SCREEN_H
#define SCREEN_H

class Screen
{
    public:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void update(sf::Event& event) const;
   
    private:
        bool enable;
};

/* lembrete: o botao iniciar ira chamar um metodo da classe game */
#endif

