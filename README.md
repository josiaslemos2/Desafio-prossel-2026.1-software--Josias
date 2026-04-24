-Introdução: 
O sistema de controle desenvolvido  baseia-se em uma arquitetura de Comportamentos Condicionais, semelhante  uma máquina de estados finitos, 
onde as decisões são tomadas em tempo real com base no estado atual do ambiente (posição da bola, aliados e oponentes). 

-Tomador de Decisão: A estrutura de decisão utiliza uma abordagem de Seleção de Comportamento por Funções Fixas, 
complementada por uma Lógica de Transição de Estado Interna.

Estratégia de Papéis:

Goleiro: Foca na interceptação da trajetória da bola dentro da pequena área, 
utilizando uma zona de movimento restrita por clamping no eixo X e Y.

Ala: Atua como um robô de suporte defensivo. Seu estado alterna entre manter-se atrás da bola recuando e "ataca" quando a bola está muito próxima do gol.

Atacante: Possui o estado de maior agressividade. Sua lógica transita entre aproximação da bola e condução ao gol adversário, 
com supressão de forças repulsivas no momento do contato. Par evitar momentos de "equílibrio" (Fr = 0), existem 2 classes de atacante: 
		- Atacante Principal: Se está mais próximo da bola tem prioridade.
		- Atacante secundário: Segue a bola mas mantém distância.

-Planejador de Caminho: O planejador foi implementado através da técnica de Campos Potenciais.

Funcionamento Matemático:O movimento resultante do robô é a soma vetorial de duas forças principais:

Força de Atração (vec{F}_{atr}): Um vetor unitário que aponta do robô para o alvo definido pelo Tomador de Decisão.

Força de Repulsão (vec{F}_{rep}): Um campo de força inversamente proporcional à distância entre o robô e os obstáculos (outros robôs).

Algoritmo de Navegação:Para evitar o problema em que a Fr = 0, implementamos um Vetor de Escape Tangencial. 
Quando a magnitude da força resultante é próxima de zero, adicionamos um vetor perpendicular (pi/2radianos) para permitir que o robô "escorregue" pelo obstáculo.

Quebra de simetria
Pequeno desvio lateral para evitar decisões idênticas.

Zona morta
Evita tremor quando o robô já está no alvo.

Suavização (inércia)
Mistura movimento atual com o anterior para evitar mudanças bruscas

-Justificativa das Escolhas

Usou-se por uma lógica de MSF por ser uma estrutura simples e eficiente para tomada de decisão em tempo real, 

O uso do Campo Potencial permite que o robô recalcule sua trajetória a cada frame de simulação. Isso é ideal para desviar de obstáculos móveis (oponentes), pois o robô não planeja uma rota estática, mas sim "flui" através do campo de forças. 

-Coordenação e Matemática: 
A cooperação entre os 3 robôs é garantida através de:

Zonas de Influência Geográfica:Distribuição Espacial: Através da função std::clamp, limitamos o Ala e o Goleiro a quadrantes específicos. Isso impede que os três robôs corram para a bola ao mesmo tempo, 
garantindo a profundidade defensiva.

Normalização Vetorial: Toda a movimentação é normalizada (vec{V}|vec{V}). 
Isso garante que, independente da intensidade das forças de desvio, o robô mantenha uma velocidade constante e previsível, 
facilitando a sincronia da equipe.

Divisão entre os papeis de atacante principal e secundário.

























































