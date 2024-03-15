import streamlit as st
import pandas as pd
from balance import balance_data
from Telemetry import GeneralTravelFigure, TravelHistogram
from velocity import VelocityHistogram
from pressure import pressure_figure, minPressure
from sessionInit import SessionDataInit,StreamlitApi, BaseDataAndFigure
from myFFT import fftFigure
from progression import ProgressionFigure
import numpy as np
if __name__ == '__main__':
    st.set_page_config(layout="wide")
    my_api = StreamlitApi()
    file_status, suspension_raw_data = my_api.upload_file_with_data()
    my_api.prepare_suspension_data(suspension_raw_data)
    sidebarSelector = st.sidebar.selectbox(
        "Main Menu",
        ["Load & Process data", "Charts & Statistics"])
    
    if sidebarSelector == "Load & Process data":
        st.title("How to start:")    
        st.write("1. Load the data first, if you are using only one shock absorber then load the data for front and rear at the same time, same for pressure")
    

    if sidebarSelector == "Charts & Statistics" and file_status:
        st.title(f"{sidebarSelector}")
        baseTravelFigure = GeneralTravelFigure()
        baseTravelFigure.createFigure()

        progressionFigure = ProgressionFigure()
        progressionFigure.createFigure()
        
        st.subheader('Stiffness Histogram')
        howManyBins = 20
        travelHistogram = TravelHistogram(bins=howManyBins)
        travelHistogram.createFigure()

        st.header('FFT Charts')
        frontFFT = fftFigure()
        frontFFT.createFigure()
        rearFFT = fftFigure('Rear')
        rearFFT.createFigure()
        
        frontVelocityHistogram = VelocityHistogram(dataFrom='Front')
        frontVelocityHistogram.createFigure()
        rearVelocityHistogram = VelocityHistogram(dataFrom='Rear')
        rearVelocityHistogram.createFigure()

        st.header('Balance')
        st.subheader('Rebound')
        reboundBalance = balance_data(bikeSetup="Reb")
        reboundBalance.createFigure()
        st.subheader('Compression')
        compressionBalance = balance_data(bikeSetup="Comp")
        compressionBalance.createFigure()