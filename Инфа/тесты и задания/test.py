import matplotlib.pyplot as plt

# Данные
N = [10, 100, 1000, 10000, 100000, 1000000]
search_time = [2, 4, 46, 672, 6515, 89303]  # мс

# Построение графика
plt.figure(figsize=(10, 6))

# Линия
plt.plot(N, search_time, label="Время поиска", linewidth=2)

# Точки
plt.scatter(N, search_time)

# Логарифмическая шкала по X
plt.xscale("log")

# Подписи точек
for x, y in zip(N, search_time):
    plt.annotate(f"({x}, {y})",
                 (x, y),
                 textcoords="offset points",
                 xytext=(5, 5))

# Подписи осей и заголовок
plt.xlabel("Длина списка (N)")
plt.ylabel("Время поиска, мс")
plt.title("Зависимость времени поиска от длины списка")

# Сетка и легенда
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()

# Отображение
plt.show()
