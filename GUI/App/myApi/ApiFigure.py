
from numpy import mean, ndarray, degrees, arctan, polyfit, poly1d, arange, max, abs
from pandas import DataFrame
from bokeh.plotting import figure, Figure
from bokeh.models import ColumnDataSource, Range1d, Label
from bokeh.palettes import Spectral11
from bokeh.models.tickers import FixedTicker
from bokeh.models.mappers import LinearColorMapper
from bokeh.models.annotations import Span, ColorBar
from abc import ABC, abstractmethod
from typing import Any
from copy import deepcopy


class BaseCalculateMethods:
    def __init__(self):
        pass
    def avg_data(self, data: ndarray):
        return mean(data)

    def max_data(self, data: ndarray):
        return max(data)
    
    def min_data(self, data: ndarray):
        return min(data)
    
    def comp_velocity(self, velocity: ndarray):
        mask = velocity >= 0
        return velocity[mask]
    
    def reb_velocity(self, velocity: ndarray):
        mask = velocity < 0
        return velocity[mask]
    
    def comp_velocity_with_travel(self, travel: ndarray, velocity: ndarray):
        mask = velocity >= 0
        return travel[mask], velocity[mask]
    
    def reb_velocity_with_travel(self, travel: ndarray, velocity: ndarray):
        mask = velocity < 0
        return travel[mask], abs(velocity[mask])
    
    def linear_fit_coefficients(self, x_data : ndarray, y_data : ndarray):
        coefficients  = polyfit(x_data, y_data,1)
        return coefficients
    
    def calculate_linear_fit(self, x_data: ndarray, y_data: ndarray):
        coefficients = self.linear_fit_coefficients(x_data=x_data, y_data=y_data)
        travel_fit = arange(0, 101, 1)
        velocity_fit = poly1d(coefficients)
        linear_velocity = velocity_fit(travel_fit)
        return travel_fit, linear_velocity
    
    def count_alfa(self, x_data: ndarray, y_data: ndarray):
        coefficients = self.linear_fit_coefficients(x_data=x_data, y_data=y_data)
        degree = degrees(arctan(coefficients[0]))
        return degree

    def calculate_bar_width(self, data_source : dict):
        
        bar_width = 0.9 * (data_source['x'][1] - data_source['x'][0])
        return bar_width
    
    

