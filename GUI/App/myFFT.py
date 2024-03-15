from numpy.fft import fft, fftfreq, fftshift
from bokeh.plotting import Figure
from bokeh.models import Range1d
from sessionInit import BaseDataAndFigure
from streamlit import subheader
class fftFigure(BaseDataAndFigure):
    def __init__(self, dataFrom = 'Front'):
        super().__init__()
        subheader(f"{dataFrom} FFT")
        if 'Front' == dataFrom:
            self.__frequency, self.__FFT = self.__calculateFFT(self.getFrontTravel())
        else:
            self.__frequency, self.__FFT = self.__calculateFFT(self.getRearTravel())
    def __calculateFFT(self, data : list):
        time = self.getTimeTravelPressure()[:2]
        fs = 1/(time[1] - time[0])
        N = len(data)
        f = fftfreq(N,1/fs)
        f = fftshift(f)
        X = abs(fft(data))
        X = fftshift(X)
        return f, X
    def power_spectrum(self, data):
        # Apply Fourier Transform
        fft_result = fft(data)
        
        # Compute power spectrum: the square of the absolute value of the fft result
        power_spectrum = abs(fft_result) ** 2
        
        # Compute frequencies for the spectrum
        sample_rate = 1  # adjust as needed
        freqs = fftfreq(len(data), 1/sample_rate)
    
        return freqs, power_spectrum
    def XY_LabelAbstract(self, abstractFigure: Figure):
        abstractFigure.height = 250
        abstractFigure.xaxis.axis_label = 'Frequency [Hz]'
        abstractFigure.yaxis.axis_label = 'Amplitude'
        x_range = Range1d(start=-10, end=10)
        abstractFigure.x_range = x_range
        return abstractFigure
    def simpleFigureAbstract(self, abstractFigure : Figure):
        abstractFigure.circle(self.__frequency, self.__FFT, size=3)
        return abstractFigure