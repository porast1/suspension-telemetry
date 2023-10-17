import numpy as np
import math as m
from bokeh.plotting import figure
from bokeh.models import ColumnDataSource
def travel_figure(selected_series, frontTravelData,rearTravelData, time):
    p = figure(
    height = 300,
    min_border_left=70,
    min_border_right=50,
    sizing_mode="stretch_both",
    x_axis_label="Time (s)",
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

def travel_histogram_data(travel,maxTravel,char:str):

    n = len(travel)
    iterationTravel = range(0,100,5)
    travel = np.array(travel)
    hist = np.empty(0)
    for i in range(len(iterationTravel)):
        if iterationTravel[i] >= 95:
            mask = (travel >= iterationTravel[i])
        else:
            mask = (travel >= iterationTravel[i]) & (travel < iterationTravel[i+1])
        hist = np.append(hist, len(np.where(mask)[0])/n)
    if (char == "Travel mm"):
        iterationTravel = [x * maxTravel / 100 for x in iterationTravel]  # Pomnożenie elementów listy przez maxTravel/100
    return hist, iterationTravel


def travel_histogram_figure(data, maxTravel, char: str):
    hist, bins = travel_histogram_data(data, maxTravel, char)
    average = round(sum(data) / len(data), 2)
    max_data = round(max(data), 2)

    # Tworzenie źródła danych dla tekstu
    text_source = ColumnDataSource(data=dict(x=[max_data, average], y=[np.max(hist)/2, np.max(hist)/2], text=[f'Max: {max_data}', f'Avg: {average}']))

    if (char == "Travel mm"):
        p = figure(
            height=300,
            min_border_left=70,
            min_border_right=50,
            x_range=(-0.05*max(data), 1.05 * np.max(data)),
            sizing_mode="stretch_both",
            x_axis_label="Travel (mm)",
            y_axis_label='Time (%)',
            toolbar_location='above',
            tools='xpan,xwheel_zoom,reset, hover, save',
            active_drag='xpan',
            output_backend='webgl'
        )
    else:
        p = figure(
            height=300,
            min_border_left=70,
            min_border_right=50,
            x_range=(-0.05*max(data), 1.05 * np.max(data)),
            sizing_mode="stretch_both",
            x_axis_label="Travel (%)",
            y_axis_label='Time (%)',
            toolbar_location='above',
            tools='xpan,xwheel_zoom,reset, hover, save',
            active_drag='xpan',
            output_backend='webgl'
        )

    p.vbar(x=bins,top = hist, bottom = 0, width=max(data) / (len(bins) - 1), line_width=2, fill_alpha=0.4)

    # Zaznaczanie wartości maksymalnej i średniej wartości
    p.segment(x0=[max_data, average], y0=[0,0], x1=[max_data, average], y1=[np.max(hist), np.max(hist)], line_width=2, line_dash="dashed", line_color=["red", "green"])

    # Wyświetlanie tekstu z wartościami
    p.text(x='x', y='y', text='text', source=text_source, angle=1.5708, text_baseline="bottom",text_font_size="9pt")
    return p

def calculate_derivative(samples, dt):
    # Oblicz różnice między kolejnymi próbkami
    differences = np.diff(samples)
    
    # Oblicz pochodną, dzieląc różnice przez odstęp czasu
    derivative = differences / dt

    derivative = np.insert(derivative, 0, 0)
    
    return derivative


def maxTravel(data, n):
    top_n_max_indices = np.argpartition(data, -n)[-n:]  # Znajdź indeksy n największych wartości
    top_n_max_values = data[top_n_max_indices]  # Wybierz wartości na podstawie indeksów

    average_of_top_n_max_values = np.mean(top_n_max_values)
    return average_of_top_n_max_values