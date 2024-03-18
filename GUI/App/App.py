import streamlit as st
import pandas as pd
from balance import balance_data
from Telemetry import GeneralTravelFigure, travel_histogram, general_figure
from velocity import VelocityHistogram
from pressure import pressure_figure, minPressure
from sessionInit import StreamlitApi, PrepareData, UserApi
from myFFT import fftFigure
from progression import ProgressionFigure
import numpy as np
from bokeh.layouts import column


if __name__ == '__main__':
    st.set_page_config(layout="wide")
    
    st_api = StreamlitApi(suspension_data=st.session_state)
    sidebarSelector = st_api.sidebar_selector( "Choose your option", ["Load & Process data", "Charts & Statistics"])
    suspension_data = st_api.suspension_data_frame
    if sidebarSelector == "Load & Process data":
        st_api.title("How to start:")    
        #st_api.display_text("1. Load the data first, if you are using only one shock absorber then load the data for front and rear at the same time, same for pressure")
    

    if sidebarSelector == "Charts & Statistics" and suspension_data is not None:
        st_api.title("Charts & Statistics")
        general_figure(st_api, suspension_data)

        # progressionFigure = ProgressionFigure()
        # progressionFigure.createFigure()
        
        travel_histogram(st_api, suspension_data)

        # st_api.header('FFT Charts')
        # frontFFT = fftFigure()
        # frontFFT.createFigure()
        # rearFFT = fftFigure('Rear')
        # rearFFT.createFigure()
        
        # frontVelocityHistogram = VelocityHistogram(dataFrom='Front')
        # frontVelocityHistogram.createFigure()
        # rearVelocityHistogram = VelocityHistogram(dataFrom='Rear')
        # rearVelocityHistogram.createFigure()

        # st_api.header('Balance')
        # st_api.subheader('Rebound')
        # reboundBalance = balance_data(bikeSetup="Reb")
        # reboundBalance.createFigure()
        # st_api.subheader('Compression')
        # compressionBalance = balance_data(bikeSetup="Comp")
        # compressionBalance.createFigure()