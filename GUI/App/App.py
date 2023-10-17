import streamlit as st
from balance import balance_data, balance_Figure
import Telemetry as t
import velocity as vel
from pressure import pressure_figure, minPressure
import sessionInit
from myFFT import my_fft_figure
from progression import suspension_Progression_Data, travel_progression_figure, progressionDataConvert
st.set_page_config(layout="wide")
sessionInit.sessionVariablesInit()
with st.sidebar:
    selected = st.selectbox(
        "Main Menu",
        ["Load & Process data", "Charts & Statistics"]
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
if selected == "Load & Process data":
    st.title("How to start:")    
    st.write("1. Load the data first, if you are using only one shock absorber then load the data for front and rear at the same time, same for pressure")
    #(selected == "Load & Process data") and 
if ((frontTravel and rearTravel) or (frontPressure and rearPressure)):
    st.title("Choose your setup")
    sessionInit.yourBikeSetup()
    lenTime = []
    if(frontTravel is not None and
        rearTravel is not None):
            lenTime.append(sessionInit.sessionDataPreparationTravel(frontTravel, rearTravel))
            st.session_state.suspensionDataUploaded = True  # Ustaw stan sesji na True
    if(frontPressure is not None and
        rearPressure is not None):
            lenTime.append(sessionInit.sessionDataPreparationPressure(frontPressure, rearPressure))  
            st.session_state.pressureDataUploaded = True  # Ustaw stan sesji na True
    if(accelerometer is not None):
            lenTimeAcc = sessionInit.sessionDataPreparationAccelerometer(accelerometer)
            st.session_state.accelerometerDataUploaded = True  # Ustaw stan sesji na True
    if (st.session_state.pressureDataUploaded or st.session_state.suspensionDataUploaded):
        sessionInit.cutDataToLenTime(min(lenTime), st.session_state.suspensionDataUploaded, st.session_state.pressureDataUploaded)
        st.success("Dane Przetworzone")
        
else:
    sessionInit.sessionVariablesInit()

# Aktualizacja warunku dla widoku "Rebound/Comp"
if selected == "Charts & Statistics" and st.session_state.suspensionDataUploaded:
    #Short names of st.session..
    sampleTimeAdc = 0.005
    travelPressureTime = st.session_state.travelPressureTime 
    FrontTravel = st.session_state.dfFrontTravel
    RearStroke = st.session_state.dfRearTravel

    maxFrontTravel = st.session_state.maxFrontTravel
    maxRearStroke = st.session_state.maxRearStroke
    maxRearTravel = st.session_state.maxRearTravel
    maxFrontTravelCalculate = t.maxTravel(FrontTravel,10)
    maxRearStrokeCalculate = t.maxTravel(RearStroke,10)

    if maxFrontTravelCalculate >= maxFrontTravel + 10:
        maxFrontTravel = int(maxFrontTravelCalculate)
        st.write('Data from the measurement sensor say that the maximum Front stroke is: ', maxFrontTravel)
        st.write('the maximum stroke was set according to the value on the sensor')
    if maxRearStrokeCalculate >= maxRearTravel + 10:
        maxRearStroke = int(maxRearStrokeCalculate)
        st.write('Data from the measurement sensor say that the maximum Rear stroke is: ', maxRearStroke)
        st.write('the maximum stroke was set according to the value on the sensor')
    
    normalizer = maxRearTravel/maxRearStroke
    RearTravel = normalizer * RearStroke
    frontVelocity = t.calculate_derivative(FrontTravel, sampleTimeAdc)
    rearVelocity = t.calculate_derivative(RearTravel, sampleTimeAdc)
    FrontTravelPercent = 100 * FrontTravel/maxFrontTravel
    RearTravelPercent = 100 * RearTravel/maxRearTravel
    if (st.session_state.pressureDataUploaded):
        FrontPressure = st.session_state.dfFrontPressure 
        RearPressure = st.session_state.dfRearPressure 
        minFrontPressure = minPressure(FrontTravel, FrontPressure)
        minRearPressure = minPressure(RearTravel, RearPressure)
    accelerometerTime = st.session_state.accelerometerTime 

    stepDamping = 100



    # Title
    st.title(f"{selected}")
    st.header("General diagram of suspension work")
    selectedSeries = st.multiselect("Choose data series:", ["Front", "Rear"])
    travelFigure = t.travel_figure(selectedSeries,FrontTravel,RearTravel,travelPressureTime)
    st.bokeh_chart(travelFigure, use_container_width=True)

    st.header('Travel/Spring rate')
    st.write('this graph shows the average - dynamic progression of our suspension based on readings from the travel sensor. Very useful for finding the balance of our suspension in terms of overall hardness. The overall hardness is influenced by the slow-fast compression settings, the suspension hardness and the number of tokens. Therefore, this chart must be considered on the basis of the charts below, first we set the relative balance of the air spring/sag hardness, then the balance settings for compression and come back here to verify the settings')

    progressionSelectedSeries = st.selectbox("Choose figure type:", ["Progression", "Velocity"])
    progressionSelectedX_axis = st.selectbox("Choose display option:", ["Travel %", "Travel mm"])
    percentTravel, progressionAvgVelocityFront  = suspension_Progression_Data(FrontTravelPercent, frontVelocity)
    percentTravel, progressionAvgVelocityRear  = suspension_Progression_Data(RearTravelPercent, rearVelocity)
    if progressionSelectedSeries == "Velocity":
        progressionFigure = travel_progression_figure(progressionSelectedX_axis, progressionAvgVelocityFront, progressionAvgVelocityRear, percentTravel, maxFrontTravel, maxRearTravel)
        st.bokeh_chart(progressionFigure, use_container_width=True)
    else: 
        progressionFrontTime = progressionDataConvert(progressionAvgVelocityFront, percentTravel)
        progressionRearTime = progressionDataConvert(progressionAvgVelocityRear, percentTravel)
        progressionFigure = travel_progression_figure(progressionSelectedX_axis, progressionFrontTime, progressionRearTime, percentTravel, maxFrontTravel, maxRearTravel)
        st.bokeh_chart(progressionFigure, use_container_width=True)
    if (frontPressure is not None and
        rearPressure is not None):
        st.subheader("Pressure")
        st.write("Here you can see the pressure graphs of your suspension, a particularly useful option is 'Progression' which shows the characteristics of pressure progression in the shock absorber chambers. This can be especially useful when setting the number of tokens.")
        selectedPressure = st.selectbox(
            "",
            ["Pressure", "Progression"]
        )
        pressureFigure = pressure_figure(FrontTravelPercent,RearTravelPercent,FrontPressure, RearPressure, selectedPressure)
        st.bokeh_chart(pressureFigure, use_container_width=True)

    st.write('This section describes the hardness of our suspension, the first two graphs tell us in what sections of the travel we spend %% of the time. \nExample: The further to the left = stiffer suspension, to the right = the softer \nor simply: The softer the suspension, the more time we will spend in the upper travel values and vice versa.')

    fs = 1/sampleTimeAdc
    col1, col2 = st.columns(2)
    with col1:
        st.subheader("Front")
        frontTravelHist = t.travel_histogram_figure(FrontTravel,maxFrontTravel )
        st.bokeh_chart(frontTravelHist, use_container_width=True)
        frontTravelFFT = my_fft_figure(FrontTravel,fs)
        st.bokeh_chart(frontTravelFFT, use_container_width=True)
    with col2:
        st.subheader("Rear")
        frontTravelHist = t.travel_histogram_figure(RearTravel, maxRearTravel)
        st.bokeh_chart(frontTravelHist, use_container_width=True)
        rearTravelFFT = my_fft_figure(RearTravel,fs)
        st.bokeh_chart(rearTravelFFT, use_container_width=True)

    #speed histograms
    st.header('Damping')
    st.write('This section describes the suspension speed distribution over time. The percentages in which a given speed occurred in the travel are marked in colors on the sub-bars. The graphs should look somewhat similar for the front and rear suspension, and most importantly, resemble the Gaussian normal distribution curve marked with a red dashed line.')
    st.subheader('Front')
    frontVelocityFigure = vel.velocity_histogram_figure(FrontTravelPercent,frontVelocity,stepDamping)
    st.bokeh_chart(frontVelocityFigure, use_container_width=True)
    st.subheader('Rear')
    rearVelocityFigure = vel.velocity_histogram_figure(RearTravelPercent,rearVelocity,stepDamping)
    st.bokeh_chart(rearVelocityFigure, use_container_width=True)

    st.header("Balance")
    st.write('This section describes the front/rear balance characteristics. Using the linear regression curve, we can see the speed at which the piston moves at a given moment of the suspension stroke. Thanks to this, we can precisely set the suspension speed at the front and rear to make it as balanced as possible!')
    st.subheader('Rebound')
    a_rebFront, b_rebFront, travelReboundFront = balance_data(frontVelocity, FrontTravel,"Reb")
    a_rebRear, b_rebRear, travelReboundRear = balance_data(rearVelocity, RearTravel,"Reb")
    velocityRegressionFrontRebound = a_rebFront * travelReboundFront + b_rebFront
    velocityRegressionRearRebound = a_rebRear * travelReboundRear + b_rebRear
    velocityBalanceReboundFigure = balance_Figure(a_rebFront, a_rebRear, travelReboundFront, travelReboundRear,
                                                  velocityRegressionFrontRebound,velocityRegressionRearRebound,
                                                   maxFrontTravel, maxRearTravel, progressionSelectedX_axis )
    st.bokeh_chart(velocityBalanceReboundFigure, use_container_width=True)


    st.subheader('Comp')
    a_compFront, b_compFront, travelCompressionFront = balance_data(frontVelocity, FrontTravel,"Comp")
    a_compRear, b_compRear, travelCompressionRear = balance_data(rearVelocity, RearTravel,"Comp")
    velocityRegressionFrontCompression = a_compFront * travelCompressionFront + b_compFront
    velocityRegressionRearCompression = a_compRear * travelCompressionRear + b_compRear
    velocityBalanceCompressionFigure = balance_Figure(a_compFront, a_compRear, travelCompressionFront, travelCompressionRear,
                                                  velocityRegressionFrontCompression,velocityRegressionRearCompression,
                                                   maxFrontTravel, maxRearTravel, progressionSelectedX_axis )
    st.bokeh_chart(velocityBalanceCompressionFigure, use_container_width=True)