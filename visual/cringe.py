import pandas as pd
import plotly.graph_objects as go

# Чтение данных из файла csv
data = pd.read_csv('../tests/2024-05-17_00-21-12.txt')

# Выборка значений N, M и времени работы алгоритма
N = data['N']
M = data['M']

# Создание графика
fig = go.Figure()


for cls, col in [['seq_time', 'red'], ['mt_time', 'green']]:
    time = data[cls].values
    # np.resize(time, (len(data['N'].unique()), len(data['M'].unique())))
    ns = len(data['N'].unique())
    ms = len(data['M'].unique())
    traces = []
    # Добавление точек данных
    for i in range(ns):
        traces.append(go.Scatter3d(x=N[ms*i:ms*(i+1)], y=M[ms*i:ms*(i+1)], z=time[ms*i:ms*(i+1)], mode='lines', line=dict(width=2, color=col)))

    for i in range(ms):
        traces.append(go.Scatter3d(x=N[i::ms], y=M[i::ms], z=time[i::ms], mode='lines', line=dict(width=2, color=col)))
    fig.add_traces(traces)

# Настройка макета графика
fig.update_layout(
    title='Green - mt, Red - seq',
    scene=dict(
        xaxis=dict(title='N', range=[N.min(), N.max()]),
        yaxis=dict(title='M', range=[M.min(), M.max()]),
        zaxis=dict(title='Time')
    )
)

# Отображение графика
fig.show()
