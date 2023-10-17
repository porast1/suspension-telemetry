import numpy as np
from bokeh.palettes import Spectral11
from bokeh.plotting import figure
from bokeh.models import ColumnDataSource
from scipy.stats import norm


from bokeh.models.annotations import ColorBar
from bokeh.models.tickers import FixedTicker
from bokeh.models.mappers import LinearColorMapper
def _normal_distribution_data(velocity: list[float],
                              step: float):

    stroke_velocity = np.array(velocity)
    mu, std = norm.fit(stroke_velocity)
    ny = np.linspace(stroke_velocity.min(), stroke_velocity.max(), 100)
    pdf = norm.pdf(ny, mu, std) * step * 100
    return pdf, ny

#Travel ony convert in %
def velocity_histogram_figure(travel, velocity, step):
    n = len(travel)
    iterationTravel = range(0,100,10)
    travel_dict = {key: 0 for key in iterationTravel}
    iterationVelocity = range(-20*step,20*step,step)
    travel = np.array(travel)
    velocity = np.array(velocity)


    for i in range(len(iterationTravel)):
        if iterationTravel[i] >= 90:
            mask = (travel >= iterationTravel[i])
            travel_dict[iterationTravel[i]] = np.where(mask)[0]
        else:
            mask = (travel >= iterationTravel[i]) & (travel < iterationTravel[i+1])
            travel_dict[iterationTravel[i]] = np.where(mask)[0]
    for key, values in travel_dict.items():
        #interacja pokluczu
        key_val = []
        tmp_tab = np.array([], dtype=float)
        for value in values:
            tmp_tab = np.append(tmp_tab,velocity[value])
        for i in range(len(iterationVelocity)):
            if iterationVelocity[i] >= iterationVelocity[-1]:
                mask = (tmp_tab >= iterationVelocity[i])
                key_val.append(len(np.where(mask)[0]))
            else:
                mask = (tmp_tab>= iterationVelocity[i]) & (tmp_tab < iterationVelocity[i+1])
                key_val.append(len(np.where(mask)[0]))
        travel_dict[key] = None 
        tmp_tab = None
        travel_dict[key] = [100 * x / n for x in key_val]
    palette = Spectral11[1:]
    travel_dict = {str(key): travel_dict[key] for key in travel_dict}
    data = {
        'velocity': iterationVelocity,
    }
    data = {**data, **travel_dict}

    # Tworzenie ColumnDataSource z danymi
    source = ColumnDataSource(data)

    p = figure(x_range=(min(iterationVelocity), max(iterationVelocity)), 
                title="velocity chart",
                height = 300,
                min_border_left=70,
                min_border_right=50,
                sizing_mode="stretch_both",
                x_axis_label="Speed mm/s",
                y_axis_label="Time (%)",
                toolbar_location='above',
                tools='pan,wheel_zoom,reset,hover',
                active_drag='pan',
                output_backend='webgl')
        
    p.vbar_stack(list(travel_dict.keys()), x='velocity', width=(max(iterationVelocity)-min(iterationVelocity))/(len(iterationVelocity)), color=palette, source=source)
    p.y_range.start = 0
    p.xgrid.grid_line_color = None
    p.axis.minor_tick_line_color = None
    p.outline_line_color = None
    p.legend.location = "top_left"
    p.legend.orientation = "horizontal"
    mapper = LinearColorMapper(palette=palette, low=0, high=100)
    color_bar = ColorBar(
        color_mapper=mapper,
        height=16,
        width=100,
        location='top_left',
        title="Travel (%)",
        ticker=FixedTicker(ticks=[0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100])
    )
    p.add_layout(color_bar, 'above')
    #Gauss PDF
    maxVelocityComp = max(velocity)
    maxVelocityReb = min(velocity)

    mu, std = norm.fit(velocity)
    if (maxVelocityReb > min(iterationVelocity)) or (maxVelocityComp < max(iterationVelocity)):
        ny = np.linspace(min(iterationVelocity), max(iterationVelocity), 100)
    else:
        ny = np.linspace(maxVelocityReb, maxVelocityComp, 100)
    pdf = norm.pdf(ny, mu, std) * step * 100

    source_normal = ColumnDataSource(
        name='ds_normal',
        data={'pdf': pdf.tolist(), 'ny': ny.tolist()}
    )

    p.line(x='ny', y='pdf', line_width=2, source=source_normal, line_dash='dashed', color=Spectral11[-2])

    count_comp = sum(1 for x in velocity if x > 0)
    count_reb = sum(1 for x in velocity if x < 0)
    avgComp = sum(x for x in velocity if x > 0) / count_comp
    avgReb = sum(x for x in velocity if x < 0) / count_reb

    # Tworzenie źródła danych dla tekstu
    text_source_comp = ColumnDataSource(data=dict(x=[avgComp], y=[max(pdf)/2], text=[f'AVG Comp: {avgComp:.2f}']))
    text_source_reb = ColumnDataSource(data=dict(x=[avgReb], y=[max(pdf)/2], text=[f'AVG Rebound: {avgReb:.2f}']))

    if maxVelocityComp < iterationVelocity[-1]:
        text_source_MaxComp = ColumnDataSource(data=dict(x=[maxVelocityComp], y=[max(pdf)/2], text=[f'MAX Comp: {maxVelocityComp:.2f}']))
    else:
        text_source_MaxComp = ColumnDataSource(data=dict(x=[iterationVelocity[-2]], y=[max(pdf)/2], text=[f'AVG Comp: {avgComp:.2f}']))

    if maxVelocityReb > iterationVelocity[0]:
        text_source_MaxReb = ColumnDataSource(data=dict(x=[maxVelocityReb], y=[max(pdf)/2], text=[f'MAX Reb: {maxVelocityReb:.2f}']))
    else:
        text_source_MaxReb = ColumnDataSource(data=dict(x=[iterationVelocity[1]], y=[max(pdf)/2], text=[f'Max Reb: {maxVelocityReb:.2f}']))
    # Dodawanie linii przerywanych z informacją o średnich wartościach
    p.segment(x0=[avgComp, avgReb, maxVelocityComp,maxVelocityReb], y0=[0, 0, 0, 0], x1=[avgComp, avgReb, maxVelocityComp,maxVelocityReb], y1=[max(pdf), max(pdf),max(pdf), max(pdf)], line_width=2, line_dash="dashed", line_color=["grey", "grey","grey", "grey"])

    # Dodawanie tekstu z informacją o średnich wartościach
    p.text(x='x', y='y', text='text', source=text_source_comp, text_align="left",angle=1.5708, text_baseline="top", text_font_size="6pt")
    p.text(x='x', y='y', text='text', source=text_source_reb, text_align="left",angle=1.5708, text_baseline="bottom", text_font_size="6pt")
    p.text(x='x', y='y', text='text', source=text_source_MaxComp, text_align="left",angle=1.5708, text_baseline="top", text_font_size="6pt")
    p.text(x='x', y='y', text='text', source=text_source_MaxReb, text_align="left",angle=1.5708, text_baseline="bottom", text_font_size="6pt")
    return p

