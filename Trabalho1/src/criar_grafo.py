import matplotlib.pyplot as plt
import networkx as nx

'''def criar_grafo_sem_peso():
    # Criar um grafo simples (não direcionado)
    G = nx.Graph()

    # Adicionar nós
    G.add_node("A")
    G.add_node("B")
    G.add_node("C")
    G.add_node("D")
    G.add_node("E")

    # Adicionar arestas (sem pesos)
    G.add_edge("A", "B")
    G.add_edge("A", "C")
    G.add_edge("B", "D")
    G.add_edge("C", "D")
    G.add_edge("D", "E")

    # Configurar layout para o grafo
    pos = nx.spring_layout(G)

    # Criar a figura com proporção mais larga
    plt.figure(figsize=(12, 6))  # largura 12, altura 6

    # Desenhar os nós e as arestas
    nx.draw(G, pos, with_labels=True, node_color='lightgreen', node_size=2000, font_size=15, font_weight='bold', edge_color='gray')

    # Salvar o grafo em formato PNG
    # plt.title("Exemplo de Grafo Sem Peso")
    plt.savefig("grafo_sem_peso_largo.png", format="PNG", bbox_inches='tight')

    # Mostrar o grafo
    plt.show()

# Chamar a função para criar e salvar o grafo
criar_grafo_sem_peso()

import matplotlib.pyplot as plt
import networkx as nx

def criar_arvore():
    # Criar um grafo (árvore)
    G = nx.Graph()

    # Adicionar nós (nós da árvore)
    G.add_node("A")  # Raiz
    G.add_node("B")  # Filho da raiz
    G.add_node("C")  # Filho da raiz
    G.add_node("D")  # Filho de B
    G.add_node("E")  # Filho de B
    G.add_node("F")  # Filho de C
    G.add_node("G")  # Filho de C

    # Adicionar arestas (conexões na árvore)
    G.add_edge("A", "B")
    G.add_edge("A", "C")
    G.add_edge("B", "D")
    G.add_edge("B", "E")
    G.add_edge("C", "F")
    G.add_edge("C", "G")

    # Configurar layout para o grafo
    pos = nx.spring_layout(G)

    # Criar a figura com proporção mais larga
    plt.figure(figsize=(12, 6))  # largura 12, altura 6

    # Desenhar os nós e as arestas
    nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=2000, font_size=15, font_weight='bold', edge_color='gray')

    # Salvar a árvore em formato PNG
    #plt.title("Exemplo de Árvore")
    plt.savefig("arvore_exemplo.png", format="PNG", bbox_inches='tight')

    # Mostrar a árvore
    plt.show()

# Chamar a função para criar e salvar a árvore
criar_arvore()
'''


import matplotlib.pyplot as plt
import networkx as nx

def adicionar_nos(grafo, pos, k, x, y, altura):
    if k == 0:
        grafo.add_node(f"{x},{y}")
        pos[(f"{x},{y}")] = (x, y)
    else:
        grafo.add_node(f"{x},{y}")
        pos[(f"{x},{y}")] = (x, y)

        # Adicionar filhos
        x_left = x - 1 / (2 ** (k + 1))
        x_right = x + 1 / (2 ** (k + 1))
        y_next = y - altura

        # Chamar recursivamente para os filhos
        adicionar_nos(grafo, pos, k - 1, x_left, y_next, altura)
        adicionar_nos(grafo, pos, k - 1, x_right, y_next, altura)

        # Adicionar arestas entre o nó pai e os filhos
        grafo.add_edge(f"{x},{y}", f"{x_left},{y_next}")
        grafo.add_edge(f"{x},{y}", f"{x_right},{y_next}")

def criar_arvore_binomial(k):
    # Criar um grafo para a árvore binomial
    G = nx.Graph()
    pos = {}

    # Adicionar nós recursivamente
    adicionar_nos(G, pos, k, 0, 0, 1)  # (posição x, posição y, altura)

    # Criar a figura com proporção mais larga
    plt.figure(figsize=(12, 6))  # largura 12, altura 6

    # Desenhar os nós e as arestas
    nx.draw(G, pos, with_labels=True, node_color='lightcoral', node_size=2000, font_size=10, font_weight='bold', edge_color='gray')

    # Salvar a árvore em formato PNG
    # plt.title(f"Árvore Binomial de Ordem {k}")
    plt.savefig(f"arvore_binomial_ordem_{k}.png", format="PNG", bbox_inches='tight')

    # Mostrar a árvore
    plt.show()

# Definir a ordem da árvore binomial (k)
ordem = 3  # Você pode alterar este valor para criar árvores de diferentes ordens
criar_arvore_binomial(ordem)
