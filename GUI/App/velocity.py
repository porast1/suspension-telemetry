import numpy as np
from bokeh.palettes import Spectral11
from bokeh.plotting import Figure, figure
from bokeh.models import ColumnDataSource
from scipy.stats import norm


from bokeh.models.annotations import ColorBar
from bokeh.models.tickers import FixedTicker
from bokeh.models.mappers import LinearColorMapper
from sessionInit import BaseFigure
from numpy import histogram
from bokeh.models.ranges import Range1d
class VelocityHistogram(BaseFigure):
    def __init__(self, dataFrom = 'Front', step = 50):
        super().__init__()
        self.__step = step
        self.__dataFrom = dataFrom
        if 'Front' == self.__dataFrom:
            travel = self.getFrontTravelPercent()
            velocity = self.getFrontVelocity()
            self.__figureColumnDataSource = self.__calculateHistogramData( travel, velocity)
            self.__pdf, self.__ny = self._normal_distribution_data(velocity, self.__step)
        else:
            travel = self.getRearTravelPercent()
            velocity = self.getRearVelocity()
            self.__figureColumnDataSource = self.__calculateHistogramData( travel, velocity)
            self.__pdf, self.__ny = self._normal_distribution_data(velocity, self.__step)

    def _normal_distribution_data(self, velocity: list[float], step: float):
        mu, std = norm.fit(velocity)
        ny = np.linspace(-2500, 2500, 100)
        pdf = norm.pdf(ny, mu, std) * 100 * 100
        return pdf, ny

    def __calculateHistogramData(self, travel, velocity):
        lenData = len(travel)
        histTravel, edgeTravel = histogram(travel, 11, range=(0, 110))
        data = {'velocities': None}
        for edgeIndex in range(len(edgeTravel) - 1):
            if edgeIndex == (len(edgeTravel) - 1):
                mask = (travel >= edgeTravel[edgeIndex]) & (travel <= edgeTravel[edgeIndex + 1])
            else:
                mask = (travel >= edgeTravel[edgeIndex]) & (travel < edgeTravel[edgeIndex + 1])
            tmpVelocity = velocity[mask]
            histVel, edgeVel = histogram(tmpVelocity, self.__step, range=(-2500, 2500))
            data[str(int(edgeTravel[edgeIndex]))] = 100 * histVel/lenData  

        offset = (edgeVel[1] - edgeVel[0]) / 2
        data['velocities'] = edgeVel[:-1] + offset
        return data

    def label_XY(self, abstractFigure: Figure):
        x_range = Range1d(min(self.__figureColumnDataSource['velocities']), max(self.__figureColumnDataSource['velocities']))
        abstractFigure.xaxis.axis_label = 'Velocity [mm/s]'
        abstractFigure.yaxis.axis_label = 'Time [%]'
        abstractFigure.x_range = x_range
        abstractFigure.y_range.start = 0
        abstractFigure.x_range.start -= 0.05 * (abstractFigure.x_range.end - abstractFigure.x_range.start)  # Add padding to the start
        abstractFigure.x_range.end += 0.05 * (abstractFigure.x_range.end - abstractFigure.x_range.start)    # Add padding to the end

        abstractFigure.xgrid.grid_line_color = None
        abstractFigure.axis.minor_tick_line_color = None
        abstractFigure.outline_line_color = None
        mapper = LinearColorMapper(palette=Spectral11, low=0, high=110)
        color_bar = ColorBar(
        color_mapper=mapper,
        height=16,
        width=100,
        location='top_left',
        title="Travel (%)",
        ticker=FixedTicker(ticks=list(range(0, 110, 10)))
        )
        abstractFigure.add_layout(color_bar, 'above')
        return abstractFigure

    def figure_data(self, abstractFigure: Figure):
        Travel = [str(element) for element in range(0, 110, 10)]
        bar_width = 0.9 * (self.__figureColumnDataSource['velocities'][1] - self.__figureColumnDataSource['velocities'][0])
        source = self.__figureColumnDataSource
        keys = [str(element) for element in range(0, 110, 10)]  # Klucze do przeszukania

        
        abstractFigure.vbar_stack( Travel , x='velocities', width=bar_width, color=Spectral11, source=source)
        abstractFigure.line(x=self.__ny, y=self.__pdf, line_width=2,  line_dash='dashed', color=Spectral11[-2])

        # Tworzenie źródła danych dla tekstu
        if 'Front' == self.__dataFrom:
            avgComp, maxVelocityComp, avgReb, maxVelocityReb = self.calculate_avg_max_velocity(self.getFrontVelocity())
        else:
            avgComp, maxVelocityComp, avgReb, maxVelocityReb = self.calculate_avg_max_velocity(self.getRearVelocity())
        
        text_source_comp = ColumnDataSource(data=dict(x=[avgComp], y=[max(self.__pdf)], text=[f'AVG Comp: {avgComp:.2f}']))
        text_source_reb = ColumnDataSource(data=dict(x=[avgReb], y=[max(self.__pdf)], text=[f'AVG Rebound: {avgReb:.2f}']))
        text_source_MaxComp = ColumnDataSource(data=dict(x=[max(source['velocities']) - 200], y=[max(self.__pdf)], text=[f'MAX Comp: {maxVelocityComp:.2f}']))
        text_source_MaxReb = ColumnDataSource(data=dict(x=[min(source['velocities']) + 200], y=[max(self.__pdf)], text=[f'MAX Reb: {maxVelocityReb:.2f}']))

        # Dodawanie linii przerywanych z informacją o średnich wartościach
        y1_values = [max(self.__pdf) for _ in range(4)]
        abstractFigure.segment(x0=[avgComp, avgReb, maxVelocityComp, maxVelocityReb], y0=[0, 0, 0, 0], x1=[avgComp, avgReb, maxVelocityComp, maxVelocityReb], y1=y1_values, line_width=2, line_dash="dashed", line_color=["grey", "grey", "grey", "grey"])

        # Dodawanie tekstu z informacją o średnich wartościach
        abstractFigure.text(x='x', y='y', text='text', source=text_source_comp, text_align="left",angle=1.5708, text_baseline="top", text_font_size="6pt")
        abstractFigure.text(x='x', y='y', text='text', source=text_source_reb, text_align="left",angle=1.5708, text_baseline="bottom", text_font_size="6pt")
        abstractFigure.text(x='x', y='y', text='text', source=text_source_MaxComp, text_align="left",angle=1.5708, text_baseline="top", text_font_size="6pt")
        abstractFigure.text(x='x', y='y', text='text', source=text_source_MaxReb, text_align="left",angle=1.5708, text_baseline="bottom", text_font_size="6pt")
        return abstractFigure
           
    
    


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
    width = (max(iterationVelocity)-min(iterationVelocity))/(len(iterationVelocity))
    p.line(x=self.__ny, y='pdf', line_width=2, source=source_normal, line_dash='dashed', color=Spectral11[-2])
    p.vbar_stack(list(travel_dict.keys()), x='velocity', width=width, color=palette, source=source)
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
        text_source_MaxComp = ColumnDataSource(data=dict(x=[iterationVelocity[-2]], y=[max(pdf)/2], text=[f'MAX Comp: {maxVelocityComp:.2f}']))

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

