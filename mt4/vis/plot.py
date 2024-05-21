import json
import matplotlib.pyplot as plt

# Загрузка сегментов из файла JSON
with open('../gens/now20_16-33-06.json') as f:
    segments = json.load(f)

# Рисование сегментов
for seg in segments:
    plt.plot([seg['x1'], seg['x2']], [seg['y1'], seg['y2']], marker='o')

plt.xlabel('X')
plt.ylabel('Y')
plt.title('Segments and BSP Tree')
plt.grid(True)
plt.show()
