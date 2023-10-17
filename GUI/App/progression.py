import numpy as np 
import random
from bokeh.plotting import figure

def suspension_Progression_Data(percentTravel, velocity):
    n = len(percentTravel)
    iterationpercentTravel = range(0,101,1)
    percentTravel = np.array(percentTravel)
    velocity = np.array(velocity)
    avgVelocity = np.empty(0)
    index = np.empty(0)
    mask = (velocity > 0)
    indexVelocity = np.where(mask)[0]
    for i in range(len(iterationpercentTravel) - 1):
        newIndex = np.empty(0)
        if iterationpercentTravel[i] >= 100:
            mask = (percentTravel >= iterationpercentTravel[i])
        else:
            mask = (percentTravel >= iterationpercentTravel[i]) & (percentTravel < iterationpercentTravel[i+1])
        index = np.where(mask)[0]
        newIndex = np.intersect1d(indexVelocity, index)
        if (len(newIndex) > 0):
            avgVelocity = np.append(avgVelocity, np.mean(velocity[newIndex]))
        else:
            avgVelocity = np.append(avgVelocity, 0)

    return iterationpercentTravel, avgVelocity

    
def travel_progression_figure(selected_series,timeOrVelocityFront, timeOrVelocityRear, travelDataPercent,maxFrontTravel,maxRearTravel):

    if selected_series == "Travel mm":
        p = figure(
            height = 350,
            min_border_left=70,
            min_border_right=50,
            sizing_mode="stretch_both",
            x_axis_label="Travel (mm)",
            y_axis_label='Time(ms)',
            toolbar_location='above',
            tools='xpan,xwheel_zoom,box_zoom,reset, hover, save',
            active_drag='xpan',
            output_backend='webgl')
        travelDataFront = [percent * maxFrontTravel / 100 for percent in travelDataPercent]
        travelDataRear = [percent * maxRearTravel / 100 for percent in travelDataPercent]
        p.line(x=travelDataFront, y=timeOrVelocityFront, line_width=2, legend_label='Front', line_color='blue')
        p.line(x=travelDataRear, y=timeOrVelocityRear, line_width=2, legend_label='Rear', line_color='red')
    else:
        p = figure(
            height = 350,
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
    

def progressionDataConvert(dataAvgVelocity,percerntTravel):

    tmpPercent = []
    tmpVelocity = []
    results = []
    
    for percent in percerntTravel:
        tmpPercent = percerntTravel[0:percent]
        tmpVelocity = dataAvgVelocity[0:percent]
        if len(tmpPercent) > 0 and len(tmpVelocity) > 0:
            result = np.trapz(tmpVelocity, tmpPercent)
            results.append(result)
        else: 
            continue
    return results