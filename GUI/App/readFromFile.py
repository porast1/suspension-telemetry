import numpy as np

# Przykładowa tablica
arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9])

# Znajdź indeksy elementów, które spełniają warunek (np. większe niż 5)
indices = np.where(arr > 5)

# Wydobądź elementy o znalezionych indeksach
selected_elements = arr[indices]

print(indices)  # Wyświetlenie indeksów
print(selected_elements)  # Wyświetlenie wybranych elementów
