#include <iostream>
#include <random>

typedef struct
{
	char val;
	char dir;
	char bai;
}Ponto;

const int size = 10;
int navios[] = {4, 4, 3, 3, 2, 2, 2};
int totalships = 0;
Ponto **board;

enum
{
	LAT = 1,
	CIR,
	MEI,
	PON_ESQ,
	PON_DIR,
	PON_CIM,
	PON_BAI,
    SCIR,
	SMEI,
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
	if(navio == 1)
    {
		board[i][j] = {CIR, 0, 0};
		if(i < size-1)
			board[i+1][j].val = LAT;
		if(i > 0)
			board[i-1][j].val = LAT;
		if(j < size-1)
			board[i][j+1].val = LAT;
		if(j > 0)
        {
			board[i][j-1].val = LAT;
			board[i][j-1].dir = 0;
		}

	}
	else{
		if(j > 0)
        {
			board[i][j-1].val = LAT;
			board[i][j-1].dir = 0;
		}
    }
	int x, y, c;
	for(y = 2; j - y >= 0; y++) // esquerda da lateral esquerda do navio
    {
		if(board[i][j-y].val == LAT)
        {
            board[i][j-y].dir = y-2;
            break;
        }
        else
        {
			board[i][j-y].dir = y-2;
        }
    }
	// lateral cima
	if(i > 0)
    {
        // valor da direita (lateral de cima)
		for(y = 0;  y < navio-1; y++)
			board[i-1][j+y].dir = 0;
        // valor da direita (antes da lateral de cima)
		for(y = 1; j-y >=0; y++)
        {
			if(board[i-1][j-y].val == LAT)
            {
                board[i-1][j-y].dir = y-1;
				break; // achou navio, para a contagem
            }
			else
            {
				board[i-1][j-y].dir = y-1;
            }
        }
	}
    // lateral de baixo
    if(i < size-1)
    {
        // valor da direita (lateral de baixo)
		for(y = 0;  y < navio-1; y++)
			board[i+1][j+y].dir = 0;
        // valor da direita (antes da lateral de baixo)
		for(y = 1; j-y >=0; y++)
        {
			if(board[i+1][j-y].val == LAT)
            {
                board[i+1][j-y].dir = y-1;
				break; // achou navio, para a contagem
            }
			else
            {
				board[i+1][j-y].dir = y-1;
            }
        }
	}
	// acima da lateral da esquerda, atualiza valor baixo
	if(j > 0)
    {
        for(x = 1; i - x >= 0; x++)
        {
                if(board[i-x][j-1].val == LAT)
                {
                    board[i-x][j-1].bai = x-1;
                    break;
                }
                else 
                {
                    board[i-x][j-1].bai = x-1;
                }
        }
    }
                
	for(y = j; y < j + navio; y++) // preencher o navio, calcular valor baixo
    {
		if(navio > 1)
        {
			if(y == j)
				board[i][y] = {PON_ESQ, 0, 0};
			else if(y == j+navio-1)
				board[i][y] = {PON_DIR, 0, 0};
			else
				board[i][y] = {MEI, 0, 0};
        }
		if(i > 0) // lateral de cima na coluna atual
        {
			board[i-1][y].val = LAT;
			board[i-1][y].bai = 0;
		}
		if(i < size-1) // lateral de baixo na coluna atual
			board[i+1][y].val = LAT;
        // atualiza valor baixo para y (coluna) atual
        for(c = 2; i - c >= 0; c++)
        {
            if(board[i-c][y].val == LAT)
            {
                board[i-c][y].bai = c-2;
                break;
            }
            else
            {
                board[i-c][y].bai = c-2;
            }
        }
	}
	// limites do barco
	// lateral direita
	if(j+navio < size)
    {
        for(x = 1; i-x >=0; x++)
        {
            if(board[i-x][j+navio].val == LAT)
            {
                board[i-x][j+navio].bai = x-1;
                break;
            }
            else
            {
                board[i-x][j+navio].bai = x-1;
            }
        }
    }
    // lateral esquerda
    if(j > 0)
    {
        for(x = 1; i-x >=0; x++)
        {
            if(board[i-x][j-1].val == LAT)
            {
                board[i-x][j-1].bai = x-1;
                break;
            }
            else
            {
                board[i-x][j-1].bai = x-1;
            }
        }
    }
    if(j + navio < size)
        board[i][j+navio].val = LAT;
    return;
}	

