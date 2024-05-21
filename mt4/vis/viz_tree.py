import json
import networkx as nx
import matplotlib.pyplot as plt
from networkx.drawing.nx_agraph import graphviz_layout


# Функция для рисования сегментов
def plot_segments(ax, segments, color='black'):
    for seg in segments:
        x_values = [seg['x1'], seg['x2']]
        y_values = [seg['y1'], seg['y2']]
        ax.plot(x_values, y_values, color=color)


def build_graph(node, graph, parent=None, edge_label=""):
    if node is None:
        return

    node_id = str(id(node))

    n = node['segments'][0]
    segment_str = f"({n['x1']},{n['y1']})-({n['x2']},{n['y2']})"

    graph.add_node(node_id, label=segment_str)

    if parent is not None:
        graph.add_edge(parent, node_id, label=edge_label)

    if 'front' in node and node['front'] is not None:
        build_graph(node['front'], graph, node_id, "front")

    if 'back' in node and node['back'] is not None:
        build_graph(node['back'], graph, node_id, "back")


def hierarchy_pos(G, root=None, width=1., vert_gap=0.2, vert_loc=0, xcenter=0.5):
    pos = _hierarchy_pos(G, root, width, vert_gap, vert_loc, xcenter)
    return pos


def _hierarchy_pos(G, root, width=1., vert_gap=0.2, vert_loc=0, xcenter=0.5, pos=None, parent=None, parsed=[]):
    if pos is None:
        pos = {root: (xcenter, vert_loc)}
    else:
        pos[root] = (xcenter, vert_loc)

    children = list(G.neighbors(root))
    if not isinstance(G, nx.DiGraph) and parent is not None:
        children.remove(parent)

    if len(children) != 0:
        dx = width / len(children)
        nextx = xcenter - width / 2 - dx / 2
        for child in children:
            nextx += dx
            pos = _hierarchy_pos(G, child, width=dx, vert_gap=vert_gap, vert_loc=vert_loc - vert_gap, xcenter=nextx,
                                 pos=pos, parent=root, parsed=parsed)

    return pos


def draw_tree(tree):
    graph = nx.DiGraph()
    build_graph(tree, graph)

    pos = hierarchy_pos(graph, str(id(tree)))
    labels = nx.get_node_attributes(graph, 'label')

    plt.figure(figsize=(12, 8))
    nx.draw(graph, pos, labels=labels, with_labels=True, node_size=300, node_color='skyblue', font_size=5,
            font_weight='bold')

    edge_labels = nx.get_edge_attributes(graph, 'label')
    nx.draw_networkx_edge_labels(graph, pos, edge_labels=edge_labels, font_size=10)

    plt.gca().invert_yaxis()
    plt.title('BSP Tree Visualization')
    plt.show()


def main():
    # Чтение JSON файла
    with open('../trees/t_20_19-28-12.json', 'r') as file:
        tree = json.load(file)

    # Визуализация дерева
    draw_tree(tree)


if __name__ == "__main__":
    main()