class BaseFigure(ABC, BaseCalculateMethods):
    def __init__(self):
        super().__init__()
        self.__figure_data_source = {
            'x' : [],
            'y' : [],
        }
        self.__figure_text_source = {
            'x' : 0,
            'y' : 0,
            'x_offset' : 0,
            'y_offset' : 0,
            'text' : None,
            'text_align' : "center",
            'text_baseline' : "bottom",
            'angle' : 1.5708,
            'bar_width' : None,
            'legend_visible' : True,    
            'legend_label' : None,
            'legend_location' : 'top_right',
            'line_dash' : 'solid',
            'color' : None
        }
        self.__color_bar_source = {
            'color_mapper' : None,
            'width' : 100,
            'height' : 16,
            'location' : 'top_left',
            'title' : None,
            'ticker' : None
        }
        self.__XY_label_source = {
            'x' : None,
            'y' : None,
            'x_range_start' : None,
            'x_range_end' : None
        }
    
    
    @abstractmethod
    def create_figure(self, figure_instance):
        figure_instance = figure_instance or self._create_empty_plot()
        # Pass the data_source argument from the class instance
        # Pass the text_source argument from the class instance
        # return self.prepare_figure(figure_instance, data_source, text_source)
        return figure_instance

    def template_data_source(self):
        figure_data_source = deepcopy(self.__figure_data_source)
        return figure_data_source
    
    def template_text_source(self, legend_label : str):
        figure_text_source = deepcopy(self.__figure_text_source)
        figure_text_source['legend_label'] = legend_label
        if legend_label == "Front":
            figure_text_source['color'] = "blue"
        else:
            figure_text_source['color'] = "red"
        return figure_text_source
    
    def template_color_bar_source(self):
        color_bar_source = deepcopy(self.__color_bar_source)
        return color_bar_source
    
    def template_XY_label_source(self):
        XY_label_source = deepcopy(self.__XY_label_source)
        return XY_label_source
    
    def _configure_axes(self, figure_instance : Any, label_source : dict):
        return self._axis_range(*self._axis_label(figure_instance, label_source))[0]

    
    
    def _figure_text(self, figure_instance : Any, text_source : dict):
        if text_source['x']:
            text_source['y_offset'] = figure_instance.plot_height / 3
        elif text_source['y']:
            text_source['x_offset'] = figure_instance.plot_width / 2
        else:
            text_source['x_offset'] = figure_instance.plot_width / 10
            text_source['y_offset'] = figure_instance.plot_height / 2

        if 'Front' == text_source['legend_label']:
            text_source['text_baseline'] = 'top'
        return self._dislpay_text_in_figure(figure_instance, text_source)
    
    def fit_text_refline_coordinates(self, label_source, coordinate):
        new_coordinate = 0
        start_coordinate = label_source['x_range_start']
        end_coordinate = label_source['x_range_end']
        if coordinate < 0:
            if coordinate < start_coordinate:
                new_coordinate = start_coordinate + (0.1 * abs(start_coordinate))
            else:
                new_coordinate = coordinate
        else:
            if coordinate > end_coordinate:
                new_coordinate = end_coordinate - (0.1 * end_coordinate)
            else:
                new_coordinate = coordinate
        return new_coordinate
    
    @abstractmethod
    def _create_empty_plot(self):
        pass

    @abstractmethod
    def _create_data_frame(self, data : dict):
        return DataFrame(data)
    
    @abstractmethod
    def _axis_label(self, figure_instance : Any, label_source : dict):
        return figure_instance, label_source
    
    @abstractmethod
    def _axis_range(self, figure_instance : Any, label_source : dict):
        return figure_instance, label_source
    
    @abstractmethod
    def _legend_visible(self, figure_instance : Any, text_source : dict):
        return figure_instance
    
    @abstractmethod
    def _legend_location(self, figure_instance : Any, text_source : dict):
        return figure_instance
    
    @abstractmethod
    def _figure_vbar(self, figure_instance : Any, data_source : dict, text_source : dict):
        source = self._create_data_frame(data_source)
        bar_width = self.calculate_bar_width(data_source)
        return figure_instance
    @abstractmethod
    def _figure_stack_vbar(self, figure_instance : Any, data_source : dict, text_source : dict):
        pass

    @abstractmethod
    def _color_bar(self, figure_instance : Any, text_source : dict):
        pass
    
    @abstractmethod
    def _figure_line(self, figure_instance : Any, data_source : dict, text_source : dict):
        source = self._create_data_frame(data_source)
        return figure_instance
    
    @abstractmethod
    def _figure_circle(self, figure_instance : Any, data_source : dict):
        source = self._create_data_frame(data_source)
        return figure_instance
    
    @abstractmethod
    def _figure_reference_vertical_line(self, figure_instance : Any, text_source : dict):
        return figure_instance
    
    @abstractmethod
    def _figure_reference_horizontal_line(self, figure_instance : Any, text_source : dict):
        return figure_instance
    
    @abstractmethod
    def _dislpay_text_in_figure(self, figure_instance : Any, text_source : dict):
        return figure_instance
    
    
    