void pos_ship_v(int i, int j, int navio) // logica replicada
{
	if(navio == 1)
    {
		board[i][j] = {CIR, 0, 0};
		if(i < size-1)
			board[i+1][j].val = LAT;
		if(i > 0)
        {
			board[i-1][j].val = LAT;
            board[i-1][j].bai = 0;
        }
		if(j < size-1)
			board[i][j+1].val = LAT;
		if(j > 0)
			board[i][j-1].val = LAT;
	}
	else
    {
		if(i > 0)
        {
			board[i-1][j].val = LAT;
		}
    }
	int x, y, c;
	for(x = 2; i - x >= 0; x++)
    {
		if(board[i-x][j].val == LAT)
        {
            board[i-x][j].bai = x-2;
            break;
        }
        else
        {
			board[i-x][j].bai = x-2;
        }
    }
	if(j > 0)
    {
		for(x = 0;  x < navio-1; x++)
			board[i+x][j-1].bai = 0;
		for(x = 1; i-x >=0; x++)
        {
			if(board[i-x][j-1].val == LAT)
            {
                board[i-x][j-1].bai = x-1;
				break;
            }
			else
            {
				board[i-x][j-1].bai = x-1;
            }
        }
	}
    if(j < size-1)
    {
		for(x = 0;  x < navio-1; x++)
			board[i+x][j+1].bai = 0;
		for(x = 1; i-x >=0; x++)
        {
			if(board[i-x][j+1].val == LAT)
            {
                board[i-x][j+1].bai = x-1;
				break;
            }
			else
            {
				board[i-x][j+1].bai = x-1;
            }
        }
	}
	if(i > 0)
    {
        for(y = 1; j - y >= 0; y++)
        {
            if(board[i-1][j-y].val == LAT)
            {
                board[i-1][j-y].dir = y-1;
                break;
            }
            else 
            {
                board[i-1][j-y].dir = y-1;
            }
        }
    }
	for(x = i; x < i + navio; x++)
    {
		if(navio > 1)
        {
			if(x == i)
				board[x][j] = {PON_CIM, 0, 0};
			else if(x == i+navio-1)
				board[x][j] = {PON_BAI, 0, 0};
			else
				board[x][j] = {MEI, 0, 0};
        }
		if(j > 0)
        { 
			board[x][j-1].val = LAT;
			board[x][j-1].dir = 0;
		}
		if(j < size-1) 
			board[x][j+1].val = LAT;
        for(c = 2; j - c >= 0; c++)
        {
            if(board[x][j-c].val == LAT)
            {
                board[x][j-c].dir = c-2;
                break;
            }
            else
            {
               board[x][j-c].dir = c-2;
            }
        }
	}
	if(i+navio < size)
    {
        for(y = 1; j-y >=0; y++)
        {
            if(board[i+navio][j-y].val == LAT)
            {
                board[i+navio][j-y].dir = y-1;
                break;
            }
            else
            {
                board[i+navio][j-y].dir = y-1;
            }
        }
    }
    if(i > 0)
    {
        for(y = 1; j-y >=0; y++)
        {
            if(board[i-1][j-y].val == LAT)
            {
                board[i-1][j-y].dir = y-1;
                break;
            }
            else
            {
                board[i-1][j-y].dir = y-1;
            }
        }
    }
    if(i+navio < size)
        board[i+navio][j].val = LAT;
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
			if(board[i][j].val == SMEI)cout << "* "; 	
			else if(board[i][j].val == SCIR)cout << "O "; 	
			else if(board[i][j].val == SPON_ESQ)cout << "< "; 	
			else if(board[i][j].val == SPON_DIR)cout << "> "; 	
			else if(board[i][j].val == SPON_CIM)cout << "M "; 	
			else if(board[i][j].val == SPON_BAI)cout << "W "; 	
            else if(board[i][j].val == SWATER)cout << "X "; 
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

int shoot(unsigned int x, unsigned int y)
{
    if(x >= size || y >= size)
        return 0;
    if(board[x][y].val > LAT)
    {
        if(board[x][y].val > PON_BAI)
            return 0;
        board[x][y].val += 6;
        totalships--;
        board[x][size].val--;
        board[size][y].val--;
        return 0;
    }
    board[x][y].val = SWATER;
    return 1;
    
}

void free_board()
{
    int i;
    for(i = 0; i < size + 1; ++i)
		free(board[i]);
    free(board);
}
int main()
{
	fill_board();
	pos_ships();
    count_ships();
    int x, y, ten_res = 2;
    print_board(); 
    while(totalships > 0 && ten_res > 0)
    {
        cout << "Tentativas restantes: " << ten_res << endl;
        cout << "Faca uma jogada [linha coluna]: ";
        cin >> x >> y;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        }
        else if(shoot(x, y) == 1)
            ten_res--;
        print_board(); 
    }
    cout << "Jogo Terminado.\n";
    if(totalships == 0)
        cout << "Voce ganhou.\n";
    else
        cout << "Voce perdeu.\n";
    cout << "Pressione enter para sair...\n";
    system("read a");
    free_board();
    return EXIT_SUCCESS;
}
