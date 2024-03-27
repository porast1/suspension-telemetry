import numpy as np
from bokeh.palettes import Spectral11
from bokeh.plotting import Figure
from scipy.stats import norm
from myApi.ApiFigure import BokehFigure


class VelocityHistogram(BokehFigure):
    def __init__(self, travel_data : list, velocity_data : list, legend_label : str):
        super().__init__()
        travel_data = travel_data[:]
        velocity_data = velocity_data[:]
        steps = 50
        self.__XY_label_source = self.template_XY_label_source()
        self.__norm_data_source = self.template_data_source()

        self.__vbar_text_source = self.template_text_source(legend_label=legend_label)
        self.__norm_text_source = self.template_text_source(legend_label=legend_label)
        self.__color_bar_source = self.template_color_bar_source()

        self.__avg_reb_text_source = self.template_text_source(legend_label=legend_label)
        self.__avg_comp_text_source = self.template_text_source(legend_label=legend_label)
        self.__max_reb_text_source = self.template_text_source(legend_label=legend_label)
        self.__max_comp_text_source = self.template_text_source(legend_label=legend_label)

        self.__XY_label_source['x'] = 'Velocity [mm/s]'
        self.__XY_label_source['y'] = 'Travel [%]'
        self.__XY_label_source['x_range_start'] = -2500
        self.__XY_label_source['x_range_end'] = 2500
        
        self.__travel_stack, self.__vbar_data_source = self.__calculate_stack_histogram( travel_data=travel_data, velocity_data=velocity_data, steps=steps)

        ny, pdf = self._normal_distribution_data(velocity=velocity_data, step=steps)
        self.__norm_data_source['x'] = ny
        self.__norm_data_source['y'] = pdf

        self.__vbar_text_source['color'] = Spectral11

        self.__norm_text_source['color'] = 'green'
        self.__norm_text_source['line_dash'] = 'dashed'
        self.__norm_text_source['legend_visible'] = False

        self.__color_bar_source['title'] = 'Travel [%]'

        avg_velocity_reb = self.avg_data(self.reb_velocity(velocity=velocity_data))
        self.__avg_reb_text_source['x'] = avg_velocity_reb
        self.__avg_reb_text_source['text'] = f'AVG Rebound: {avg_velocity_reb:.2f}'

        avg_velocity_comp = self.avg_data(self.comp_velocity(velocity=velocity_data))
        self.__avg_comp_text_source['x'] = avg_velocity_comp
        self.__avg_comp_text_source['text'] = f'AVG Comp: {avg_velocity_comp:.2f}'

        max_velocity_reb = self.min_data(self.reb_velocity(velocity=velocity_data))
        self.__max_reb_text_source['x'] = self.fit_text_refline_coordinates(label_source=self.__XY_label_source, coordinate=max_velocity_reb)
        self.__max_reb_text_source['text'] = f'MAX Rebound: {max_velocity_reb:.2f}'

        max_velocity_comp = self.max_data(self.comp_velocity(velocity=velocity_data))
        self.__max_comp_text_source['x'] = self.fit_text_refline_coordinates(label_source=self.__XY_label_source, coordinate=max_velocity_comp)
        self.__max_comp_text_source['text'] = f'MAX Comp: {max_velocity_comp:.2f}'

        

    def create_figure(self, figure_instance : Figure):
        figure_instance = super().create_figure(figure_instance)
        
        stack_data = self.__travel_stack
        vbar_data_source = self.__vbar_data_source
        norm_data_source = self.__norm_data_source

        vbar_text_source = self.__vbar_text_source
        norm_text_source = self.__norm_text_source
        color_bar_source = self.__color_bar_source

        XY_label_source = self.__XY_label_source

        avg_reb_text_source = self.__avg_reb_text_source
        avg_comp_text_source = self.__avg_comp_text_source
        max_reb_text_source = self.__max_reb_text_source
        max_comp_text_source = self.__max_comp_text_source

        self._configure_axes(figure_instance=figure_instance, label_source=XY_label_source)

        self._color_bar(figure_instance=figure_instance, color_bar_source=color_bar_source)
        self._figure_stack_vbar(figure_instance=figure_instance, stack_data=stack_data, data_source=vbar_data_source, text_source=vbar_text_source)

        self._figure_line(figure_instance=figure_instance, data_source=norm_data_source, text_source=norm_text_source)

        self._figure_text(figure_instance=figure_instance, text_source=self.__avg_reb_text_source)
        self._figure_reference_vertical_line(figure_instance=figure_instance, text_source=avg_reb_text_source)

        self._figure_text(figure_instance=figure_instance, text_source=self.__avg_comp_text_source)
        self._figure_reference_vertical_line(figure_instance=figure_instance, text_source=avg_comp_text_source)

        self._figure_text(figure_instance=figure_instance, text_source=self.__max_reb_text_source)
        self._figure_reference_vertical_line(figure_instance=figure_instance, text_source=max_reb_text_source)

        self._figure_text(figure_instance=figure_instance, text_source=self.__max_comp_text_source)
        self._figure_reference_vertical_line(figure_instance=figure_instance, text_source=max_comp_text_source)
        return figure_instance




    def _normal_distribution_data(self, velocity: list[float], step: float):
        mu, std = norm.fit(velocity)
        ny = np.linspace(-2500, 2500, 100)
        pdf = norm.pdf(ny, mu, std) * 100 * 100
        return ny, pdf

    def __calculate_stack_histogram(self, travel_data : list, velocity_data : list, steps : int=50):
        len_data = len(travel_data)
        travel_stack = [element for element in range(0, 110, 10)]
        
        data = {'x': None}
        for travel_index in range(len(travel_stack) - 1):
            mask = (travel_data >= travel_stack[travel_index]) & (travel_data < travel_stack[travel_index + 1])
            tmp_velocity = velocity_data[mask]
            mask = ((tmp_velocity <=-20) | (tmp_velocity >= 20)) 
            tmp_velocity = tmp_velocity[mask]
            hist_vel, edge_vel = np.histogram(tmp_velocity, bins=steps, range=(-2500, 2500))
            data[str(int(travel_stack[travel_index]))] = 100 * hist_vel / len_data  

        offset = (edge_vel[1] - edge_vel[0]) / 2
        data['x'] = edge_vel[:-1] + offset
        travel_stack = [str(element) for element in travel_stack]
        return travel_stack, data