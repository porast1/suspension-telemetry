import streamlit as st
import Telemetry.AppFigures as myFig
from myApi.ApiGUI import StreamlitApi

if __name__ == '__main__':
    st.set_page_config(layout="wide")
    
    st_api = StreamlitApi(suspension_data=st.session_state)
    sidebarSelector = st_api.sidebar_selector( "Choose your option", ["Load & Process data", "Charts & Statistics"])
    suspension_data = st_api.suspension_data_frame
    
    if sidebarSelector == "Load & Process data":
        st_api.title("How to start:")    

    if sidebarSelector == "Charts & Statistics" and suspension_data is not None:

        st_api.title("Charts & Statistics")
        myFig.general_figure(st_api, suspension_data)

        myFig.progression_figure(st_api, suspension_data)
        
        myFig.travel_histogram(st_api, suspension_data)


        myFig.fft_figure(st_api, suspension_data)
        
        myFig.velocity_distribution_figure(st_api, suspension_data)

        st_api.header('Balance of suspension')
        myFig.balance_figure(st_api, suspension_data, damping="Rebound")
        myFig.balance_figure(st_api, suspension_data, damping="Compression")
