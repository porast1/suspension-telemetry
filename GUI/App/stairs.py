# Inicjalizacja pustej tablicy
tablica = []

# Wartość początkowa
wartość = 0

# Dodawanie wartości do tablicy
for i in range(2000):
    tablica.append(wartość)
    if i % 100 == 99 and wartość < 250:
        wartość += 10

# Wyświetlenie tablicy


sformatowane_dane = ["{}".format(tablica[i]) for i in range(len(tablica))]
dane_jako_spacja = " ".join(sformatowane_dane)  # Łączenie danych w jedną linię

with open("schodki.txt", "w") as plik:
    plik.write(dane_jako_spacja)