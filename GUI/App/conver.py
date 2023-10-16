import csv
# Odczytaj dane z pliku REAR0.txt
with open('FRONT0.txt', 'r') as rear_file:
    rear_data = rear_file.read().strip().split()  # Odczytaj i podziel dane

# Otwórz plik CSV do zapisu
with open('output.csv', 'w', newline='') as output_csv:
    csv_writer = csv.writer(output_csv, delimiter=';')  # Użyj średnika jako separatora

    # Zapisz dane w jednej kolumnie
    csv_writer.writerow(['Wheel_T'])
    for value in rear_data:
        csv_writer.writerow([value])