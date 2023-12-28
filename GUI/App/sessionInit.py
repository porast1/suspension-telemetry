import streamlit as st
from filters import load_data
import numpy as np

timePerSample = 0.005
timePerSampleAcc = 0.005

def sessionVariablesInit():
    # Inicjalizacja kontekstu sesji
    if 'suspensionDataUploaded' not in st.session_state:
        st.session_state.suspensionDataUploaded = False
    if 'accelerometerDataUploaded' not in st.session_state:
        st.session_state.accelerometerDataUploaded = False
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
    if 'maxFrontTravel' not in st.session_state:
        st.session_state.maxFrontTravel = None
    if 'maxRearTravel' not in st.session_state:
        st.session_state.maxRearTravel = None
    if 'maxRearStroke' not in st.session_state:
        st.session_state.maxRearStroke = None
def yourBikeSetup():
    st.session_state.maxFrontTravel = st.number_input("Add max front travel:",0,300,170)
    st.session_state.maxRearTravel = st.number_input("Add max Rear travel:",0,300,150)
    st.session_state.maxRearStroke = st.number_input("Add max Rear stroke:",0,300,78)

def sessionDataPreparationTravel(frontTravel,rearTravel):
    st.session_state.dfFrontTravel = load_data(frontTravel)  # Wczytaj dane z pliku tymczasowego
    st.session_state.dfRearTravel =load_data(rearTravel)  # Wczytaj dane z pliku tymczasowego
    lenTime = min(len(st.session_state.dfFrontTravel),len(st.session_state.dfRearTravel))
    return lenTime

def sessionDataPreparationPressure(frontPressure,rearPressure):

    st.session_state.dfFrontPressure = load_data(frontPressure)  # Wczytaj dane z pliku tymczasowego
    st.session_state.dfRearPressure = load_data(rearPressure)  # Wczytaj dane z pliku tymczasowego
    lenTime = min(len(st.session_state.dfFrontPressure), len(st.session_state.dfRearPressure))
    return lenTime


def sessionDataPreparationAccelerometer(accelerometer):
    st.session_state.dfAccelerometer = load_data(accelerometer)  # Wczytaj dane z pliku tymczasowego
    lenTimeAcc = len(st.session_state.dfAccelerometer)
    st.session_state.accelerometerTime = np.arange(0, timePerSampleAcc * lenTimeAcc, timePerSampleAcc)

def cutDataToLenTime(lenTime, travelState, pressureState):
    if travelState:
        st.session_state.travelPressureTime = np.arange(0, timePerSample * lenTime, timePerSample)
        st.session_state.dfFrontTravel = st.session_state.dfFrontTravel[:lenTime]
        st.session_state.dfRearTravel = st.session_state.dfRearTravel[:lenTime]
    if pressureState:
        st.session_state.travelPressureTime = np.arange(0, timePerSample * lenTime, timePerSample)
        st.session_state.dfFrontPressure =  st.session_state.dfFrontPressure[:lenTime]
        st.session_state.dfRearPressure =  st.session_state.dfRearPressure[:lenTime]