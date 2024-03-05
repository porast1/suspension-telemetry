import streamlit as st
from numpy import array, insert, diff, linspace, where, zeros_like, mean
from pandas import read_csv, DataFrame, to_numeric, date_range
from bokeh.plotting import figure, Figure
class SessionDataInit:
    #private
    __uploaded_file = None
    #public
    def __init__(self, samplingTime : float, interpolateConstanst : int):
        self.__initSessionVariables()
        self.__yourBikeSetup()
        #dane
        SessionDataInit.__uploaded_file = st.sidebar.file_uploader("Upload a CSV file", type=["csv"])
        if ( SessionDataInit.__uploaded_file is not None) and (SessionDataInit.__uploaded_file.name != st.session_state.dataFileName):
            data = read_csv(SessionDataInit.__uploaded_file, delimiter=';')
            data = self.__load_data(data=data,samplingTime=samplingTime, interpolateConst=interpolateConstanst)
            if data is not None:
                st.session_state.dfFrontTravel = array(data["FrontTravel"] if "FrontTravel" in data.columns else None)
                st.session_state.dfRearTravel = array(data["RearTravel"] if "RearTravel" in data.columns else None)
                st.session_state.dfRearTravel = self.__calculateRearTravelFromStroke(st.session_state.dfRearTravel)
                with open('RearTravel.txt', 'w') as ft:
                # Zapisujemy całą listę do pliku, łącząc elementy za pomocą znaku nowej linii
                    ft.write('\n'.join(map(str, st.session_state.dfRearTravel)))
                st.session_state.dfFrontTravelPercent = self.__calculateTravelPercent(st.session_state.dfFrontTravel, st.session_state.maxFrontTravel)
                st.session_state.dfRearTravelPercent = self.__calculateTravelPercent(st.session_state.dfRearTravel, st.session_state.maxRearTravel)
                with open('RearTravelPercent.txt', 'w') as fp:
                # Zapisujemy całą listę do pliku, łącząc elementy za pomocą znaku nowej linii
                    fp.write('\n'.join(map(str, st.session_state.dfRearTravelPercent)))
                st.session_state.dfFrontPressure = array(data["FrontPressure"] if "FrontPressure" in data.columns else None)
                st.session_state.dfRearPressure = array(data["RearPressure"] if "RearPressure" in data.columns else None)
                st.session_state.travelPressureTime = linspace(0,(samplingTime / interpolateConstanst) * (data.shape[0]-1),data.shape[0])
                st.session_state.dfFrontVelocity = self.__calculateVelocity(samples=st.session_state.dfFrontTravel, sampleTime=(samplingTime / interpolateConstanst))
                st.session_state.dfRearVelocity = self.__calculateVelocity(samples=st.session_state.dfRearTravel, sampleTime=(samplingTime / interpolateConstanst))
                with open('RearVelocity.txt', 'w') as f:
                # Zapisujemy całą listę do pliku, łącząc elementy za pomocą znaku nowej linii
                    f.write('\n'.join(map(str, st.session_state.dfRearVelocity)))
                st.session_state.dataFileName = SessionDataInit.__uploaded_file.name
                st.session_state.suspensionDataUploaded = True
                st.success("Dane zostały pomyślnie wczytane!")
            else:
                st.session_state.suspensionDataUploaded = False
                st.error("Brak danych do wczytania.")
            
    def __initSessionVariables(self):
         # Lista zmiennych sesji do zainicjalizowania
        session_variables = {
            'suspensionDataUploaded': False,
            'dataFileName': False,
            'travelPressureTime': None,
            'dfFrontTravel': None,
            'dfRearTravel': None,
            'dfFrontTravelPercent': None,
            'dfRearTravelPercent': None,
            'dfFrontVelocity' : None,
            'dfRearVelocity' : None,
            'dfFrontPressure': None,
            'dfRearPressure': None,
            'maxFrontTravel': None,
            'maxRearTravel': None,
            'maxRearStroke': None
        }
        # Inicjalizacja kontekstu sesji
        for variable_name, default_value in session_variables.items():
            if variable_name not in st.session_state:
                st.session_state[variable_name] = default_value

    def __yourBikeSetup(self):
        st.title("Choose your setup")
        st.session_state.maxFrontTravel = st.number_input("Add max front travel:",0,300,180)
        st.session_state.maxRearTravel = st.number_input("Add max Rear travel:",0,300,170)
        st.session_state.maxRearStroke = st.number_input("Add max Rear stroke:",0,300,65)

    @st.cache_data
    def __load_data(_self, data : DataFrame, samplingTime : float, interpolateConst : float):
        if data is not None and len(data) > 0:
            samplingTime_ms = samplingTime * 1000
            data = data.apply(to_numeric, errors='coerce')
            index = date_range(start='2024-02-07 00:00:00', periods=len(data), freq=f'{samplingTime_ms}ms')
            data.index = index
            #interpolation
            resampleTime = samplingTime_ms/interpolateConst
            data = data.resample(f'{resampleTime}ms').interpolate(method='cubic')
            data = data.mask(data < 0, 0)
            return data
        else:
            return None
    @st.cache_data
    def __calculateVelocity(_self, samples : list, sampleTime : float):
        differences = diff(samples)
        derivative = differences / sampleTime
        derivative = insert(derivative, 0, 0)
        return derivative
    @st.cache_data
    def __calculateRearTravelFromStroke(_self, strokeData : list):
        normalizer = st.session_state.maxRearTravel / st.session_state.maxRearStroke
        return (normalizer * strokeData)
    @st.cache_data
    def __calculateTravelPercent(_self, travelData : list, maxTravel):
        nonzero_mask = (travelData != 0)
        percent_values = zeros_like(travelData)
        percent_values[nonzero_mask] = 100 * (travelData[nonzero_mask] / maxTravel)
        return percent_values
