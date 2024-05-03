import plotly.graph_objects as go
import pandas as pd

# Чтение данных из файла csv
data = pd.read_csv('../tests/mt1_tests2.txt')
print(data.head())
# Создание графика
fig = go.Figure()

# # Добавление плоскости для времени выполнения последовательного алгоритма
# fig.add_trace(go.Surface(x=data['N'], y=data['M'], z=data['seq_time'], colorscale='Blues', name='Sequential Time'))
#
# # Добавление плоскости для времени выполнения параллельного алгоритма (race_time)
# fig.add_trace(go.Surface(x=data['N'], y=data['M'], z=data['race_time'], colorscale='Reds', name='Race Time'))

# Добавление плоскости для времени выполнения параллельного алгоритма (mt_time)
fig.add_trace(go.Surface(x=data['N'], y=data['M'], z=data['mt_time'], colorscale='Greens', name='MT Time'))

# Настройка макета графика
fig.update_layout(
    title='Time vs N vs M',
    scene=dict(
        xaxis=dict(title='N', range=[data['N'].min(), data['N'].max()]),
        yaxis=dict(title='M', range=[data['M'].min(), data['M'].max()]),
        zaxis=dict(title='Time')
    )
)

# Отображение графика
fig.show()
