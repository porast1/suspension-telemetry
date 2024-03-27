from numpy import histogram

from myApi.ApiFigure import BokehFigure

class GeneralTravelFigure(BokehFigure):
    def __init__(self, travel_data : list, time_data : list, legend_label : str):
        super().__init__()
        time_data = time_data[:]
        travel_data = travel_data[:]

        self.__XY_label_source = self.template_XY_label_source()
        self.__data_source = self.template_data_source()
        self.__text_source = self.template_text_source(legend_label=legend_label)
        
        self.__XY_label_source['x'] = 'Time [s]'
        self.__XY_label_source['y'] = 'Travel [mm]'
        
        self.__data_source['x'] = time_data
        self.__data_source['y'] = travel_data

    def prepare_figure(self, figure_instance, data_source, text_source, XY_label_source):
        figure_instance = super().prepare_figure(figure_instance=figure_instance, data_source=data_source, text_source=text_source, XY_label_source=XY_label_source)
        self._figure_line(figure_instance=figure_instance, data_source=data_source, text_source=text_source)
        return figure_instance
    
    def create_figure(self, figure_instance):
        figure_instance = super().create_figure(figure_instance=figure_instance)
        
        data_source = self.__data_source
        text_source = self.__text_source
        XY_label_source = self.__XY_label_source
        self._configure_axes(figure_instance=figure_instance, label_source=XY_label_source)
        self._figure_line(figure_instance=figure_instance, data_source=data_source, text_source=text_source)
        return figure_instance
        


class TravelHistogram(BokehFigure):
    def __init__(self, bins:int, travel_data : list, legend_label : str = None):
        super().__init__()
        travel_data = travel_data[:]

        self.__XY_label_source = self.template_XY_label_source()
        self.__data_source = self.template_data_source()
        self.__avg_text_source = self.template_text_source(legend_label=legend_label)
        self.__max_text_source = self.template_text_source(legend_label=legend_label)

        hist_time, edge = self.__calculateHistogramData(travel_data=travel_data,bins=bins)

        self.__XY_label_source['x'] = 'Travel [%]'
        self.__XY_label_source['y'] = 'Time [%]'
        self.__XY_label_source['x_range_start'] = -5
        self.__XY_label_source['x_range_end'] = 110

        self.__data_source['x'] = edge[:-1]
        self.__data_source['y'] = hist_time

        avg_travel = self.avg_data(data=travel_data)
        self.__avg_text_source['text'] = f"{legend_label} Avarage = {avg_travel:.2f}"
        self.__avg_text_source['x'] = avg_travel

        max_travel = self.max_data(data=travel_data)
        self.__max_text_source['x'] = self.fit_text_refline_coordinates(label_source=self.__XY_label_source, coordinate=max_travel)
        self.__max_text_source['text'] = f"{legend_label} Max Travel = {max_travel:.2f}"
        
            
    def __calculateHistogramData(self, travel_data : list, bins : int):
        hist, edges = histogram(travel_data, bins)
        hist = (hist/len(travel_data)) * 100
        return hist, edges

    def create_figure(self, figure_instance):
        figure_instance = super().create_figure(figure_instance=figure_instance)

        XY_label_source = self.__XY_label_source
        data_source = self.__data_source
        base_text_source = self.__avg_text_source
        avg_text_source = self.__avg_text_source
        max_text_source = self.__max_text_source
        
        self._configure_axes(figure_instance=figure_instance, label_source=XY_label_source)

        self._figure_vbar(figure_instance=figure_instance, data_source=data_source, text_source=base_text_source)

        self._figure_text(figure_instance=figure_instance, text_source=avg_text_source)
        self._figure_reference_vertical_line(figure_instance=figure_instance, text_source=avg_text_source)

        self._figure_text(figure_instance=figure_instance, text_source=max_text_source)
        self._figure_reference_vertical_line(figure_instance=figure_instance, text_source=max_text_source)
        return figure_instance