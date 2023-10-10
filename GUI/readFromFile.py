import pandas as pd 
import numpy as np
from scipy.signal import butter,filtfilt

def butter_lowpass_filter(data, cutoff, fs, order):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    # Get the filter coefficients 
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    y = filtfilt(b, a, data)
    return y

with open("App\\FRONT0.TXT", 'r') as file:
    # Odczytaj zawartość pliku i podziel ją na liczby oddzielone spacjami
    content = file.read()
    lista_liczb = [float(x) for x in content.split()]


T = 5.0         # Sample Period
fs = 30.0       # sample rate, Hz
cutoff = 2      # desired cutoff frequency of the filter, Hz ,  
order = 2
dane = butter_lowpass_filter(lista_liczb, cutoff, fs, order)
print(dane)