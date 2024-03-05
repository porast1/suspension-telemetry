import numpy as np 
from bokeh.plotting import figure, Figure
from sessionInit import BaseDataAndFigure
from streamlit import header, write, selectbox, bokeh_chart, cache_data
class ProgressionFigure(BaseDataAndFigure):
    def __init__(self):
        super().__init__()
        header('Travel/Spring rate')
        write('this graph shows the average - dynamic progression of our suspension based on readings from the travel sensor. Very useful for finding the balance of our suspension in terms of overall hardness. The overall hardness is influenced by the slow-fast compression settings, the suspension hardness and the number of tokens. Therefore, this chart must be considered on the basis of the charts below, first we set the relative balance of the air spring/sag hardness, then the balance settings for compression and come back here to verify the settings')
        self.__Mode = selectbox("Choose figure type:", ['Progression', 'Velocity'])
        self.__X_Axis = selectbox("Choose display option:", ['Travel [%]' , 'Travel [mm]'])
        frontDataTravelPercent = self.getFrontTravelPercent()
        frontDataVelocity = self.getFrontVelocity()
        rearDataTravelPercent = self.getRearTravelPercent()
        rearDataVelocity = self.getRearVelocity()
        self.__uniqueFrontTravel, self.__uniqueFrontVelocity = self.__calculateUniqueVelocity(dataTravel=frontDataTravelPercent, dataVelocity=frontDataVelocity)
        self.__uniqueRearTravel, self.__uniqueRearVelocity = self.__calculateUniqueVelocity(dataTravel=rearDataTravelPercent, dataVelocity=rearDataVelocity)
        self.__frontProgressionTime = self.__calculateProgression(uniqueTravel=self.__uniqueFrontTravel, uniqueVelocity= self.__uniqueFrontVelocity)
        self.__rearProgressionTime = self.__calculateProgression(uniqueTravel= self.__uniqueRearTravel, uniqueVelocity = self.__uniqueRearVelocity)
    def XY_LabelAbstract(self, abstractFigure : Figure):
        if 'Progression' == self.__Mode:
            abstractFigure.xaxis.axis_label = 'Travel [%]'
            abstractFigure.yaxis.axis_label = 'Time [s]'
        else:
            abstractFigure.xaxis.axis_label = 'Travel [%]'
            abstractFigure.yaxis.axis_label = 'Velocity [mm/s]'
        return abstractFigure
    def simpleFigureAbstract(self, abstractFigure : Figure):
        if 'Progression' == self.__Mode:
            abstractFigure.line(x=self.__uniqueFrontTravel, y=self.__frontProgressionTime, line_width=2, legend_label='Front', line_color='blue')
            abstractFigure.line(x=self.__uniqueRearTravel, y=self.__rearProgressionTime, line_width=2, legend_label='Rear', line_color='red')
        else:
            abstractFigure.line(x=self.__uniqueFrontTravel, y=self.__uniqueFrontVelocity, line_width=2, legend_label='Front', line_color='blue')
            abstractFigure.line(x=self.__uniqueRearTravel, y=self.__uniqueRearVelocity, line_width=2, legend_label='Rear', line_color='red')
        abstractFigure.legend.location = 'top_left'
        return abstractFigure
    @cache_data
    def __calculateUniqueVelocity(_self, dataVelocity : list, dataTravel : list ):
        mask = dataVelocity >= 0
        indices = np.where(mask)[0]
        dataVelocity = dataVelocity[indices]
        dataTravel = dataTravel[indices]
        sorted_Indices = np.argsort(dataTravel)
        sortedVelocity = dataVelocity[sorted_Indices]
        sortedTravel = np.sort(dataTravel)

        uniqueTravel = np.array(np.unique(sortedTravel))
        uniqueVelocity = np.array([])
        for unique_value in uniqueTravel:
            meanVelocity = np.mean(sortedVelocity[sortedTravel == unique_value])
            if 0 < meanVelocity:
                uniqueVelocity = np.append(uniqueVelocity,meanVelocity)
            else:
                uniqueVelocity = np.append(uniqueVelocity,np.nan)
        non_nan_indices = np.arange(len(uniqueVelocity))[~np.isnan(uniqueVelocity)]
        uniqueVelocity = np.interp(np.arange(len(uniqueVelocity)), non_nan_indices, uniqueVelocity[non_nan_indices])
        uniqueVelocity[0] = 0
        return uniqueTravel, uniqueVelocity
    @cache_data
    def __calculateProgression(_self,  uniqueTravel : list, uniqueVelocity : list):
        progressionTime = np.array([])
        for i in range(1, len(uniqueTravel)):
            tmpPercent = uniqueTravel[1:i]
            tmpVelocity = uniqueVelocity[1:i]
            
            if len(tmpPercent) > 0 and len(tmpVelocity) > 0 and 0 not in tmpVelocity[1:]:
                wynik = 1/tmpVelocity
                progressionTime = np.append(progressionTime, np.trapz(wynik, tmpPercent))
            else:
                progressionTime = np.append(progressionTime, np.nan)
        progressionTime[0] = 0
        progressionTime = np.insert(progressionTime, 0, 0)
        return progressionTime