class BaseDataAndFigure:
    #private
    _frontTravel = None
    _rearTravel = None
    _frontTravelPercent = None
    _rearTravelPercent = None
    _frontVelocity = None
    _rearVelocity = None
    _frontPressure = None
    _rearPressure = None
    _timeTravelPressure = None
    _maxFrontTravel = None
    _maxRearTravel = None

    #public
    def __init__(self):
        self._frontTravel = st.session_state.dfFrontTravel
        self._rearTravel = st.session_state.dfRearTravel
        self._frontTravelPercent = st.session_state.dfFrontTravelPercent
        self._rearTravelPercent =  st.session_state.dfRearTravelPercent
        self._frontVelocity = st.session_state.dfFrontVelocity
        self._rearVelocity = st.session_state.dfRearVelocity
        self._frontPressure = st.session_state.dfRearTravel
        self._rearPressure = st.session_state.dfRearTravel
        self._timeTravelPressure = st.session_state.travelPressureTime
        self._maxFrontTravel = st.session_state.maxFrontTravel
        self._maxRearTravel = st.session_state.maxRearTravel

    # Getters
    def getFrontTravel(self):
        return self._frontTravel

    def getRearTravel(self):
        return self._rearTravel

    def getFrontTravelPercent(self):
        return self._frontTravelPercent

    def getRearTravelPercent(self):
        return self._rearTravelPercent

    def getFrontVelocity(self):
        return self._frontVelocity

    def getRearVelocity(self):
        return self._rearVelocity

    def getFrontPressure(self):
        return self._frontPressure

    def getRearPressure(self):
        return self._rearPressure

    def getTimeTravelPressure(self):
        return self._timeTravelPressure

    def getMaxFrontTravel(self):
        return self._maxFrontTravel

    def getMaxRearTravel(self):
        return self._maxRearTravel
    def calculateAvgandMaxVelocity(self, velocity: list[float]):
        maskComp = velocity >= 0
        maskReb = velocity < 0
        comp = velocity[maskComp]
        reb = velocity[maskReb]
        avgComp = mean(comp)
        avgReb = mean(reb)
        maxVelocityComp = max(velocity)
        maxVelocityReb = min(velocity)
        return avgComp, maxVelocityComp, avgReb,  maxVelocityReb
    
    #figure abstraction
    def simpleFigureAbstract(self, abstractFigure : Figure):
        pass
    def XY_LabelAbstract(self, abstractFigure : Figure):
        pass

    #figures
    def simpleFigure(self):
        p = figure(
            height = 350,
            min_border_left=70,
            min_border_right=50,
            sizing_mode='stretch_both',
            toolbar_location='above',
            tools='xpan,xwheel_zoom,box_zoom,reset, hover, save',
            active_drag='xpan',
            output_backend='webgl')
        p = self.XY_LabelAbstract(abstractFigure=p)
        p.circle([],[])
        p = self.simpleFigureAbstract(abstractFigure=p)
        return p
    def createFigure (self):
        classFigure = self.simpleFigure()
        st.bokeh_chart(classFigure, use_container_width=True)