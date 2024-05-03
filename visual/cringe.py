import numpy as np
import pandas as pd
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
import plotly.graph_objects as go

# Чтение данных из файла csv
data = pd.read_csv('../tests/mt1_tests2.txt')

# Выборка значений N, M и времени работы алгоритма
N = data['N']
M = data['M']
time = data['mt_time'].values

# Создание объекта PolynomialFeatures для создания полиномиальных признаков
poly = PolynomialFeatures(degree=3)

# Преобразование данных для включения полиномиальных признаков
X = poly.fit_transform(np.column_stack((N, M)))

# Создание модели линейной регрессии
model = LinearRegression()

# Обучение модели на данных
model.fit(X, time)

# Получение коэффициентов модели
coefficients = model.coef_.T

# Вычисление значений времени для отображения кривой плоскости
N_mesh, M_mesh = np.meshgrid(np.linspace(N.min(), N.max(), 100), np.linspace(M.min(), M.max(), 100))
X_mesh = poly.fit_transform(np.column_stack((N_mesh.ravel(), M_mesh.ravel())))
time_surface = np.dot(X_mesh, coefficients).reshape(N_mesh.shape)

# Создание графика
fig = go.Figure()

# Добавление поверхности кривой плоскости
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
