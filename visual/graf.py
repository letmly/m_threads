import numpy as np
import pandas as pd
from scipy.optimize import least_squares
import plotly.graph_objects as go

# Чтение данных из файла csv
data = pd.read_csv('../tests/mt1_tests2.txt')

# Выборка значений N, M и времени работы алгоритма
N = data['N']
M = data['M']
time = data['mt_time'].values

# Функция, определяющая расстояние от точек данных до плоскости
def residuals(coefficients, N, M, time):
    a, b, c, d = coefficients
    return a * N + b * M + c * N * M + d - time

# Начальное приближение для коэффициентов плоскости
initial_guess = [0, 0, 0, 0]

# Вычисление коэффициентов плоскости методом наименьших квадратов
result = least_squares(residuals, initial_guess, args=(N, M, time))

# Расчет значений плоскости для отображения
a, b, c, d = result.x
N_mesh, M_mesh = np.meshgrid(np.linspace(N.min(), N.max(), 100), np.linspace(M.min(), M.max(), 100))
time_surface = a * N_mesh + b * M_mesh + c * N_mesh * M_mesh + d

# Создание графика
fig = go.Figure()

# Добавление поверхности плоскости
fig.add_trace(go.Surface(x=N_mesh, y=M_mesh, z=time_surface, colorscale='Blues', name='Time Surface'))

# Настройка макета графика
fig.update_layout(
    title='Time Surface vs N vs M',
    scene=dict(
        xaxis=dict(title='N', range=[N.min(), N.max()]),
        yaxis=dict(title='M', range=[M.min(), M.max()]),
        zaxis=dict(title='Time')
    )
)

# Отображение графика
fig.show()
