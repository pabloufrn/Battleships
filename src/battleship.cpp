#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include "quadboard.h"
#include "button.h"

#define TENTATIVAS 25

typedef struct
{
	char val;
	char dir;
	char bai;
}Ponto;

const int size = 10;
int navios[] = {4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1};
int totalships = 0;
Ponto **board;
QuadBoard *qboard;
sf::Text text;
int tentativas = TENTATIVAS;

enum
{
	LAT = 1,
	CIR, // Submarine
	MEI_VER,
    MEI_HOR,
	PON_ESQ,
	PON_DIR,
	PON_CIM,
	PON_BAI,
    // Destoyed part 
	SCIR,
	SMEI_VER,
    SMEI_HOR,
	SPON_ESQ,
	SPON_DIR,
	SPON_CIM,
	SPON_BAI,
	SWATER
};

using namespace std;

void fill_board()
{
	int i, j;
	board = (Ponto**) malloc(sizeof(Ponto**)*(size+1));
	for(i = 0; i < size; ++i)
	{
		board[i] = (Ponto*) calloc(size+1, sizeof(Ponto));
		for(j = 0; j < size; ++j)
		{
			board[i][j].val = 0;
			board[i][j].dir = size-j-1;
			board[i][j].bai = size-i-1;
		}
	}
	board[size] = (Ponto*) calloc(size+1, sizeof(Ponto));
	for(i = 0; i < size; ++i)
		board[i][size].val = 0;
	for(j = 0; j < size; ++j)
		board[size][j].val = 0;
	return;
}

void pos_ship_h(int i, int j, int navio)
{
	int x, y;

    // PREENCHE O NAVIO, AS LATERIAS, ATUALIZA VALOR BAI ACIMA DAS LATERAIS E ZERA DIR NAS LATERAIS
	for(y = j; y < j + navio; y++)
    {
        // COLOCA A PEÇA ATUAL DO NAVIO
		if(navio > 1)
        {
			if(y == j)
            {
				board[i][y] = {PON_ESQ, 0, 0};
            }
			else if(y == j+navio-1)
            {
				board[i][y] = {PON_DIR, 0, 0};
            }
			else
            {
				board[i][y] = {MEI_HOR, 0, 0};
            }
        }
        else
        {
            board[i][y] = {CIR, 0, 0};
        }
        
        // COLOCA A LATERAL DE CIMA
		if(i > 0)
        {
			board[i-1][y].val = LAT;
            board[i-1][y].bai = 0;
            if(y  != j + navio - 1) // so atualiza dir se não for a ultima lateral
            {
                board[i-1][y].dir = 0;
            }
		}
		
		// COLOCA A LATERAL DE BAIXO
		if(i < size-1)
        {
			board[i+1][y].val = LAT;
            if(y  != j + navio -1) // so atualiza dir se não for a ultima lateral
            {
                board[i+1][y].dir = 0;
            }
        }
        
        // ATUALIZA O VALOR DE BAI ACIMA DA LATERAL
        // SE FOR UMA PEÇA DE NAVIO NÃO ATUALIZA
        if(i - 2 >= 0 and board[i-2][y].val <= LAT)
        {
            for(x = 2; i - x >= 0; x++)
            {
                board[i-x][y].bai = x-2;
                if(board[i-x][y].val == LAT)
                {
                    break;
                }
            }
        }
	}
	
	// ATUALIZA O VALOR DIR ANTES DA LATERAL ESQUERDA, se não for uma parte de navio
	//    ---- 
	//xxx-<==>-
	//    ----
	if(j > 1 and board[i][j-2].val <= LAT)
    {
        for(y = 2; j-y >=0; y++)
        {
            board[i][j-y].dir = y-2;
            if(board[i][j-y].val == LAT)
            {
                break; // achou uma lateral, para a contagem
            }
        }
        
	}
	
	// ATUALIZA O VALOR DIR ANTES DA LATERAL DE CIMA, se não for uma parte de navio
	// xxx---- 
	//   -<==>-
	//    ----
	if(i > 0 and j > 0 and board[i-1][j-1].val <= LAT)
    {
        for(y = 1; j-y >=0; y++)
        {
            board[i-1][j-y].dir = y-1;
            if(board[i-1][j-y].val == LAT)
            {
                break; // achou uma lateral, para a contagem
            }
        }
        
	}
	
	// ATUALIZA O VALOR DIR ANTES DA LATERAL DE BAIXO, se não for uma parte de navio
	//    ---- 
	//   -<==>-
	// xxx----
	if(i < size-1 and j > 0 and board[i+1][j-1].val <= LAT)
    {
        for(y = 1; j-y >=0; y++)
        {
            board[i+1][j-y].dir = y-1;
            if(board[i+1][j-y].val == LAT)
            {
                break; // achou uma lateral, para a contagem
            }
        }
        
	}
	// ATUALIZA O VALOR BAI ACIMA DA LATERAL ESQUERDA, se não for uma parte de navio
    //   x
    //   x----
	//   -<==>- 
	
	if(i > 0 and j > 0 and board[i-1][j-1].val <= LAT)
    {
        for(x = 1; i-x >=0; x++)
        {
            board[i-x][j-1].bai = x-1;
            if(board[i-x][j-1].val == LAT)
            {
                break;
            }
        }
    }
    
	// ATUALIZA O VALOR BAI ACIMA DA LATERAL DIREITA, se não for uma parte de navio
    //        x
    //    ----x
	//   -<==>- 
	if(j+navio < size and i > 0 and board[i-1][j+navio].val <= LAT)
    {
        for(x = 1; i-x >=0; x++)
        {
            board[i-x][j+navio].bai = x-1;
            if(board[i-x][j+navio].val == LAT)
            {
                break;
            }
        }
    }
    // COLOCA A LATERAL DA ESQUERDA
    if(j > 0)
    {
        board[i][j-1].val = LAT;
        board[i][j-1].dir = 0;
    }
    // COLOCA A LATERAL DA DIREITA
    if(j + navio < size)
    {
        board[i][j+navio].val = LAT;
    }
}	

