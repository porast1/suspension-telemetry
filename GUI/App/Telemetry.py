import numpy as np
from bokeh.plotting import figure, Figure
from bokeh.models import Range1d, Label
from sessionInit import BaseFigure, UserApi


def general_figure(api : UserApi, suspension_data : dict):
    api.header("General diagram of suspension work")
    selector = api.multi_selector("Choose data series:", ["Front", "Rear"])
    front_figure = GeneralTravelFigure(time_data=suspension_data['time'], travel_data=suspension_data['front_travel'])
    rear_figure = GeneralTravelFigure(time_data=suspension_data['time'], travel_data=suspension_data['rear_travel'])
    figure = front_figure.create_empty_plot()
    if "Front" in selector:
        figure = front_figure.create_figure(figure_instance= figure,legend_label="Front", line_color="blue")
    if "Rear" in selector:
        figure = rear_figure.create_figure(figure_instance= figure, legend_label="Rear", line_color="red")
    api.show_figure(figure)

def travel_histogram(api : UserApi, suspension_data : dict):
    api.header("Histogram of suspension stiffness")
    api.display_text('This section describes the hardness of our suspension, the first two graphs tell us in what sections of the travel we spend %% of the time. \nExample: The further to the left = stiffer suspension, to the right = the softer \nor simply: The softer the suspension, the more time we will spend in the upper travel values and vice versa.')
    bins = 20
    front_hist = TravelHistogram(bins=bins, travel_data=suspension_data['front_travel_percent'], parameter_name="Front")
    rear_hist = TravelHistogram(bins=bins, travel_data=suspension_data['rear_travel_percent'], parameter_name="Rear")
    histogram = front_hist.create_empty_plot()
    histogram = front_hist.create_figure(figure_instance= histogram,legend_label="Front", line_color="blue")
    histogram = rear_hist.create_figure(figure_instance= histogram, legend_label="Rear", line_color="red")
    api.show_figure(histogram)

class GeneralTravelFigure(BaseFigure):
    def __init__(self, travel_data : list, time_data : list):
        super().__init__()
        self.__travel = travel_data
        self.__time = time_data
    def label_XY(self, figure_instance : Figure):
        figure_instance.xaxis.axis_label = 'Time [s]'
        figure_instance.yaxis.axis_label = 'Travel [mm]'
        return figure_instance
    def figure_data(self,figure_instance : Figure, legend_label : str, line_color : str):
        figure_instance.line(x=self.__time, y=self.__travel, line_width=2, legend_label=legend_label, line_color=line_color)
        return figure_instance

class TravelHistogram(BaseFigure):
    def __init__(self, bins:int, travel_data : list, parameter_name : str):
        super().__init__()
       
        self.__bins = bins
        self.__avg_travel = self.avg_travel(travel=travel_data)
        self.__max_travel = self.max_travel(travel=travel_data)
        self.__parameter_name = parameter_name
        self.__hist_time, self.__edge = self.__calculateHistogramData(travel_data=travel_data,bins=bins)

    def __calculateHistogramData(self, travel_data : list, bins : int):
        hist, edges = np.histogram(travel_data, bins)
        hist = (hist/len(travel_data)) * 100
        return hist, edges
    
    def label_XY(self, figure_instance: Figure):
        figure_instance.xaxis.axis_label = 'Travel [%]'
        figure_instance.yaxis.axis_label = 'Time [%]'
        x_range = Range1d(start=-5, end=105)
        figure_instance.x_range = x_range
        return figure_instance
    
    def figure_data(self,figure_instance : Figure, legend_label : str, line_color : str):
        bar_width = self.__max_travel / (self.__bins + 1)
        figure_instance.vbar(x=self.__edge[:-1], top=self.__hist_time, width=bar_width, legend_label=legend_label, color=line_color, alpha = 0.5)
        figure_instance.line(x=[self.__avg_travel, self.__avg_travel], y=[min(self.__hist_time), max(self.__hist_time) + 2], color=line_color)

        label_text = f"{self.__parameter_name} Avarage = {self.__avg_travel:.2f}"
        line_label = Label(x=self.__avg_travel + 0.1, y=(min(self.__hist_time) + max(self.__hist_time)) / 2, text=label_text, text_font_size="8pt")
        figure_instance.add_layout(line_label)
        return figure_instance