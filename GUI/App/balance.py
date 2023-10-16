from bokeh.plotting import figure
import numpy as np

def balance_data(velocity, travel, char: str):
    reboundSpeedsIndex = []
    compressionSpeedsIndex = []
    maskCompression = (velocity >= 0)
    compressionSpeedsIndex = np.where(maskCompression)
    maskRebound = (velocity < 0)
    reboundSpeedsIndex = np.where(maskRebound)
    velocityCompression = velocity[compressionSpeedsIndex]
    travelCompression = travel[compressionSpeedsIndex]
    velocityRebound = velocity[reboundSpeedsIndex]
    travelRebound = travel[reboundSpeedsIndex]
    a, b = 0, 0
    travelRegression = 0
    if char == "Comp":
        a, b = np.polyfit(travelCompression, velocityCompression,1)
        travelRegression = travelCompression
    else:
        a, b = np.polyfit(travelRebound, velocityRebound,1)
        travelRegression = travelRebound
    return a, b, travelRegression

def balance_Figure(travelDataFront, travelDataRear, velocityFront,  velocityRear,maxFrontTravel,maxRearTravel,  selected_series):
    if selected_series == "Travel mm":
        p = figure(
            height = 350,
            min_border_left=70,
            min_border_right=50,
            sizing_mode="stretch_both",
            x_axis_label="Travel (mm)",
            y_axis_label='Velocity mm/s',
            toolbar_location='above',
            tools='xpan,xwheel_zoom,box_zoom,reset, hover, save',
            active_drag='xpan',
            output_backend='webgl')

        p.line(x=travelDataFront, y=velocityFront, line_width=2, legend_label='Front', line_color='blue')
        p.line(x=travelDataRear, y=velocityRear, line_width=2, legend_label='Rear', line_color='red')
    else:
        p = figure(
            height = 350,
            min_border_left=70,
            min_border_right=50,
            sizing_mode="stretch_both",
            x_axis_label="Travel (%)",
            y_axis_label='velocity mm/s',
            toolbar_location='above',
            tools='xpan,xwheel_zoom,box_zoom,reset, hover, save',
            active_drag='xpan',
            output_backend='webgl')
        travelDataFrontPercent = [value/maxFrontTravel * 100 for value in travelDataFront]
        travelDataRearPercent = [value/ maxRearTravel * 100 for value in travelDataRear]
        p.line(x=travelDataFrontPercent, y=velocityFront, line_width=2, legend_label='Front', line_color='blue')
        p.line(x=travelDataRearPercent, y=velocityRear, line_width=2, legend_label='Rear', line_color='red')
    return p