void pos_ship_v(int i, int j, int navio)
{
	int x, y;
    
    // PREENCHE O NAVIO, AS LATERIAS, ATUALIZA VALOR DIR ANTES DAS LATERAIS E ZERA BAI NAS LATERAIS
	for(x = i; x < i + navio; x++)
    {
        // COLOCA A PEÇA ATUAL DO NAVIO
		if(navio > 1)
        {
			if(x == i)
            {
				board[x][j] = {PON_CIM, 0, 0};
            }
			else if(x == i+navio-1)
            {
				board[x][j] = {PON_BAI, 0, 0};
            }
			else
            {
				board[x][j] = {MEI_VER, 0, 0};
            }
        }
        else
        {
            board[x][j] = {CIR, 0, 0};
        }
        
        // COLOCA A LATERAL DA ESQUERDA
		if(j > 0)
        {
			board[x][j-1].val = LAT;
            board[x][j-1].dir = 0;
            if(x != i + navio - 1) // so atualiza dir se não for a ultima lateral
            {
                board[x][j-1].bai = 0;
            }
		}
		
		// COLOCA A LATERAL DA DIREITA
		if(j < size-1)
        {
			board[x][j+1].val = LAT;
            if(x != i + navio - 1) // so atualiza bai se não for a ultima lateral
            {
                board[x][j+1].bai = 0;
            }
        }
        
        // ATUALIZA O VALOR DE DIR ANTES DA LATERAL
        // SE FOR UMA PEÇA DE NAVIO NÃO ATUALIZA
        if(j - 2 >= 0 and board[x][j-2].val <= LAT)
        {
            for(y = 2; j - y >= 0; y++)
            {
                board[x][j-y].dir = y-2;
                if(board[x][j-y].val == LAT)
                {
                    break;
                }
            }
        }
        
	}
	
	// ATUALIZA O VALOR BAI ACIMA DA LATERAL CIMA, se não for uma parte de navio
	//    ----  
	//xxx-<==>- IMAGINE ESSA IMAGEM VIRADA 90º À DIREITA
	//    ----  
	if(i > 1 and board[i-2][j].val <= LAT)
    {
        for(x = 2; i-x >=0; x++)
        {
            board[i-x][j].bai = x-2;
            if(board[i-x][j].val == LAT)
            {
                break; // achou uma lateral, para a contagem
            }
        }
        
	}
	
	// ATUALIZA O VALOR BAI ACIMA DA LATERAL DIREITA, se não for uma parte de navio
	// xxx---- 
	//   -<==>- IMAGINE ESSA IMAGEM VIRADA 90º À DIREITA
	//    ----
	if(j < size - 1 and i > 0 and board[i-1][j+1].val <= LAT)
    {
        for(x = 1; i-x >= 0; x++)
        {
            board[i-x][j+1].bai = x-1;
            if(board[i-x][j+1].val == LAT)
            {
                break; // achou uma lateral, para a contagem
            }
        }
        
	}
	
	// ATUALIZA O VALOR BAI ACIMA DA LATERAL ESQUERDA, se não for uma parte de navio
	//    ---- 
	//   -<==>-
	// xxx----
	if(j > 0 and i > 0 and board[i-1][j-1].val <= LAT)
    {
        for(x = 1; i-x >= 0; x++)
        {
            board[i-x][j-1].bai = x-1;
            if(board[i-x][j-1].val == LAT)
            {
                break; // achou uma lateral, para a contagem
            }
        }
        
	}
	// ATUALIZA O VALOR DIR ANTES DA LATERAL DE CIMA, se não for uma parte de navio
    //   x
    //   x----
	//   -<==>- 
	if(i > 0 and j > 0 and board[i-1][j-1].val <= LAT)
    {
        for(y = 1; j-y >= 0; y++)
        {
            board[i-1][j-y].dir= y-1;
            if(board[i-1][j-y].val == LAT)
            {
                break;
            }
        }
    }
    
	// ATUALIZA O VALOR DIR ANTES DA LATERAL DE BAIXO, se não for uma parte de navio
    //        x
    //    ----x
	//   -<==>- 
	if(i+navio < size and j > 0 and board[i+navio][j-1].val <= LAT)
    {
        for(y = 1; j-y >= 0; y++)
        {
            board[i+navio][j-y].dir = y-1;
            if(board[i+navio][j-y].val == LAT)
            {
                break;
            }
        }
    }
    
    // COLOCA A LATERAL DE CIMA
    if(i > 0)
    {
        board[i-1][j].val = LAT;
        board[i-1][j].bai = 0;
    }
    // COLOCA A LATERAL DE BAIXO
    if(i + navio < size)
    {
        board[i+navio][j].val = LAT;
    }
}

