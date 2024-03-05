from bokeh.plotting import Figure, figure
import numpy as np
from bokeh.models import ColumnDataSource
from sessionInit import BaseDataAndFigure

class balance_data(BaseDataAndFigure):
    def __init__(self, bikeSetup: str):
        super().__init__()
        self.__bikeSetup = bikeSetup
        self.__frontVelocityCompCoefficients,self.__frontVelocityRebCoefficients,self.__frontVelocityComp, self.__frontVelocityReb = self.balance_data(Travel=self.getFrontTravelPercent(), Velocity=self.getFrontVelocity())
        self.__rearVelocityCompCoefficients, self.__rearVelocityRebCoefficients,self.__rearVelocityComp, self.__rearVelocityReb = self.balance_data(Travel=self.getRearTravelPercent(), Velocity=self.getRearVelocity())
    def balance_data(self, Travel, Velocity):
        maskCompression = (Velocity >= 0)
        maskRebound = (Velocity < 0)
        velocityCompression = Velocity[maskCompression]
        travelCompression = Travel[maskCompression]
        velocityRebound = abs(Velocity[maskRebound])
        travelRebound = Travel[maskRebound]
        coefficientsComp  = np.polyfit(travelCompression, velocityCompression,1)
        coefficientsReb  = np.polyfit(travelRebound, velocityRebound,1)
        return coefficientsComp, coefficientsReb, velocityCompression, velocityRebound
    def count_alfa(self, tgAlfa):
        degreeVariable = np.degrees(np.arctan(tgAlfa))
        return degreeVariable
    def XY_LabelAbstract(self, abstractFigure: Figure):
        abstractFigure.xaxis.axis_label = 'Travel [%]'
        abstractFigure.yaxis.axis_label = 'Velocity [mm/s]'
        return abstractFigure
    def simpleFigureAbstract(self, abstractFigure: Figure):
        frontTravelFit = np.arange(0, 100 + 1, 1)
        rearTravelFit = np.arange(0, 100 + 1, 1)
        if self.__bikeSetup == "Comp":
            frontLinearFit = np.poly1d(self.__frontVelocityCompCoefficients)
            rearLinearFit = np.poly1d(self.__rearVelocityCompCoefficients)
            linearVelocityFront = frontLinearFit(frontTravelFit)
            linearVelocityRear = rearLinearFit(rearTravelFit)
            frontAlfa = self.count_alfa(self.__frontVelocityCompCoefficients[0])
            rearAlfa = self.count_alfa(self.__rearVelocityCompCoefficients[0])
            avgFront = abs(np.mean(self.__frontVelocityComp))
            avgRear = abs(np.mean(self.__rearVelocityComp))
        else:
            frontLinearFit = np.poly1d(self.__frontVelocityRebCoefficients)
            rearLinearFit = np.poly1d(self.__rearVelocityRebCoefficients)
            linearVelocityFront = frontLinearFit(frontTravelFit)
            linearVelocityRear = rearLinearFit(rearTravelFit)
            frontAlfa = self.count_alfa(self.__frontVelocityRebCoefficients[0])
            rearAlfa = self.count_alfa(self.__rearVelocityRebCoefficients[0])
            avgFront = abs(np.mean(self.__frontVelocityReb))
            avgRear = abs(np.mean(self.__rearVelocityReb))
        
        
        # Tworzenie źródła danych dla tekstu
        text_source_front = ColumnDataSource(data=dict(x=[0], y=[avgFront], text=[f'AVG Front: {avgFront:.2f}']))
        text_source_rear = ColumnDataSource(data=dict(x=[0], y=[avgRear], text=[f'AVG Rear: {avgRear:.2f}']))
        text_source_front_alfa = ColumnDataSource(data=dict(x=[0], y=[1.5*max(avgFront,avgRear)], text=[f'Alfa Front: {frontAlfa:.2f}']))
        text_source_rear_alfa = ColumnDataSource(data=dict(x=[0], y=[1.4*max(avgFront,avgRear)], text=[f'Alfa Rear: {rearAlfa:.2f}']))
        abstractFigure.line(x=frontTravelFit, y=linearVelocityFront, line_width=2, legend_label='Front', line_color='blue')
        abstractFigure.line(x=rearTravelFit, y=linearVelocityRear, line_width=2, legend_label='Rear', line_color='red')
        abstractFigure.segment(x0=[0,0], y0=[avgFront, avgRear], x1=[99,99], y1=[avgFront, avgRear], line_width=2, line_dash="dashed", line_color=["blue", "red"])

        # Dodawanie tekstu z informacją o średnich wartościach
        abstractFigure.text(x='x', y='y', text='text', source=text_source_front, text_align="left", text_baseline="top", text_font_size="9pt")
        abstractFigure.text(x='x', y='y', text='text', source=text_source_rear, text_align="left", text_baseline="top", text_font_size="9pt")
        abstractFigure.text(x='x', y='y', text='text', source=text_source_front_alfa, text_align="left", text_baseline="bottom", text_font_size="9pt")
        abstractFigure.text(x='x', y='y', text='text', source=text_source_rear_alfa, text_align="left", text_baseline="bottom", text_font_size="9pt")
        return abstractFigure