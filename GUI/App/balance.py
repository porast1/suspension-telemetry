from bokeh.plotting import figure
import numpy as np
from bokeh.models import ColumnDataSource

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
def count_alfa(tgAlfa):
    degreeVariable = np.degrees(np.arctan(tgAlfa))
    radiansVariable = kat_w_radianach = np.arctan(tgAlfa)
    return degreeVariable, radiansVariable
def balance_Figure(alfaFront, alfaRear, travelDataFront, travelDataRear, velocityFront,  velocityRear,maxFrontTravel,maxRearTravel,  selected_series):
    count_Front = sum(1 for x in velocityFront)
    count_Rear = sum(1 for x in velocityRear)
    avgFront = abs(sum(x for x in velocityFront) / count_Front)
    avgRear = abs(sum(x for x in velocityRear) / count_Rear)
    
    frontAlfa, frontAlfaRadians = count_alfa(alfaFront)
    rearAlfa, rearAlfaRadians = count_alfa(alfaRear)
    # Tworzenie źródła danych dla tekstu
    text_source_front = ColumnDataSource(data=dict(x=[0], y=[avgFront], text=[f'AVG Front: {avgFront:.2f}']))
    text_source_rear = ColumnDataSource(data=dict(x=[0], y=[avgRear], text=[f'AVG Rear: {avgRear:.2f}']))
    text_source_front_alfa = ColumnDataSource(data=dict(x=[0], y=[1.5*max(avgFront,avgRear)], text=[f'Alfa Front: {frontAlfa:.2f}']))
    text_source_rear_alfa = ColumnDataSource(data=dict(x=[0], y=[1.4*max(avgFront,avgRear)], text=[f'Alfa Rear: {rearAlfa:.2f}']))
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
        p.line(x=travelDataFront, y=abs(velocityFront), line_width=2, legend_label='Front', line_color='blue')
        p.line(x=travelDataRear, y=abs(velocityRear), line_width=2, legend_label='Rear', line_color='red')
        p.segment(x0=[0,0], y0=[avgFront, avgRear], x1=[99,99], y1=[avgFront, avgRear], line_width=2, line_dash="dashed", line_color=["blue", "red"])

    # Dodawanie tekstu z informacją o średnich wartościach
        p.text(x='x', y='y', text='text', source=text_source_front, text_align="left", text_baseline="up", text_font_size="9pt")
        p.text(x='x', y='y', text='text', source=text_source_rear, text_align="left", text_baseline="up", text_font_size="9pt")
        p.text(x='x', y='y', text='text', source=text_source_front_alfa, text_align="left", text_baseline="bottom", text_font_size="9pt")
        p.text(x='x', y='y', text='text', source=text_source_rear_alfa, text_align="left", text_baseline="bottom", text_font_size="9pt")
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

        p.line(x=travelDataFrontPercent, y=abs(velocityFront), line_width=2, legend_label='Front', line_color='blue')
        p.line(x=travelDataRearPercent, y=abs(velocityRear), line_width=2, legend_label='Rear', line_color='red')
        p.segment(x0=[0,0], y0=[avgFront, avgRear], x1=[99,99], y1=[avgFront, avgRear], line_width=2, line_dash="dashed", line_color=["grey", "grey","grey", "grey"])

    # Dodawanie tekstu z informacją o średnich wartościach
        p.text(x='x', y='y', text='text', source=text_source_front, text_align="left", text_baseline="up", text_font_size="9pt")
        p.text(x='x', y='y', text='text', source=text_source_rear, text_align="left", text_baseline="up", text_font_size="9pt")
        p.text(x='x', y='y', text='text', source=text_source_front_alfa, text_align="left", text_baseline="bottom", text_font_size="9pt")
        p.text(x='x', y='y', text='text', source=text_source_rear_alfa, text_align="left", text_baseline="bottom",  text_font_size="9pt")

    return p
