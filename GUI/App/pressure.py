from bokeh.plotting import figure
import numpy as np
def minPressure(dataTravel,dataPressure):
    mask = (dataTravel <= 1)
    travelIdn = np.where(mask)[0]
    minDataPressure = np.mean(dataPressure[travelIdn])
    return minDataPressure
def pressure_figure(travelFront, travelRear, FrontPressure, RearPressure, selectedPressure):
    minFrontPress = minPressure(travelFront,FrontPressure)
    minRearPress = minPressure(travelRear,RearPressure)
    if "Pressure" in selectedPressure:
        p = figure(
        height = 350,
        min_border_left=70,
        min_border_right=50,
        sizing_mode="stretch_both",
        x_axis_label="Travel (%)",
        y_axis_label='Pressure[psi]',
        toolbar_location='above',
        tools='xpan,xwheel_zoom,box_zoom,reset, hover, save',
        active_drag='xpan',
        output_backend='webgl')
        p.line(x=travelFront, y=FrontPressure, line_width=2, legend_label='Front', line_color='blue')
        p.line(x=travelRear, y=RearPressure, line_width=2, legend_label='Rear', line_color='red')
    if "Progression" in selectedPressure:
        p = figure(
        height = 350,
        min_border_left=70,
        min_border_right=50,
        sizing_mode="stretch_both",
        x_axis_label="Travel (%)",
        y_axis_label='Progression [Amplitude]',
        toolbar_location='above',
        tools='xpan,xwheel_zoom,box_zoom,reset, hover, save',
        active_drag='xpan',
        output_backend='webgl')

        p.line(x=travelFront, y=FrontPressure/minFrontPress, line_width=2, legend_label='Front', line_color='blue')
        p.line(x=travelRear, y=RearPressure/minRearPress, line_width=2, legend_label='Rear', line_color='red')
    return p