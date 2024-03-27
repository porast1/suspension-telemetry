from bokeh.plotting import Figure
from myApi.ApiFigure import BokehFigure

class balance_data(BokehFigure):
    def __init__(self, data_travel : list, data_velocity : list, legend_label: str, damping: str):
        super().__init__()
        data_travel = data_travel[:]
        data_velocity = data_velocity[:]

        if damping == "Compression":
            travel, velocity = self.comp_velocity_with_travel(travel=data_travel, velocity=data_velocity)
        else:
            travel, velocity = self.reb_velocity_with_travel(travel=data_travel, velocity=data_velocity)
        travel_fit, linear_velocity = self.calculate_linear_fit(x_data=travel, y_data=velocity)

        self.__XY_label_source = self.template_XY_label_source()

        self.__data_source = self.template_data_source()

        self.__text_source = self.template_text_source(legend_label=legend_label)
        self.__angle_text_source = self.template_text_source(legend_label=legend_label)
        self.__avg_text_source = self.template_text_source(legend_label=legend_label)

        self.__XY_label_source['x'] = 'Travel [%]'
        self.__XY_label_source['y'] = 'Velocity [mm/s]'

        self.__data_source['x'] = travel_fit
        self.__data_source['y'] = linear_velocity

        self.__text_source['legend_location'] = 'top_left'

        angle = self.count_alfa(x_data=travel, y_data=velocity)
        self.__angle_text_source['text'] = f'{legend_label} Angle = {angle:.2f}'
        self.__angle_text_source['angle'] = 0

        avg_velocity = abs(self.avg_data(data=velocity))
        self.__avg_text_source['y'] = avg_velocity
        self.__avg_text_source['text'] = f'{legend_label} Avarage velocity = {avg_velocity:.2f}'
        self.__avg_text_source['angle'] = 0


    def create_figure(self, figure_instance : Figure):
        figure_instance = super().create_figure(figure_instance)

        XY_label_source = self.__XY_label_source
        data_source = self.__data_source
        text_source = self.__text_source
        angle_text_source = self.__angle_text_source
        avg_text_source = self.__avg_text_source
        
        self._configure_axes(figure_instance=figure_instance, label_source=XY_label_source)
        self._figure_line(figure_instance=figure_instance, data_source=data_source, text_source=text_source)

        self._figure_text(figure_instance=figure_instance, text_source=avg_text_source)
        self._figure_reference_horizontal_line(figure_instance=figure_instance, text_source=avg_text_source)

        self._figure_text(figure_instance=figure_instance, text_source=angle_text_source)
        
        return figure_instance