
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
def load_data(data):
    if data is not None and len(data) > 0:
        st.success("Dane zostały pomyślnie wczytane!")
        return data
    else:
        st.error("Brak danych do wczytania.")
        return None



