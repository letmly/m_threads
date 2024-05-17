import matplotlib.pyplot as plt
import csv

N = []
seq_time = []
mt_time = []

with open('../tests/floid_2024-05-17_03-16-26.txt', 'r') as file:
    reader = csv.reader(file)
    next(reader)
    for row in reader:
        N.append(int(row[0]))
        seq_time.append(float(row[1]))
        mt_time.append(float(row[2]))

plt.figure(figsize=(10, 6))
plt.plot(N, seq_time, label='Seq Time', marker='o')
plt.plot(N, mt_time, label='Mt Time', marker='o')

plt.xlabel('N')
plt.ylabel('Time (s)')
plt.title('Сравнение по времени выполнения seq и mt алгоритма Флойда')
plt.legend()

plt.grid(True)
plt.show()
