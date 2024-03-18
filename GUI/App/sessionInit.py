import streamlit as st
from numpy import array, insert, diff, linspace, where, zeros_like, mean
from pandas import read_csv, DataFrame, to_numeric, date_range
from bokeh.plotting import figure, Figure
from scipy.signal import butter, lfilter
from abc import ABC, abstractmethod
class UserApi(ABC):
    def __init__(self):
        #fixed parameters names
        self.__suspension_parameters_template = {
            'data_file_name': None,
            'time': None,
            'front_travel': None,
            'rear_travel': None,
            'rear_stroke': None,
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
    
    def _your_bike_setup(self, suspension_data : dict):
        self.title("Choose your setup")
        suspension_data['max_front_travel'] = self._select_max_travel("Add max front travel:", 180)
        suspension_data['max_rear_travel'] = self._select_max_travel("Add max Rear travel:", 170)
        suspension_data['max_rear_stroke'] = self._select_max_travel("Add max Rear stroke:", 65)

    @abstractmethod
    def read_suspension_data_from_file(self, suspension_data : dict):
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
    @abstractmethod
    def display_text(self, text : str):
        pass
    @abstractmethod
    def sidebar_selector(self, label : str, options : list):
        pass
    @abstractmethod
    def multi_selector(self, label : str, options : list):
        pass
    @abstractmethod
    def header(self, header : str):
        pass
    @abstractmethod
    def subheader(self, subheader : str):
        pass
    @abstractmethod
    def show_figure(self, figure_instance : Figure):
        pass
    
class StreamlitApi(UserApi):
    def __init__(self, suspension_data : dict):
        super().__init__()
        self.__suspension_parameters = self._get_suspension_parameters_template()
        self._your_bike_setup(suspension_data=self.__suspension_parameters)
        self._init_suspension_df(suspension_parameters=self.__suspension_parameters, suspension_data=suspension_data)
        raw_suspension_data = self.read_suspension_data_from_file()
        prepared_data = PrepareData()
        self.__suspension_data = prepared_data.prepare_suspension_data(raw_data=raw_suspension_data, suspension_data=suspension_data)
    def read_suspension_data_from_file(self):
        uploaded_file = self.file_uploader(label="Upload a CSV file", type=["csv"])
        data = None
        if ( uploaded_file is not None):
            data = read_csv(uploaded_file, delimiter=';')
        return data
    def _init_suspension_df(self, suspension_parameters : dict, suspension_data : dict ):
        for parameter, default_value in suspension_parameters.items():
            if parameter not in suspension_data:
                suspension_data[parameter] = default_value

    def _select_max_travel(self, label : str, travel : int, max_travel : int = 310):
        slider = st.number_input(f'{label}',0, max_travel, travel)
        return slider
    def file_uploader(self, label : str, type : str):
        file = st.sidebar.file_uploader(label, type=type)
        return file
    def title(self, title : str):
        st.title(f"{title}")
    def display_text(self, text : str):
        st.write(f"{text}")
    def sidebar_selector(self, label : str, options : list):
        return st.sidebar.selectbox(label, options)
    def multi_selector(self, label : str, options : list):
        return st.multiselect(label, options)
    def header(self, header : str):
        st.header(header)
    def subheader(self, subheader : str):
        st.subheader(subheader)
    def show_figure(self, figure_instance : Figure):
        st.bokeh_chart(figure_instance, use_container_width=True)
    @property
    def suspension_data_frame(self):
        return self.__suspension_data

class PrepareData:
    def __init__(self):
        pass
    def prepare_suspension_data(self, raw_data : DataFrame, suspension_data : dict, sampling_time : float = 0.005, interpolate_const : int = 5):
        prepared_suspension_data = None
        if raw_data is not None:
            prepared_suspension_data = suspension_data
            prepared_data = self._prepare_data(data=raw_data, sampling_time=sampling_time, interpolate_const=interpolate_const)
            prepared_suspension_data['front_travel'] = array(prepared_data["FrontTravel"] if "FrontTravel" in prepared_data.columns else None)
            prepared_suspension_data['rear_stroke'] = array(prepared_data["RearTravel"] if "RearTravel" in prepared_data.columns else None)
            prepared_suspension_data['front_pressure'] = array(prepared_data["FrontPressure"] if "FrontPressure" in prepared_data.columns else None)
            prepared_suspension_data['rear_pressure'] = array(prepared_data["RearPressure"] if "RearPressure" in prepared_data.columns else None)
            prepared_suspension_data['rear_travel'] = self.__calculateRearTravelFromStroke(suspension_data)
            prepared_suspension_data['front_travel_percent'] = self.__calculateTravelPercent(suspension_data['front_travel'], suspension_data['max_front_travel'])
            prepared_suspension_data['rear_travel_percent'] = self.__calculateTravelPercent(suspension_data['rear_travel'], suspension_data['max_rear_travel'])
            prepared_suspension_data['front_velocity'] = self.__calculateVelocity(samples=suspension_data['front_travel'], sample_time=(sampling_time / interpolate_const))
            prepared_suspension_data['rear_velocity'] = self.__calculateVelocity(samples=suspension_data['rear_travel'], sample_time=(sampling_time / interpolate_const))
            prepared_suspension_data['time'] = linspace(0,(sampling_time / interpolate_const) * (prepared_data.shape[0]-1),prepared_data.shape[0])
        return prepared_suspension_data

    @st.cache_data
    def _prepare_data(_self, data : DataFrame, sampling_time : float, interpolate_const : float):
        if data is not None and len(data) > 0:
            samplingTime_ms = sampling_time * 1000
            data = data.apply(to_numeric, errors='coerce')
            index = date_range(start='2024-02-07 00:00:00', periods=len(data), freq=f'{samplingTime_ms}ms')
            data.index = index
            resampleTime = samplingTime_ms/interpolate_const
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
    def __calculateVelocity(_self, samples : list, sample_time : float):
        differences = diff(samples)
        derivative = differences / sample_time
        derivative = insert(derivative, 0, 0)
        return derivative
    

    def __calculateRearTravelFromStroke(self, suspension_data : dict):
        normalizer = suspension_data['max_rear_travel'] / suspension_data['max_rear_stroke']
        return (normalizer * suspension_data['rear_stroke'])
    
    @st.cache_data
    def __calculateTravelPercent(_self, travel_data : list, maxTravel):
        nonzero_mask = (travel_data != 0)
        percent_values = zeros_like(travel_data)
        percent_values[nonzero_mask] = 100 * (travel_data[nonzero_mask] / maxTravel)
        return percent_values
class BaseFigure(ABC):

    def __init__(self):
        pass

    def avg_travel(self, travel: list[float]):
        return mean(travel)

    def max_travel(self, travel: list[float]):
        return max(travel)
    
    def calculate_avg_comp_velocity(self, velocity: list[float]):
        maskComp = velocity >= 0
        comp = velocity[maskComp]
        avgComp = mean(comp)
        return avgComp
    
    def calculate_avg_reb_velocity(self, velocity: list[float]):
        maskReb = velocity < 0
        reb = velocity[maskReb]
        avgReb = mean(reb)
        return avgReb
    
    def calculate_max_velocity(self, velocity: list[float]):
        return max(velocity)
    
    def calculate_min_velocity(self, velocity: list[float]):
        return min(velocity)
    
    def create_empty_plot(self):
        figure_instance = figure(
            height = 350,
            min_border_left=70,
            min_border_right=50,
            sizing_mode='stretch_both',
            toolbar_location='above',
            tools='xpan,xwheel_zoom,box_zoom,reset, hover, save',
            active_drag='xpan',
            output_backend='webgl')
        figure_instance.circle([],[])
        return figure_instance
    @abstractmethod
    def figure_data(self, figure_instance : Figure, legend_label : str, line_color : str):
        pass
    @abstractmethod
    def label_XY(self, figure_instance : Figure):
        pass

    #figures
    def create_figure(self, figure_instance, legend_label : str, line_color : str):
        p = self.label_XY(figure_instance=figure_instance)
        p = self.figure_data(figure_instance=figure_instance, legend_label= legend_label, line_color = line_color)
        return p

        
