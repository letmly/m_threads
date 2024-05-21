import json
import matplotlib.pyplot as plt
import matplotlib.lines as mlines


def plot_segments(ax, segments, color, label_prefix, linestyle='-'):
    for i, seg in enumerate(segments):
        x_values = [seg['x1'], seg['x2']]
        y_values = [seg['y1'], seg['y2']]
        ax.plot(x_values, y_values, color=color, linestyle=linestyle, label=f"{label_prefix} {i + 1}" if i == 0 else "")


def collect_segments(tree, front_segments, back_segments, intersect_segments, depth=0):
    if tree is None:
        return

    intersect_segments.extend(tree['segments'])

    if 'front' in tree and tree['front'] is not None:
        collect_segments(tree['front'], front_segments, back_segments, intersect_segments, depth + 1)

    if 'back' in tree and tree['back'] is not None:
        collect_segments(tree['back'], front_segments, back_segments, intersect_segments, depth + 1)


def visualize_bsp_tree(tree):
    fig, ax = plt.subplots(figsize=(12, 8))
    ax.set_aspect('equal', 'box')

    front_segments = []
    back_segments = []
    intersect_segments = []

    collect_segments(tree, front_segments, back_segments, intersect_segments)

    plot_segments(ax, intersect_segments, 'green', 'Intersect')
    plot_segments(ax, front_segments, 'blue', 'Front')
    plot_segments(ax, back_segments, 'red', 'Back')

    # Create legend entries
    intersect_line = mlines.Line2D([], [], color='green', label='Intersect Segments')
    front_line = mlines.Line2D([], [], color='blue', label='Front Segments')
    back_line = mlines.Line2D([], [], color='red', label='Back Segments')

    ax.legend(handles=[intersect_line, front_line, back_line])

    plt.title('BSP Tree Segments Visualization')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.grid(True)
    plt.show()


# Чтение JSON файла
with open('../trees/t_20_16-33-06.json', 'r') as file:
    tree = json.load(file)

# Визуализация дерева
visualize_bsp_tree(tree)