void pos_ships()
{
	int i, j;
	bool ok = false;
	for(int & navio: navios)
	{
		random_device rd; 
		mt19937 rng(rd());   
		uniform_int_distribution<int> uni(0,9);
		auto x = uni(rng);
		auto y = uni(rng);
		i = x + 1;
		j = y + 1;
		ok = false;
		if(i == size)i = 0;
		for(; i <= size && !ok; i++)
		{
			if(i == x && y == j)break;
			if(i == size)i = 0;
			if(j == size)j = 0;
			for(; j < size; j++)
			{
				if(i == x && y == j)break;
				if(board[i][j].dir >= navio-1)
				{
					if(board[i][j].val == 0)
					{
						pos_ship_h(i, j, navio);
					}
					else if(board[i][j].dir >= navio)
					{
						pos_ship_h(i, j+1, navio);
					}
					ok = true;
					break;
				}
				else if(board[i][j].bai >= navio-1)
				{
					if(board[i][j].val == 0)
					{
						pos_ship_v(i, j, navio);
					}
					else if(board[i][j].bai >= navio)
					{
						pos_ship_v(i+1, j, navio);
					}
					ok = true;
					break;
				}
			}
		}
#ifdef DEBUG
		if(!ok)
            std::cout << "Não foi possível posicionar um navio de tamanho " << navio << "\n";
#endif
	}
	return;
}

void count_ships()
{
	int i, j;
	for(i = 0; i < size; i++)
	{
		for(j = 0; j < size; j++)
		{
			if(board[i][j].val > LAT)
			{
				board[size][j].val++;
				board[i][size].val++;
				totalships++;
			}
		}
	}
	return;
}

void print_board()
{
	int i, j;
	system("clear");
	cout << "- - - - - - - - - -\n";
	cout << "- - - - - - - - - -\n";
	for(i = 0; i < size; ++i)
	{
		for(j = 0; j < size; ++j)
		{
			if(board[i][j].val == MEI_HOR)cout << "= "; 	
            else if(board[i][j].val == MEI_VER)cout << "I ";
			else if(board[i][j].val == CIR)cout << "O "; 	
			else if(board[i][j].val == PON_ESQ)cout << "< "; 	
			else if(board[i][j].val == PON_DIR)cout << "> "; 	
			else if(board[i][j].val == PON_CIM)cout << "M "; 	
			else if(board[i][j].val == PON_BAI)cout << "W "; 	
// 			else if(board[i][j].val == SWATER)cout << "X "; 
			else cout << "  "; 	
		}
		cout << "| " << ((int) board[i][size].val) << " | ";
		cout << "- " << i << endl;
	}

	cout << "- - - - - - - - - -\n";
	for(j = 0; j < size; ++j)
		cout << (int) board[size][j].val << " ";
	cout << "\n-------------------\n";

	for(i = 0; i < size; ++i)
		cout << i << " ";
	cout << endl;
	cout << endl;
	return;
}

