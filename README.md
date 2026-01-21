# Pathfinding

Um visualizador interativo de algoritmos de busca de caminho desenvolvido em C++ com Raylib.

## 📋 Sobre o Projeto

Este projeto implementa e visualiza dois algoritmos clássicos de pathfinding:
- **A\***
- **Guloso (Greedy)**

## ✨ Funcionalidades

- 🎨 Interface gráfica interativa e responsiva
- 🖱️ Desenho de obstáculos com o mouse
- ⚡ Controle de velocidade de execução
- 🔄 Escolha entre algoritmos A* e Greedy
- 🧹 Limpeza de paredes e reinicialização do grid
- 📊 Estatísticas (nós visitados, tamanho do caminho)

## 🎮 Como Usar

### Controles

| Ação | Comando |
|------|---------|
| Desenhar parede | **Botão esquerdo do mouse** |
| Apagar parede | **Botão direito do mouse** |
| Mover ponto inicial | **Shift + Clique esquerdo** |
| Mover ponto final | **Ctrl + Clique esquerdo** |
| Reiniciar grid | **Tecla R** |

### Interface

- **Iniciar**: Executa o algoritmo selecionado
- **Limpar Grid**: Remove todas as paredes desenhadas
- **Trocar Algoritmo**: Alterna entre A* e Greedy
- **Slider de Velocidade**: Ajusta a velocidade de visualização (1-50 passos por frame)

### Legenda de Cores

- 🟢 **Verde**: Ponto inicial
- 🔴 **Vermelho**: Ponto final
- 🟦 **Azul claro**: Nós visitados
- 🟨 **Amarelo**: Fronteira (nós a serem explorados)
- 🟪 **Roxo**: Caminho encontrado
- ⬛ **Cinza escuro**: Paredes/obstáculos

## 🖥️ Requisitos do Sistema

### Para Executar
- Windows 7 ou superior
- OpenGL 3.3 ou superior
- GPU com suporte a DirectX 11 ou superior

### Para Compilar
- **Compilador**: MinGW/GCC com suporte a C++17
- **Biblioteca**: [Raylib](https://www.raylib.com/)
- **Build System**: Make (opcional)

## 🔨 Como Compilar

### Opção 1: Usando Make

```bash
make
```

### Opção 2: Compilação Manual

```bash
g++ -std=c++17 -O2 -I"<caminho_raylib>/raylib/src" \
    src/grid.cpp src/pathfinder.cpp src/ui.cpp src/main.cpp \
    -o pathfinding.exe \
    -L"<caminho_raylib>/raylib/src" \
    -lraylib -lopengl32 -lgdi32 -lwinmm
```
