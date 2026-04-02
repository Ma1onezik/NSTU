import matplotlib.pyplot as plt
import numpy as np

threads = [2, 4, 6, 8]

time_1 = [1516.68, 1221.52, 1173.22, 950.256]

time_2 = [3927.82, 2886.45, 1983.74, 1857.38]

time_3 = [6224.01, 5490.16, 4553.66, 3875.61] 

plt.figure(figsize=(12, 7))

plt.plot(threads, time_1, 'o-', linewidth=2.5, markersize=10, 
         label='25 степень', color='darkblue', markerfacecolor='lightblue')

plt.plot(threads, time_2, 's-', linewidth=2.5, markersize=10, 
         label='26 степень', color='darkred', markerfacecolor='lightcoral')

valid_threads = [t for t, v in zip(threads, time_3) if v is not None]
valid_times = [v for v in time_3 if v is not None]
plt.plot(valid_threads, valid_times, '^-', linewidth=2.5, markersize=10, 
         label='27 степень', color='darkgreen', markerfacecolor='lightgreen')

for i, (x, y) in enumerate(zip(threads, time_1)):
    plt.annotate(f'{y:.2f}', (x, y), textcoords="offset points", 
                 xytext=(0, 15), ha='center', fontsize=10, fontweight='bold',
                 bbox=dict(boxstyle='round,pad=0.3', facecolor='lightblue', alpha=0.7))

for i, (x, y) in enumerate(zip(threads, time_2)):
    plt.annotate(f'{y:.2f}', (x, y), textcoords="offset points", 
                 xytext=(0, -20), ha='center', fontsize=10, fontweight='bold',
                 bbox=dict(boxstyle='round,pad=0.3', facecolor='lightcoral', alpha=0.7))

for i, (x, y) in enumerate(zip(valid_threads, valid_times)):
    plt.annotate(f'{y:.2f}', (x, y), textcoords="offset points", 
                 xytext=(0, 15), ha='center', fontsize=10, fontweight='bold',
                 bbox=dict(boxstyle='round,pad=0.3', facecolor='lightgreen', alpha=0.7))

plt.xlabel('Количество потоков', fontsize=14, fontweight='bold')
plt.ylabel('Время выполнения (мс)', fontsize=14, fontweight='bold')
plt.title('Сравнение производительности параллельной сортировки\n(различные конфигурации)', 
          fontsize=16, fontweight='bold', pad=20)

plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=12, loc='upper right', framealpha=0.9)

plt.xlim(1.5, 8.5)
plt.ylim(0, 7500)

plt.xticks(threads, [f'{t} потоков' for t in threads], fontsize=12)
plt.yticks(fontsize=12)

plt.figtext(0.5, 0.01, 
            'Примечание: Для набора 26 (вторые значения) данные доступны только для 2 и 4 потоков', 
            ha='center', fontsize=10, style='italic', 
            bbox=dict(boxstyle='round', facecolor='lightyellow', alpha=0.8))

plt.tight_layout()
plt.savefig('sorting_performance_complete.png', dpi=300, bbox_inches='tight', 
            facecolor='white', edgecolor='none')

plt.show()

print("=" * 60)
print("СВОДНАЯ ТАБЛИЦА РЕЗУЛЬТАТОВ")
print("=" * 60)
print(f"{'Потоки':<10} {'Набор 25':<15} {'Набор 26 (1)':<15} {'Набор 26 (2)':<15}")
print("-" * 60)
for i in range(len(threads)):
    t1 = f"{time_1[i]:.2f}" if i < len(time_1) else "N/A"
    t2 = f"{time_2[i]:.2f}" if i < len(time_2) else "N/A"
    t3 = f"{time_3[i]:.2f}" if i < len(time_3) and time_3[i] is not None else "N/A"
    print(f"{threads[i]:<10} {t1:<15} {t2:<15} {t3:<15}")

print("=" * 60)

print("\nУСКОРЕНИЕ (относительно 2 потоков):")
print("-" * 40)
print(f"{'Потоки':<10} {'Набор 25':<15} {'Набор 26 (1)':<15}")
print("-" * 40)
for i in range(len(threads)):
    if i == 0:
        speedup1 = 1.0
        speedup2 = 1.0
    else:
        speedup1 = time_1[0] / time_1[i]
        speedup2 = time_2[0] / time_2[i]
    print(f"{threads[i]:<10} {speedup1:<15.2f} {speedup2:<15.2f}")