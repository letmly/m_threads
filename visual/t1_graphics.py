import pandas as pd
import matplotlib.pyplot as plt

file_path = '../tests/2024-05-17_00-21-12.txt'

data = pd.read_csv(file_path)

M_values = [10, 1000, 1000000, 100000000]

fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(14, 10))

axes = axes.flatten()

for ax, M in zip(axes, M_values):
    subset = data[data['M'] == M]
    if subset.empty:
        ax.set_title(f'No data for M = {M}')
        continue
    ax.plot(subset['N'], subset['mt_time'], marker='o', label='mt_time')
    ax.plot(subset['N'], subset['seq_time'], marker='x', label='seq_time')
    ax.set_xlabel('N')
    ax.set_ylabel('Time')
    ax.set_title(f'Graph for M = {M}')
    ax.legend()

plt.tight_layout()
plt.show()
