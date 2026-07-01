import numpy as np                  #para arreglos numericos y calculos matematicos 
import matplotlib.pyplot as plt     #para generar graficos
import time                         #para medir tiempos de ejecucion
import os                           #para verificar si existe un archivo de entrada 

# Nombre del archivo de texto a leer
INPUT_FILE = "binary_stats.txt" 

def read_binary_stats_from_file(filename, max_n):
    """
    Lee estadísticas de combinaciones binarias desde un archivo txt.
    Formato esperado: una línea por n, con n valores separados por comas (suma de 1s).
    Devuelve una lista de arreglos stats y totales.
    """
    # Dado que el archivo es demasiado grande, ignoramos la lectura y siempre generamos.
    print("El archivo es demasiado grande para leer en memoria. Usando cálculo analítico en su lugar.")
    return None

def generate_binary_stats(n, sample_size=None):
    """
    Calcula estadísticas exactas de combinaciones binarias sin padding (unpadded) para números de 1 a 2^n - 1.
    Devuelve arreglos de stats (suma de 1s por posición), counts (número de cadenas con esa posición) y total.
    Posiciones alineadas a la izquierda (MSB en pos=0).
    """
    if n == 0:
        return np.array([0]), np.array([1]), 1  # Caso especial para n=0
    
    stats = np.zeros(n, dtype=np.int64)
    counts = np.zeros(n, dtype=np.int64)
    total = (1 << n) - 1  # 2**n - 1
    
    for pos in range(n):
        if pos == 0:
            counts[pos] = total
            stats[pos] = total
        else:
            counts[pos] = (1 << pos) * ((1 << (n - pos)) - 1)
            stats[pos] = (1 << (pos - 1)) * ((1 << (n - pos)) - 1)
    
    return stats, counts, total

def plot_binary_combinations(n_range, sample_size=250000):
    """
    Genera cuatro gráficas:
    1. Mapa de calor de la densidad de 1s.
    2. Mapa de calor del logaritmo natural de la densidad.
    3. Media de las densidades por n.
    4. Varianza de las densidades por n.
    """
    max_n = max(n_range)
    heatmap_data = np.zeros((len(n_range), max_n))  # Densidades
    log_heatmap_data = np.zeros((len(n_range), max_n))  # Logaritmo de densidades
    means = np.zeros(len(n_range))  # Media por n
    variances = np.zeros(len(n_range))  # Varianza por n
    
    for i, n in enumerate(n_range):
        start_time = time.perf_counter()  # Cambiado a perf_counter para mayor precisión
        stats, counts, total = generate_binary_stats(n, sample_size)
        
        # Calcular densidad por posición: stats / counts (ya que counts varía por posición en unpadded)
        density = np.zeros(n)
        for j in range(n):
            density[j] = stats[j] / counts[j] if counts[j] > 0 else 0
        
        # Ajustar tamaño para que coincida con max_n
        padded_density = np.zeros(max_n)
        if n > 0:
            padded_density[:n] = density[:n]
        else:
            padded_density[0] = 0
        heatmap_data[i, :max_n] = padded_density
        log_density = np.log(padded_density + 1e-10)  # Evitar ln(0)
        log_heatmap_data[i, :max_n] = log_density
        means[i] = np.mean(density) if n > 0 else 0
        variances[i] = np.var(density) if n > 0 else 0
        print(f"n={n}, Tiempo: {time.perf_counter() - start_time:.4f} segundos")  # Cambiado a perf_counter y formato .4f
    
    # Gráfica 1: Mapa de calor de densidad
    plt.figure(figsize=(12, 8))
    plt.imshow(heatmap_data, cmap='viridis', aspect='auto')
    plt.colorbar(label='Densidad de 1s')
    plt.xlabel('Posición en la secuencia (0=MSB izquierda)')
    plt.ylabel('Valor de n')
    plt.title('Densidad de 1s en combinaciones binarias sin padding (n=0 a 33)')
    plt.xticks(np.arange(max_n), np.arange(max_n))
    plt.yticks(np.arange(len(n_range)), n_range)
    plt.show()
    
    # Gráfica 2: Mapa de calor de logaritmo natural
    plt.figure(figsize=(12, 8))
    plt.imshow(log_heatmap_data, cmap='plasma', aspect='auto')
    plt.colorbar(label='Logaritmo natural de la densidad')
    plt.xlabel('Posición en la secuencia (0=MSB izquierda)')
    plt.ylabel('Valor de n')
    plt.title('Logaritmo natural de la densidad de 1s (n=0 a 33)')
    plt.xticks(np.arange(max_n), np.arange(max_n))
    plt.yticks(np.arange(len(n_range)), n_range)
    plt.show()
    
    # Gráfica 3: Media de densidades
    plt.figure(figsize=(10, 6))
    plt.plot(n_range, means, marker='o', linestyle='-', color='b')
    plt.xlabel('Valor de n')
    plt.ylabel('Media de densidades')
    plt.title('Media de densidades de 1s por n (0 a 33)')
    plt.grid(True)
    plt.xticks(n_range)
    plt.show()
    
    # Gráfica 4: Varianza de densidades
    plt.figure(figsize=(10, 6))
    plt.plot(n_range, variances, marker='o', linestyle='-', color='r')
    plt.xlabel('Valor de n')
    plt.ylabel('Varianza de densidades')
    plt.title('Varianza de densidades de 1s por n (0 a 33)')
    plt.grid(True)
    plt.xticks(n_range)
    plt.show()

def main():
    n_range = range(34)  # 0 a 33
    sample_size = 250000  # No se usa, ya que calculamos analíticamente
    
    print("Generando gráficas de combinaciones binarias usando cálculo analítico...")
    plot_binary_combinations(n_range, sample_size)
    
    # Estadísticas para n=33
    n = 33
    start_time = time.perf_counter()  # Cambiado a perf_counter para mayor precisión
    stats, counts, total = generate_binary_stats(n)
    density = np.zeros(n)
    for j in range(n):
        density[j] = stats[j] / counts[j] if counts[j] > 0 else 0
    print(f"\nEstadísticas para n={n}:")
    print(f"Densidad de 1s por posición: {density}")
    print(f"Media de densidades: {np.mean(density)}")
    print(f"Varianza de densidades: {np.var(density)}")
    print(f"Total de combinaciones procesadas: {total}")
    print(f"Tiempo total: {time.perf_counter() - start_time:.4f} segundos")  # Cambiado a perf_counter y formato .4f

if __name__ == "__main__":
    main()