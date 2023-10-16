import numpy as np 
import random
from bokeh.plotting import figure

def suspension_Progression_Data(percentTravel, velocity):
    n = len(percentTravel)
    iterationpercentTravel = range(0,100,1)
    percentTravel = np.array(percentTravel)
    velocity = np.array(velocity)
    velocity = np.insert(velocity, 0, 0)
    avgVelocity = np.empty(0)
    index = np.empty(0)
    for i in range(len(iterationpercentTravel)):
        if iterationpercentTravel[i] >= 99:
            mask = (percentTravel >= iterationpercentTravel[i])
            index = np.where(mask)[0]
        else:
            mask = (percentTravel >= iterationpercentTravel[i]) & (percentTravel < iterationpercentTravel[i+1])
            index = np.where(mask)[0]
        for ind in index:
            if velocity[ind] < 0:
                index.remove(ind)
            else:
                continue
        np.append(avgVelocity, np.mean(velocity[index]))
    return iterationpercentTravel, avgVelocity

    
def travel_progression_figure(selected_series,timeOrVelocityFront, timeOrVelocityRear, travelDataPercent,maxFrontTravel,maxRearTravel):

    if selected_series == "Travel mm":
        p = figure(
            min_border_left=70,
            min_border_right=50,
            sizing_mode="stretch_both",
            x_axis_label="Travel (mm)",
            y_axis_label='Time(ms)',
            toolbar_location='above',
            tools='xpan,xwheel_zoom,box_zoom,reset, hover, save',
            active_drag='xpan',
            output_backend='webgl')
        p.line(x=travelDataPercent*maxFrontTravel, y=timeOrVelocityFront, line_width=2, legend_label='Front', line_color='blue')
        p.line(x=travelDataPercent*maxRearTravel, y=timeOrVelocityRear, line_width=2, legend_label='Rear', line_color='red')
    else:
        p = figure(
            min_border_left=70,
            min_border_right=50,
            sizing_mode="stretch_both",
            x_axis_label="Travel (%)",
            y_axis_label='Time(ms)',
            toolbar_location='above',
            tools='xpan,xwheel_zoom,box_zoom,reset, hover, save',
            active_drag='xpan',
            output_backend='webgl')
        p.line(x=travelDataPercent, y=timeOrVelocityFront, line_width=2, legend_label='Front', line_color='blue')
        p.line(x=travelDataPercent, y=timeOrVelocityRear, line_width=2, legend_label='Rear', line_color='red')
    return p
    