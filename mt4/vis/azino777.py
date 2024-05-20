import matplotlib.pyplot as plt
import numpy as np

# Заданные точки отрезков
# A1 = (1, 1)
# A2 = (4, 4)
# B1 = (1, 8)
# B2 = (2, 4)

A1 = (80, 2)
A2 = (70, 10)
B1 = (70, 6)
B2 = (30, 48)

# A1 = (80, 2)
# A2 = (40, 35)
# B1 = (70, 5)
# B2 = (30, 48)
def plot_line_through_segment(A, B, color, linestyle, label):
    # Коэффициенты уравнения прямой
    x1, y1 = A
    x2, y2 = B
    if x2 - x1 != 0:
        slope = (y2 - y1) / (x2 - x1)
        intercept = y1 - slope * x1
        # Параметры для построения линии
        x_vals = np.array(plt.gca().get_xlim())
        y_vals = intercept + slope * x_vals
        plt.plot(x_vals, y_vals, color=color, linestyle=linestyle, label=label)
    else:
        # Вертикальная линия
        y_vals = np.array(plt.gca().get_ylim())
        x_vals = np.full_like(y_vals, x1)
        plt.plot(x_vals, y_vals, color=color, linestyle=linestyle, label=label)

# Настройка графика
plt.figure(figsize=(8, 6))
plt.axhline(y=0, color='k', linewidth=0.5)
plt.axvline(x=0, color='k', linewidth=0.5)
plt.grid(True, which='both')

# Построение отрезков
plt.plot([A1[0], A2[0]], [A1[1], A2[1]], 'ro-', label='Отрезок A')
plt.plot([B1[0], B2[0]], [B1[1], B2[1]], 'bo-', label='Отрезок B')

# Построение прямых через отрезки
plot_line_through_segment(A1, A2, 'r', '--', 'Прямая через A')
plot_line_through_segment(B1, B2, 'b', '--', 'Прямая через B')

# Проверка пересечения отрезков и нахождение точки пересечения
def find_intersection(A1, A2, B1, B2):
    x1, y1 = A1
    x2, y2 = A2
    x3, y3 = B1
    x4, y4 = B2

    D = (x2 - x1) * (y4 - y3) - (y2 - y1) * (x4 - x3)

    if D == 0:
        return None  # Прямые параллельны или совпадают

    t = ((x3 - x1) * (y4 - y3) - (y3 - y1) * (x4 - x3)) / D
    u = ((x3 - x1) * (y2 - y1) - (y3 - y1) * (x2 - x1)) / D

    if 0 <= t <= 1 and 0 <= u <= 1:
        intersection_x = x1 + t * (x2 - x1)
        intersection_y = y1 + t * (y2 - y1)
        return (intersection_x, intersection_y)

    return None  # Отрезки не пересекаются

intersection_point = find_intersection(A1, A2, B1, B2)
if intersection_point:
    plt.plot(intersection_point[0], intersection_point[1], 'go', label='Точка пересечения')

# Настройки графика
plt.xlim(min(A1[0], A2[0], B1[0], B2[0]) - 10, max(A1[0], A2[0], B1[0], B2[0]) + 10)
plt.ylim(min(A1[1], A2[1], B1[1], B2[1]) - 10, max(A1[1], A2[1], B1[1], B2[1]) + 10)
plt.legend()
plt.title('Отрезки и прямые через них')
plt.xlabel('x')
plt.ylabel('y')

# Показать график
plt.show()
