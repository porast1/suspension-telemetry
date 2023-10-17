
import streamlit as st
import numpy as np
from scipy.signal import butter,filtfilt

def median_filter(data, window_size):
    filtered_data = []
    for i in range(len(data)):
        if i < (len(data) - 3) and  data[i+3] < 0:
            data[i+3] = 0
        start = max(0, i - window_size + 1)
        end = i + 1
        window = data[start:end]
        median_value = np.median(window)
        filtered_data.append(median_value)
    return filtered_data

def butter_lowpass_filter(data, cutoff, fs, order):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    # Get the filter coefficients 
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    y = filtfilt(b, a, data)
    return y

@st.cache_data
def load_data(file_path):
    T = 0.005        # Sample Period
    fs = 1/T       # sample rate, Hz
    cutoff = 2      # desired cutoff frequency of the filter, Hz ,  
    order = 2
    lista_liczb = []  # Zadeklaruj listę liczbową poza warunkiem
    if file_path is not None:
        st.success("Plik został pomyślnie wczytany!")
        # Utwórz plik tymczasowy i zapisz zawartość UploadedFile do niego
        with st.empty():
            with open(file_path.name, "wb") as f:
                f.write(file_path.read())
        with open(file_path.name, 'r') as file:
            content = file.read()
            lista_liczb = [float(x) for x in content.split()]
        data_cleaned = [x for x in lista_liczb if not (np.isnan(x) or np.isinf(x))]
        data_cleaned = median_filter(data_cleaned,4)
        data_cleaned = butter_lowpass_filter(data_cleaned, cutoff, fs, order)
    return data_cleaned

