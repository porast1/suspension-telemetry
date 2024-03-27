import numpy as np 
from bokeh.plotting import Figure

from myApi.ApiFigure import BokehFigure
from streamlit import cache_data
from scipy.signal import medfilt
from scipy.optimize import curve_fit

class ProgressionFigure(BokehFigure):
    def __init__(self, data_travel : list, data_velocity : list,  legend_label : str, Mode : str):
        super().__init__()
        data_travel = data_travel[:]
        data_velocity = data_velocity[:]

        self.__XY_label_source = self.template_XY_label_source()
        self.__data_source = self.template_data_source()
        self.__text_source = self.template_text_source(legend_label=legend_label)

        unique_travel, unique_velocity = self.__calculate_unique_velocity(data_travel=data_travel, data_velocity=data_velocity)

        self.__XY_label_source['x'] = 'Travel [%]'

        self.__data_source['x'] = unique_travel
        
        self.__text_source['legend_location'] = 'top_left'

        if 'Progression' == Mode:
            progression_time = self.__calculate_progression(unique_travel=unique_travel, unique_velocity=unique_velocity)
            self.__XY_label_source['y'] = 'Time [s]'
            self.__data_source['y'] = progression_time
        else:
            self.__XY_label_source['y'] = 'Velocity [mm/s]'
            self.__data_source['y'] = unique_velocity
            progression_time = None
    
    def create_figure(self, figure_instance : Figure):
        figure_instance = super().create_figure(figure_instance=figure_instance)

        XY_label_source = self.__XY_label_source
        data_source = self.__data_source
        text_source = self.__text_source
        
        self._configure_axes(figure_instance=figure_instance, label_source=XY_label_source)
        self._figure_line(figure_instance=figure_instance, data_source=data_source, text_source=text_source)
        return figure_instance
    
    def __calculate_curve_velocity(self, data_velocity : list, data_travel : list ):
        mask = data_velocity >= 0
        indices = np.where(mask)[0]
        data_velocity = data_velocity[indices]
        data_travel = data_travel[indices]
        def quadratic_func(x, a, ha, b, hb, c):
            return a * (x-ha)**2 +b*(x - hb) + c
        popt, pcov = curve_fit(quadratic_func, data_travel, data_velocity)
        a_fit, ha_fit, b_fit, hb_fit, c_fit = popt
        x_data = np.linspace(0, max(data_travel), 1000)
        y_data = quadratic_func(x_data, a_fit, ha_fit, b_fit, hb_fit, c_fit)
        return x_data, y_data
    
    @cache_data
    def __calculate_unique_velocity(_self, data_velocity : list, data_travel : list ):
        mask = data_velocity >= 0
        indices = np.where(mask)[0]
        data_velocity = data_velocity[indices]
        data_travel = data_travel[indices]
        sorted_indices = np.argsort(data_travel)
        sorted_velocity = data_velocity[sorted_indices]
        sorted_travel = np.sort(data_travel)

        travel_bins = np.arange(0, max(sorted_travel), max(sorted_travel) * 0.01)  
        bin_indices = np.digitize(sorted_travel, travel_bins)
        unique_velocity = [np.mean(sorted_velocity[bin_indices == i]) if np.any(bin_indices == i) else np.nan for i in range(1, len(travel_bins))]

        unique_velocity = np.array(unique_velocity)
        non_nan_indices = np.arange(len(unique_velocity))[~np.isnan(unique_velocity)]
        non_nan_values = unique_velocity[non_nan_indices]
        unique_velocity = np.interp(np.arange(len(unique_velocity)), non_nan_indices, non_nan_values)

        unique_velocity = medfilt(unique_velocity, 11)
        window_size = 8
        window = np.ones(window_size) / window_size
        unique_velocity = np.convolve(unique_velocity, window, 'same')


        unique_velocity[0] = 0
        unique_velocity[100:] = 0
        travel_bins = travel_bins[:-1]

        # travel_curve, velocity_curve = _self.__calculate_curve_velocity(data_velocity=data_velocity, data_travel=data_travel)   
        return travel_bins, unique_velocity
    
    @cache_data
    def __calculate_progression(_self,  unique_travel : list, unique_velocity : list):
        progression_time = np.array([])
        for i in range(1, len(unique_travel)):
            tmp_percent = unique_travel[1:i]
            tmp_velocity = unique_velocity[1:i]
            if len(tmp_percent) > 0 and len(tmp_velocity) > 0 and 0 not in tmp_velocity[1:]:
                result = 1/tmp_velocity
                progression_time = np.append(progression_time, np.trapz(result, tmp_percent))
            else:
                progression_time = np.append(progression_time, np.nan)
        progression_time[0] = 0
        progression_time = np.insert(progression_time, 0, 0)
        return progression_time