class BokehFigure(BaseFigure):
    def __init__(self):
        super().__init__()
    
    @abstractmethod
    def create_figure(self, figure_instance):
        figure_instance = super().create_figure(figure_instance)
        
        # Pass the data_source argument from the class instance
        # Pass the text_source argument from the class instance
        # self.prepare_figure(figure_instance, data_source, text_source)
        return figure_instance

    def _create_empty_plot(self):
        figure_instance = figure(
            height = 350,
            min_border_left=70,
            min_border_right=50,
            sizing_mode='stretch_both',
            toolbar_location='above',
            tools='xpan,xwheel_zoom,box_zoom,reset, hover, save',
            active_drag='xpan',
            output_backend='webgl')
        figure_instance.circle([],[])
        return figure_instance

    def _create_data_frame(self, data : dict):
        df = DataFrame(data)
        return ColumnDataSource(df)
    
    def _axis_label(self, figure_instance : Figure, label_source : dict):
        figure_instance.xaxis.axis_label = label_source['x']
        figure_instance.yaxis.axis_label = label_source['y']
        return figure_instance, label_source
    
    def _axis_range(self, figure_instance : Figure, label_source : dict):
        if label_source['x_range_start']and label_source['x_range_end']:
            x_range = Range1d(start=label_source['x_range_start'], end=label_source['x_range_end'])
            figure_instance.x_range = x_range
        return figure_instance, label_source
    
    def _legend_visible(self, figure_instance : Figure, text_source : dict):
        figure_instance.legend.visible = text_source['legend_visible']
        return figure_instance
    
    def _legend_location(self, figure_instance : Figure, text_source : dict):
        figure_instance.legend.location = text_source['legend_location']
        return figure_instance
    
    def _figure_vbar(self, figure_instance : Figure, data_source : dict, text_source : dict):
        bar_width = self.calculate_bar_width(data_source)
        source = self._create_data_frame(data_source)
        figure_instance.vbar(x='x', top='y', 
                             width=bar_width, 
                             legend_label=text_source['legend_label'], 
                             color=text_source['color'], 
                             alpha = 0.5, 
                             source = source)
        self._legend_visible(figure_instance, text_source)
        self._legend_location(figure_instance, text_source)
        return figure_instance
    
    def _figure_stack_vbar(self, figure_instance : Figure, stack_data : list, data_source : dict, text_source : dict):
        bar_width = self.calculate_bar_width(data_source)
        # source = self._create_data_frame(data_source)
        figure_instance.vbar_stack(stack_data , x='x', width=bar_width, color=text_source['color'], source = data_source)
        return figure_instance

    def _color_bar(self, figure_instance : Any, color_bar_source : dict):
        mapper = LinearColorMapper(palette=Spectral11, low=0, high=110)
        color_bar = ColorBar(
        color_mapper=mapper,
        width=color_bar_source['width'],
        height=color_bar_source['height'],
        location=color_bar_source['location'],
        title=color_bar_source['title'],
        ticker=FixedTicker(ticks=list(range(0, 110, 10)))
        )
        figure_instance.add_layout(color_bar, 'above')
        return figure_instance

    def _figure_line(self, figure_instance : Figure, data_source : dict, text_source : dict):
        source = self._create_data_frame(data_source)
        figure_instance.line(x='x', y='y',
                            line_width=2, 
                            legend_label=text_source['legend_label'], 
                            line_color=text_source['color'], 
                            line_dash = text_source['line_dash'], 
                            source = source)
        self._legend_visible(figure_instance, text_source)
        self._legend_location(figure_instance, text_source)
        return figure_instance, data_source, text_source
    
    def _figure_circle(self, figure_instance : Figure, data_source : dict):
        source = self._create_data_frame(data_source)
        figure_instance.circle(x = 'x', y = 'y', source=source, size=3)
        return figure_instance
    
    def _figure_reference_vertical_line(self, figure_instance : Figure, text_source : dict):
        v_line = Span(location=text_source['x'], dimension='height', line_color=text_source['color'], line_dash = "dashed", line_width=2)
        figure_instance.add_layout(v_line)
        return figure_instance
    
    def _figure_reference_horizontal_line(self, figure_instance : Figure, text_source : dict):
        h_line = Span(location=text_source['y'], dimension='width', line_color=text_source['color'], line_dash = "dashed",  line_width=2)
        figure_instance.add_layout(h_line)
        return figure_instance
    
    def _dislpay_text_in_figure(self, figure_instance : Figure, text_source : dict):
        label = Label(x=text_source['x'], 
                    y=text_source['y'], 
                    x_offset=text_source['x_offset'],
                    y_offset=text_source['y_offset'],
                    text=text_source['text'], 
                    text_align=text_source['text_align'],
                    angle=text_source['angle'], 
                    text_baseline=text_source['text_baseline'], 
                    text_font_size="8pt")
        figure_instance.add_layout(label)
        return figure_instance