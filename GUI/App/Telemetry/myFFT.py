from numpy.fft import fft, fftfreq, fftshift
from bokeh.plotting import Figure

from myApi.ApiFigure import BokehFigure

class fftFigure(BokehFigure):
    def __init__(self, data_travel : list, data_time : list):
        super().__init__()
        data_travel = data_travel[:]
        self.__XY_label_source = self.template_XY_label_source()
        self.__data_source = self.template_data_source()

        frequency, travel_FFT = self.__calculateFFT(data_travel=data_travel, data_time=data_time)
        
        self.__XY_label_source['x'] = 'Frequency [Hz]'
        self.__XY_label_source['y'] = 'Amplitude'
        self.__XY_label_source['x_range_start'] = -10
        self.__XY_label_source['x_range_end'] = 10

        self.__data_source['x'] = frequency
        self.__data_source['y'] = travel_FFT


    def create_figure(self, figure_instance : Figure):
        figure_instance = super().create_figure(figure_instance=figure_instance)
        XY_label_source = self.__XY_label_source
        data_source = self.__data_source
        self._configure_axes(figure_instance=figure_instance, label_source=XY_label_source)
        self._figure_circle(figure_instance=figure_instance, data_source=data_source)
        return figure_instance
    
    def __calculateFFT(self, data_travel : list, data_time : list):
        time = data_time[:2]
        fs = 1/(time[1] - time[0])
        N = len(data_travel)
        f = fftfreq(N,1/fs)
        f = fftshift(f)
        X = abs(fft(data_travel))
        X = fftshift(X)
        return f, X
    