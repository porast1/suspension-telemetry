import streamlit as st
import pandas as pd
import numpy as np
from streamlit_option_menu import option_menu
from bokeh.plotting import figure, show, curdoc
from bokeh.io import output_notebook
from bokeh.models import ColumnDataSource, Button
from bokeh.layouts import column
from scipy.signal import butter,filtfilt

st.set_page_config(layout="wide")
timePerSample = 5
timePerSampleAcc = 5


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
        lista_liczb = median_filter(lista_liczb,4)
        lista_liczb = butter_lowpass_filter(lista_liczb, cutoff, fs, order)
    return lista_liczb



# Inicjalizacja kontekstu sesji
if 'file_uploaded' not in st.session_state:
    st.session_state.file_uploaded = False
if 'travelPressureTime' not in st.session_state:
    st.session_state.travelPressureTime = None
if 'accelerometerTime' not in st.session_state:
    st.session_state.accelerometerTime = None
if 'dfFrontTravel' not in st.session_state:
    st.session_state.dfFrontTravel = None
if 'dfRearTravel' not in st.session_state:
    st.session_state.dfRearTravel = None
if 'dfFrontPressure' not in st.session_state:
    st.session_state.dfFrontPressure = None
if 'dfRearPressure' not in st.session_state:
    st.session_state.dfRearPressure = None
if 'dfAccelerometer' not in st.session_state:
    st.session_state.dfAccelerometer = None


with st.sidebar:
    selected = st.selectbox(
        "Main Menu",
        ["Load & Process data", "Rebound/Comp", "Pressure", "Statistics"]
    )
    frontTravel = None
    rearTravel = None
    frontPressure = None
    rearPressure = None
    accelerometer = None
        # Upload Front Travel File
    frontTravel = st.file_uploader(".txt file for Front travel", type=["txt"])
        # Upload Front Travel File
    rearTravel = st.file_uploader(".txt file for Rear travel", type=["txt"])
        # Upload Front Travel File
    frontPressure = st.file_uploader(".txt file for Front Pressure", type=["txt"])
        # Upload Front Travel File
    rearPressure = st.file_uploader(".txt file for Rear Pressure", type=["txt"])
        # Upload Front Travel File
    accelerometer = st.file_uploader(".txt file for accelerometer", type=["txt"])        
if selected == "Load & Process data" and ((frontTravel is not None and
    rearTravel is not None) or
    (frontPressure is not None and
    rearPressure is not None)):
    st.title(f"You have selected {selected}")

    st.session_state.dfFrontTravel = load_data(frontTravel)  # Wczytaj dane z pliku tymczasowego
    st.session_state.dfRearTravel = load_data(rearTravel)  # Wczytaj dane z pliku tymczasowego
    st.session_state.dfFrontPressure = load_data(frontPressure)  # Wczytaj dane z pliku tymczasowego
    st.session_state.dfRearPressure = load_data(rearPressure)  # Wczytaj dane z pliku tymczasowego
    st.session_state.dfAccelerometer = load_data(accelerometer)  # Wczytaj dane z pliku tymczasowego
    if st.session_state.dfRearPressure and st.session_state.dfFrontPressure is not none:
        lenTime = min(len(st.session_state.dfFrontTravel),len(st.session_state.dfRearTravel),
                len(st.session_state.dfFrontPressure), len(st.session_state.dfRearPressure))
    else:
        lenTime = min(len(st.session_state.dfFrontTravel),len(st.session_state.dfRearTravel))
    lenTimeAcc = len(st.session_state.dfAccelerometer)

    st.session_state.travelPressureTime = np.arange(0, timePerSample * lenTime, timePerSample)
    st.session_state.accelerometerTime = np.arange(0, timePerSample * lenTimeAcc, timePerSampleAcc)
    st.session_state.dfFrontTravel = st.session_state.dfFrontTravel[:lenTime]
    st.session_state.dfRearTravel = st.session_state.dfRearTravel[:lenTime]
    st.session_state.dfFrontPressure =  st.session_state.dfFrontPressure[:lenTime]
    st.session_state.dfRearPressure =  st.session_state.dfRearPressure[:lenTime]
    st.session_state.file_uploaded = True  # Ustaw stan sesji na True
    st.success("Dane Przetworzone")
else:
    if 'dfFrontTravel' not in st.session_state:
        st.session_state.dfFrontTravel = None

# Aktualizacja warunku dla widoku "Rebound/Comp"
if selected == "Rebound/Comp" and st.session_state.file_uploaded:
    # Title
    st.title(f"You have selected {selected}")
    selected_series = st.multiselect("Wybierz serie danych:", ["Front", "Rear"])
    
    # Utwórz wykres Bokeh
    p = figure(title="Wykres Bokeh w Streamlit", x_axis_label="Oś X", y_axis_label="Oś Y")

    if "Front" in selected_series:
        p.line(x=st.session_state.travelPressureTime, y=st.session_state.dfFrontTravel, line_width=2, legend_label='Front', line_color='blue')
    if "Rear" in selected_series:
        p.line(x=st.session_state.travelPressureTime, y=st.session_state.dfRearTravel, line_width=2, legend_label='Rear', line_color='red')
    st.bokeh_chart(p, use_container_width=True)

if selected == "Pressure":
    st.title(f"You have selected {selected}")

if selected == "Statistics":
    st.title(f"You have selected {selected}")