void change_quad_status(unsigned int& i, unsigned int& j)
{
    char v = board[i][j].val;
    switch(v)
    {
        case SCIR:
            qboard->set_quad_texture(i, j, 1);
            if((i + j) % 4 != 0)
                qboard->rotate_quad_texture(i, j, (i + j) % 4);
            return;
        case SMEI_VER:
            qboard->set_quad_texture(i, j, 2);
            return;
        case SMEI_HOR:
            qboard->set_quad_texture(i, j, 2);
            qboard->rotate_quad_texture(i, j, 1);
            return;
        case SPON_ESQ:
            qboard->set_quad_texture(i, j, 0);
            qboard->rotate_quad_texture(i, j, 1);
            return;
        case SPON_DIR:
            qboard->set_quad_texture(i, j, 0);
            qboard->rotate_quad_texture(i, j, 3);
            return;
        case SPON_CIM:
            qboard->set_quad_texture(i, j, 0);
            return;
        case SPON_BAI:
            qboard->set_quad_texture(i, j, 0);
            qboard->rotate_quad_texture(i, j, 2);
            return;
        case SWATER:
            qboard->set_quad_color(i, j, sf::Color(150, 150, 150));
            qboard->update_original_hover_color(sf::Color(150, 150, 150));
            return;
        default:
            return;
    }
}
bool shoot(unsigned int& x, unsigned int& y)
{
	if(x >= size or y >= size or board[x][y].val > PON_BAI)
		return false;
	if(board[x][y].val > LAT)
	{
		board[x][y].val += 7;
		totalships--;
        change_quad_status(x, y);
		return true;
	}
	text.setString("Tentativas: " + std::to_string(--tentativas));
	board[x][y].val = SWATER;
    change_quad_status(x, y);
	return true;

}



void free_board()
{
	int i;
	for(i = 0; i < size + 1; ++i)
		free(board[i]);
	free(board);
}

void play_again()
{
    fill_board();
    pos_ships();
    count_ships();
#ifdef DEBUG
    print_board();
#endif
    tentativas = TENTATIVAS;
    text.setString("Tentativas: " + std::to_string(tentativas));
    qboard->clear();
    return;
}

int main()
{
	// TODO: botao para mudar tamanho
	unsigned int boardsize = 10;
	unsigned int quadsize = 50;
	unsigned int margin = 0;
	
	// initalize instances
	qboard = new QuadBoard(quadsize, {boardsize, boardsize}, margin, "img/tileset.png", {2, 2});	
    
    fill_board();
    pos_ships();
    count_ships();
    
#ifdef DEBUG
    print_board();
#endif
    qboard->bind_click(shoot);
    
    // Window elements
    int window_width = boardsize*quadsize + boardsize*margin;
    int window_height = window_width + 40;
    
    sf::Font font;
    font.loadFromFile("font/arial.ttf");
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(20);
    text.setPosition(10, window_height-30);
    text.setString("Tentativas: " + std::to_string(tentativas));
    
    sf::Text tendgame;
    tendgame.setFont(font);
    tendgame.setFillColor(sf::Color::White);
    tendgame.setCharacterSize(40);
    
    Button playagain("Jogar novamente", sf::Vector2f(0 , window_height/2 + 100), 30, play_again, sf::Color(50, 200, 50));
    playagain.move(window_width/2 - playagain.getRightX()/2, 0);
    
	// window config
	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Battleships Game", sf::Style::Titlebar + sf::Style::Close);
    
	// main window draw loop
	while (window.isOpen())
	{
        sf::Event e;
        
        if(totalships == 0)
        {
            tendgame.setString(L"Você ganhou");
            tendgame.setPosition(window_width/2 -  tendgame.getLocalBounds().width/2, window_height/2 - 40);
            tentativas = 0;
        }
        else if(tentativas == 0)
        {
            tendgame.setString("Game Over");
            tendgame.setPosition(window_width/2 -  tendgame.getLocalBounds().width/2, window_height/2 - 40);
        }
       
        while(tentativas == 0 and window.isOpen()) // atualizado ao pressionar o botão
        {
            window.clear();
            window.draw(tendgame);
            window.draw(playagain);
            window.display();
            while (window.pollEvent(e))
            {
                /// check if button close was pressed
                if(e.type == sf::Event::Closed)
                {
                    window.close();
                    break;

                }
                playagain.update(e);
            }
            continue;
        }
        
		/// event prossessing
		while (window.pollEvent(e))
		{
			/// check if button close was pressed
			if(e.type == sf::Event::Closed)
			{
				window.close();
				break;

			}
			/// pass the event to the window elements
			qboard->update(e);
		}
		/// show all elements in the window
		window.clear();
        window.draw(text);
		window.draw(*qboard);
		window.display();
	}
	free_board();
}

 
 
 
 
