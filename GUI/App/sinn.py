import numpy as np

# Liczba próbek
liczba_probek = 100000

# Zakres czasu
czas = np.linspace(0, 5000, liczba_probek)

for iteracja in range(2):
    # Zakres amplitudy
    amplitudy = np.zeros(liczba_probek)
    for i in range(liczba_probek):
        if i % 300 == 0:  # Co 300 próbek zmieniamy amplitudę
            nowa_amplituda = np.random.uniform(0, 170)
            amplitudy[i] = min(nowa_amplituda, 170)  # Ograniczenie do maksymalnej wartości 170
        else:
            amplitudy[i] = amplitudy[i - 1]

    # Zakres częstotliwości
    czestotliwosci = np.random.uniform(1, 5, liczba_probek)

    # Generowanie sygnału o zmiennej amplitudzie i częstotliwości
    sygnal = amplitudy * np.sin(2 * np.pi * czestotliwosci * czas) + amplitudy

    # Zapisanie danych do pliku tekstowego
    nazwa_pliku = f"dane_sinusoidy_{iteracja + 1}.txt"
    sformatowane_dane = ["{}".format(sygnal[i]) for i in range(len(sygnal))]
    dane_jako_spacja = " ".join(sformatowane_dane)  # Łączenie danych w jedną linię

    with open(nazwa_pliku, "w") as plik:
        plik.write(dane_jako_spacja)