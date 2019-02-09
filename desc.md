
# Battleships
![Captura de tela](screenshot.png)  
Battleships é uma jogo muito conhecido tabuleiro, no qual você tem 
quer acertar o navio dos inimigos por meio de sua coordenada.  
A versão que eu fiz é feita em C++ 11 utilizando a biblioteca SFML.

# Desafios
Esse foi meu segundo projeto utilizando SFML. Se você estiver olhando 
o código fonte vai ver que eu não usei orientação a objetos no algoritmo, 
isso aconteceu porque na primeira versão (ainda em TUI) eu não estava 
modelando com orientação a objetos ainda, isso foi feito na minha transição 
de C para C++. Esses fatores dificultaram um pouco em pensar na maneira 
como eu faria a comunicação com a interface gráfica.  
Durante a implementação eu não usei nenhuma técnica conhecida para gerar 
os mapas, então eu tive que pensar muito como fazer isso. Sendo assim, para 
alcançar o objetivo, eu criei um vetor com os tamanhos de cada navio e fui 
gerando posições aleatórias e posicionando cada navio. Caso não seja possível 
posicionar todos, alguns serão ignorados.  
O código ficou repetitivo e extenso, porém não deve ter ficado difícil de 
entender com ajuda dos comentários.

# Como usar
Depois de compilado ou baixado, você pode executar com o comando `./battleships` ou dando dois cliques no executável. Nessa versão eu não coloquei 
parâmetros de execução, assim o jogo inicia com uma matriz 10x10, mas 
você pode modificar isso caso esteja com o código fonte.  
