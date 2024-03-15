import streamlit as st
from numpy import array, insert, diff, linspace, where, zeros_like, mean
from pandas import read_csv, DataFrame, to_numeric, date_range
from bokeh.plotting import figure, Figure
from scipy.signal import butter, lfilter
from abc import ABC, abstractmethod
class UserApi(ABC):
    def __init__(self):
        self.__suspension_parameters_template = {
            'data_file_name': None,
            'time': None,
            'front_travel': None,
            'rear_travel': None,
            'front_travel_percent': None,
            'rear_travel_percent': None,
            'front_velcty' : None,
            'rear_velocity' : None,
            'front_pressure': None,
            'rear_pressure': None,
            'max_front_travel': None,
            'max_rear_travel': None,
            'max_rear_stroke': None
        }
    def _get_suspension_parameters_template(self):
        return self.__suspension_parameters_template.copy()
    
    def _your_bike_setup(self):
        self.title("Choose your setup")
        self.__suspension_parameters_template['max_front_travel'] = self._select_max_travel("Add max front travel:", 180)
        self.__suspension_parameters_template['max_rear_travel'] = self._select_max_travel("Add max Rear travel:", 170)
        self.__suspension_parameters_template['max_rear_stroke'] = self._select_max_travel("Add max Rear stroke:", 65)

    @abstractmethod
    def upload_file_with_data(self, suspension_data : dict):
        pass
    
    @abstractmethod
    def _init_suspension_df(self):
        pass

    @abstractmethod
    def _select_max_travel(self, label : str, travel : int, max_travel : int = 310):
        pass
    @abstractmethod
    def file_uploader(self, label : str, type : str):
        pass
    @abstractmethod
    def title(self, title : str):
        pass
    
class StreamlitApi(UserApi):
    def __init__(self, session_state : dict):
        super().__init__()
        self.__suspension_parameters = self._get_suspension_parameters_template()
        self._init_suspension_df(suspension_parameters=self.__suspension_parameters)
        self._your_bike_setup(session_variables=self.__suspension_parameters)

    def upload_file_with_data(self, suspension_data : dict):
        uploaded_file = self.file_uploader(label="Upload a CSV file", type=["csv"])
        status = False
        data = None
        if ( uploaded_file is not None) and (uploaded_file.name != suspension_data['data_file_name']):
            suspension_data['data_file_name'] = uploaded_file.name
            data = read_csv(uploaded_file, delimiter=';')
            status = True
        elif suspension_data['data_file_name'] is not None:
            status = True
        else:
            status = False
            data = None
        return status, data
    def _init_suspension_df(self, suspension_parameters : dict, suspension_data : dict ):
        for parameter, default_value in suspension_parameters.items():
            if parameter not in suspension_data:
                suspension_data[parameter] = default_value

    def _select_max_travel(self, label : str, travel : int, max_travel : int = 310):
        slider = st.number_input(f'{label}',0, max_travel, travel)
        return slider
    def file_uploader(self, label : str, type : str):
        file = st.sidebar.file_uploader(label, type=[type])
        return file
    def title(self, title : str):
        st.title(f"{title}")
    @property
    def suspension_data_frame(self):
        return st.session_state

class PrepareData:
    def __init__(self):
        pass
    def prepare_suspension_data(self, file_data : DataFrame, samplingTime : float = 0.005, interpolateConstanst : int = 5, suspension_df : dict = st.session_state):
        if file_data is not None:
            prepared_data = self._prepare_data(data=file_data, samplingTime=samplingTime, interpolateConst=interpolateConstanst)
            suspension_df['front_travel'] = array(prepared_data["FrontTravel"] if "FrontTravel" in prepared_data.columns else None)
            rear_stroke = array(prepared_data["RearTravel"] if "RearTravel" in prepared_data.columns else None)
            suspension_df['rear_travel'] = self.__calculateRearTravelFromStroke(rear_stroke)
            suspension_df['front_travel_percent'] = self.__calculateTravelPercent(suspension_df['front_travel'], suspension_df['max_front_travel'])
            suspension_df['rear_travel_percent'] = self.__calculateTravelPercent(suspension_df['rear_travel'], suspension_df['max_rear_travel'])
            suspension_df['front_velocity'] = self.__calculateVelocity(samples=suspension_df['front_travel'], sampleTime=(samplingTime / interpolateConstanst))
            suspension_df['rear_velocity'] = self.__calculateVelocity(samples=suspension_df['rear_travel'], sampleTime=(samplingTime / interpolateConstanst))

            suspension_df['front_pressure'] = array(prepared_data["FrontPressure"] if "FrontPressure" in prepared_data.columns else None)
            suspension_df['rear_pressure'] = array(prepared_data["RearPressure"] if "RearPressure" in prepared_data.columns else None)

            suspension_df['time'] = linspace(0,(samplingTime / interpolateConstanst) * (prepared_data.shape[0]-1),prepared_data.shape[0])

    @st.cache_data
    def _prepare_data(_self, data : DataFrame, samplingTime : float, interpolateConst : float):
        if data is not None and len(data) > 0:
            samplingTime_ms = samplingTime * 1000
            data = data.apply(to_numeric, errors='coerce')
            index = date_range(start='2024-02-07 00:00:00', periods=len(data), freq=f'{samplingTime_ms}ms')
            data.index = index
            resampleTime = samplingTime_ms/interpolateConst
            for column in data.columns:
                data[column] = _self.__butter_lowpass_filter(data[column], 10, 200)
            data = data.resample(f'{resampleTime}ms').interpolate(method='cubic')
            data = data.mask(data < 0, 0)
            data = data.dropna()
            return data.iloc[200:]
        else:
            return None
        
    @st.cache_data
    def __butter_lowpass(_self,cutoff, fs, order=10):
        nyq = 0.5 * fs
        normal_cutoff = cutoff / nyq
        b, a = butter(order, normal_cutoff, btype='low', analog=False)
        return b, a
    
    @st.cache_data
    def __butter_lowpass_filter(_self,data, cutoff, fs, order=10):
        b, a = _self.__butter_lowpass(cutoff, fs, order=order)
        y = lfilter(b, a, data)
        return y
    
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

    def __init__(self):
        pass

    
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
