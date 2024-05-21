import json
import matplotlib.pyplot as plt
import matplotlib.lines as mlines
import random

from label_lines import *


def plot_segments(ax, segments, color_map):
    legend_labels = {}
    for i, seg in enumerate(segments):
        x_values = [seg['x1'], seg['x2']]
        y_values = [seg['y1'], seg['y2']]
        color = color_map[tuple(sorted((seg['x1'], seg['y1'], seg['x2'], seg['y2'])))]

        label = f"({seg['x1']},{seg['y1']})-({seg['x2']},{seg['y2']})"
        ax.plot(x_values, y_values, color=color, label=label)
        kwargs = dict()
        kwargs['color'] = color
        kwargs['va'] = 'center'
        kwargs['ha'] = 'center'
        kwargs['backgroundcolor'] = ax.get_facecolor()
        kwargs['clip_on'] = True
        kwargs['zorder'] = 2.5
        ax.text((seg['x1'] + seg['x2'])/2, (seg['y1'] + seg['y2'])/2, i + 1,rotation=0, **kwargs)

    # labelLines(plt.gca().get_lines(), labels=range(len(segments) + 1)[1:], align=False, fontsize=12)


def collect_segments(tree):
    segments = []
    if tree is None:
        return segments

    segments.extend(tree['segments'])

    if 'front' in tree and tree['front'] is not None:
        segments.extend(collect_segments(tree['front']))

    if 'back' in tree and tree['back'] is not None:
        segments.extend(collect_segments(tree['back']))

    return segments


def visualize_segments(tree):
    fig, ax = plt.subplots(figsize=(15, 15))
    ax.set_aspect('equal', 'box')

    segments = collect_segments(tree)

    # Generate a unique color for each segment
    color_map = {}
    for seg in segments:
        sorted_coords = tuple(sorted((seg['x1'], seg['y1'], seg['x2'], seg['y2'])))
        if sorted_coords not in color_map:
            color_map[sorted_coords] = (random.random(), random.random(), random.random())

    plot_segments(ax, segments, color_map)

    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
    plt.title('Сегменты дерева')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.grid(True)
    plt.show()


def main():
    with open('../trees/t_20_19-24-27.json', 'r') as file:
        tree = json.load(file)

    visualize_segments(tree)


if __name__ == "__main__":
    main()
