import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных из файла csv
data = pd.read_csv(r"D:\.dev\vstu_MultiThreads\tests\mt1_tests.txt")

# Построение графика
plt.plot(data['N'], data['seq_t'], label='Sequential Time')
plt.plot(data['N'], data['race_time'], label='Race Time')
plt.plot(data['N'], data['mt_time'], label='MT Time')

# Добавление меток и легенды
plt.xlabel('N')
plt.ylabel('Time')
plt.title('Time vs N')
plt.legend()

# Отображение графика
plt.show()
