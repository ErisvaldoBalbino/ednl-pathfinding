# Pathfinding

Visualizador interativo dos algoritmos A* e Guloso

Atividade de **Estruturas de Dados Não Lineares** (EDNL) — IFRN.

## Stack

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white) ![raylib](https://img.shields.io/badge/raylib-000000?style=for-the-badge&logo=raylib&logoColor=white)

## Como rodar

Precisa do `g++` com suporte a C++17 e da [raylib](https://www.raylib.com/) instalada.

```bash
make
./pathfinding.exe
```

## Funcionalidades

- Interface gráfica interativa e responsiva
- Desenho de obstáculos com o mouse
- Controle de velocidade de execução
- Escolha entre algoritmos A* e Greedy
- Limpeza de paredes e reinicialização do grid
- Estatísticas (nós visitados, tamanho do caminho)

## Controles

| Ação | Comando |
|------|---------|
| Desenhar parede | **Botão esquerdo do mouse** |
| Apagar parede | **Botão direito do mouse** |
| Mover ponto inicial | **Shift + Clique esquerdo** |
| Mover ponto final | **Ctrl + Clique esquerdo** |
| Reiniciar grid | **Tecla R** |

## Interface

- **Iniciar**: Executa o algoritmo selecionado
- **Limpar Grid**: Remove todas as paredes desenhadas
- **Trocar Algoritmo**: Alterna entre A* e Greedy
- **Slider de Velocidade**: Ajusta a velocidade de visualização (1-50 passos por frame)

## Legenda de cores

- **Verde**: Ponto inicial
- **Vermelho**: Ponto final
- **Azul claro**: Nós visitados
- **Amarelo**: Fronteira (nós a serem explorados)
- **Roxo**: Caminho encontrado
- **Cinza escuro**: Paredes/obstáculos
