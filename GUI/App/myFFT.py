from numpy.fft import fftfreq
from numpy.fft import fft
from numpy.fft import fftshift
from bokeh.plotting import figure
def my_fft_figure(data,fs):
    N = len(data)
    f = fftfreq(N,1/fs)
    f = fftshift(f)
    X = abs(fft(data))
    X = fftshift(X)
    p = figure(
        height = 250,
        x_range=(-10, 10),
        sizing_mode="stretch_both",
        x_axis_label="Freq [Hz]",
        y_axis_label='Amplitude',
        toolbar_location='above',
        tools='xpan,xwheel_zoom,reset, hover, save',
        active_drag='xpan',
        output_backend='webgl')
    p.circle(f, X, size=3)
    return p