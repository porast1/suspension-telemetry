import numpy as np
from bokeh.plotting import figure, Figure
from bokeh.models import Range1d, Label
from sessionInit import BaseDataAndFigure
from streamlit import header, multiselect, write
class GeneralTravelFigure(BaseDataAndFigure):
    #private

    #public

    def __init__(self):
        super().__init__()
        header("General diagram of suspension work")
        self.__selectedSeries = multiselect("Choose data series:", ["Front", "Rear"])
    def XY_LabelAbstract(self, abstractFigure : Figure):
        abstractFigure.xaxis.axis_label = 'Time [s]'
        abstractFigure.yaxis.axis_label = 'Travel [mm]'
        return abstractFigure
    def simpleFigureAbstract(self,abstractFigure : Figure):
        if "Front" in self.__selectedSeries:
            abstractFigure.line(x=self.getTimeTravelPressure(), y=self.getFrontTravel(), line_width=2, legend_label='Front', line_color='blue')
        if "Rear" in self.__selectedSeries:
            abstractFigure.line(x=self.getTimeTravelPressure(), y=self.getRearTravel(), line_width=2, legend_label='Rear', line_color='red')
        return abstractFigure

class TravelHistogram(BaseDataAndFigure):
    #private

    #public

    def __init__(self, bins:int):
        super().__init__()
        write('This section describes the hardness of our suspension, the first two graphs tell us in what sections of the travel we spend %% of the time. \nExample: The further to the left = stiffer suspension, to the right = the softer \nor simply: The softer the suspension, the more time we will spend in the upper travel values and vice versa.')
        self.__bins = bins
        self.__histFrontTime, self.__frontEdge = self.__calculateHistogramData(dataTravel=self.getFrontTravelPercent(),bins=bins)
        self.__histRearTime, self.__rearEdge = self.__calculateHistogramData(dataTravel=self.getRearTravelPercent(),bins=bins)

    def __calculateHistogramData(self, dataTravel : list, bins : int):
        hist, edges = np.histogram(dataTravel, bins)
        hist = (hist/len(dataTravel)) * 100
        return hist, edges
    
    def XY_LabelAbstract(self, abstractFigure: Figure):
        abstractFigure.xaxis.axis_label = 'Travel [%]'
        abstractFigure.yaxis.axis_label = 'Time [%]'
        x_range = Range1d(start=-5, end=105)
        abstractFigure.x_range = x_range
        return abstractFigure
    def simpleFigureAbstract(self, abstractFigure: Figure):
        abstractFigure.vbar(x=self.__frontEdge[:-1], top=self.__histFrontTime, width=max(self.getFrontTravelPercent()) / (self.__bins - 1), legend_label="Front", color="blue", alpha = 0.5)
        abstractFigure.vbar(x=self.__rearEdge[:-1], top=self.__histRearTime, width=max(self.getRearTravelPercent()) / (self.__bins - 1), legend_label="Rear", color="red", alpha=0.5)
        averageFront_value = np.mean(self.getFrontTravelPercent())
        averageRear_value = np.mean(self.getRearTravelPercent())
        
        abstractFigure.line(x=[averageFront_value, averageFront_value], y=[min(self.__histFrontTime), max(self.__histFrontTime) + 2], color="blue")
        labelFront = Label(x=averageFront_value + 0.1, y=(min(self.__histFrontTime) + max(self.__histFrontTime)) / 2, text=f"Front Avarage = {averageFront_value:.2f}", text_font_size="8pt")
        abstractFigure.add_layout(labelFront)
        abstractFigure.line(x=[averageRear_value, averageRear_value], y=[min(self.__histRearTime), max(self.__histRearTime) + 2], color="red")
        labelRear = Label(x=averageRear_value + 0.1, y=(min(self.__histRearTime) + max(self.__histRearTime)) / 4, text=f"Rear Avarage = {averageRear_value:.2f}", text_font_size="8pt")
        abstractFigure.add_layout(labelRear)
        return abstractFigure