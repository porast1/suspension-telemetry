import numpy as np
import math as m
from bokeh.plotting import figure
from bokeh.models import ColumnDataSource
import statistics
def travel_figure(selected_series, frontTravelData,rearTravelData, time):
    p = figure(
    height = 350,
    min_border_left=70,
    min_border_right=50,
    sizing_mode="stretch_both",
    x_axis_label="Time (%)",
    y_axis_label='Travel (mm)',
    toolbar_location='above',
    tools='xpan,xwheel_zoom,box_zoom,reset, hover, save',
    active_drag='xpan',
    output_backend='webgl')

    if "Front" in selected_series:
        p.line(x=time, y=frontTravelData, line_width=2, legend_label='Front', line_color='blue')
    if "Rear" in selected_series:
        p.line(x=time, y=rearTravelData, line_width=2, legend_label='Rear', line_color='red')
    return p

def travel_histogram_data(signal, maxStroke):
    deltaBin = int(maxStroke * 0.05)
    bins = list(range(0, maxStroke - 1,int(deltaBin)))
    hist = np.zeros(len(bins))
    total_count = len(signal)
    for value in signal:
        for i in range(len(bins) - 1):
            if value >= bins[-1]:
                hist[-1] +=1
            elif bins[i] <= value <= bins[i + 1]:  # Poprawiony warunek
                hist[i] += 1
    hist = hist / total_count * 100.0
    return hist, bins


def travel_histogram_figure(data, maxTravel):
    hist, bins = travel_histogram_data(data,maxTravel)
    average = round(sum(data) / len(data), 2)
    max_data = round(max(data), 2)

    # Tworzenie źródła danych dla tekstu
    text_source = ColumnDataSource(data=dict(x=[max_data, average], y=[np.max(hist), np.max(hist)], text=[f'Max: {max_data}', f'Avg: {average}']))

    p = figure(
        height=300,
        min_border_left=70,
        min_border_right=50,
        x_range=(-0.025*maxTravel, 1.2 * np.max(maxTravel)),
        sizing_mode="stretch_both",
        x_axis_label="Travel (mm)",
        y_axis_label='Time (%)',
        toolbar_location='above',
        tools='xpan,xwheel_zoom,reset, hover, save',
        active_drag='xpan',
        output_backend='webgl'
    )
    p.vbar(x=bins,top = hist, bottom = 0, width=maxTravel / (len(bins) - 1), line_width=2, fill_alpha=0.4)

    # Zaznaczanie wartości maksymalnej i średniej wartości
    p.segment(x0=[max_data, average], y0=[0,0], x1=[max_data, average], y1=[np.max(hist), np.max(hist)], line_width=2, line_dash="dashed", line_color=["red", "green"])

    # Wyświetlanie tekstu z wartościami
    p.text(x='x', y='y', text='text', source=text_source, text_align="left", text_baseline="middle")
    return p

def calculate_derivative(samples, dt):
    # Oblicz różnice między kolejnymi próbkami
    differences = np.diff(samples)
    
    # Oblicz pochodną, dzieląc różnice przez odstęp czasu
    derivative = differences / dt

    derivative = np.insert(derivative, 0, 0)
    
    return derivative


