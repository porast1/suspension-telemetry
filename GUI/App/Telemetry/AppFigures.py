
from myApi.ApiGUI import UserApi
from Telemetry.myFFT import fftFigure
from Telemetry.Balance import balance_data
from Telemetry.Progression import ProgressionFigure
from Telemetry.Telemetry import GeneralTravelFigure, TravelHistogram
from Telemetry.Velocity import VelocityHistogram


def balance_figure(api : UserApi, suspension_data : dict, damping: str):
    api.subheader(damping)
    front_figure = balance_data(data_travel=suspension_data['front_travel_percent'], data_velocity=suspension_data['front_velocity'], legend_label="Front", damping=damping)
    rear_figure = balance_data(data_travel=suspension_data['rear_travel_percent'], data_velocity=suspension_data['rear_velocity'], legend_label="Rear", damping=damping)
    new_balance = front_figure.create_figure(figure_instance=None)
    new_balance = rear_figure.create_figure(figure_instance=new_balance)
    api.show_figure(new_balance)

def fft_figure(api : UserApi, suspension_data : dict):
    api.header('FFT Charts')
    api.subheader('Front')
    frontFFT = fftFigure(data_travel=suspension_data['front_travel_percent'], data_time=suspension_data['time'])
    frontFFT_figure = frontFFT.create_figure(figure_instance=None)
    api.show_figure(frontFFT_figure)
    
    api.subheader('Rear')
    rearFFT = fftFigure(data_travel=suspension_data['rear_travel_percent'], data_time=suspension_data['time'])
    rearFFT_figure = rearFFT.create_figure(figure_instance=None)
    api.show_figure(rearFFT_figure)

def progression_figure(api : UserApi, suspension_data : dict):
    api.header('Travel/Spring rate')
    api.display_text('this graph shows the average - dynamic progression of our suspension based on readings from the travel sensor. Very useful for finding the balance of our suspension in terms of overall hardness. The overall hardness is influenced by the slow-fast compression settings, the suspension hardness and the number of tokens. Therefore, this chart must be considered on the basis of the charts below, first we set the relative balance of the air spring/sag hardness, then the balance settings for compression and come back here to verify the settings')
    selector = api.selector("Choose figure type:", ['Progression', 'Velocity'])
    progression_front_figure = ProgressionFigure(data_travel=suspension_data['front_travel_percent'], data_velocity=suspension_data['front_velocity'], legend_label="Front", Mode=selector)
    progression_rear_figure = ProgressionFigure(data_travel=suspension_data['rear_travel_percent'], data_velocity=suspension_data['rear_velocity'], legend_label="Rear", Mode=selector)
    new_figure = progression_front_figure.create_figure(figure_instance=None)
    new_figure = progression_rear_figure.create_figure(figure_instance=new_figure)
    api.show_figure(new_figure)

def general_figure(api : UserApi, suspension_data : dict):
    api.header("General diagram of suspension work")
    front_figure = GeneralTravelFigure(time_data=suspension_data['time'], travel_data=suspension_data['front_travel'], legend_label="Front")
    rear_figure = GeneralTravelFigure(time_data=suspension_data['time'], travel_data=suspension_data['rear_travel'], legend_label="Rear")
    new_figure = front_figure.create_figure(figure_instance=None)
    new_figure = rear_figure.create_figure(figure_instance=new_figure)
    api.show_figure(new_figure)

def travel_histogram(api : UserApi, suspension_data : dict):
    api.header("Histogram of suspension stiffness")
    api.display_text('This section describes the hardness of our suspension, the first two graphs tell us in what sections of the travel we spend %% of the time. \nExample: The further to the left = stiffer suspension, to the right = the softer \nor simply: The softer the suspension, the more time we will spend in the upper travel values and vice versa.')
    bins = 20
    front_hist = TravelHistogram(bins=bins, travel_data=suspension_data['front_travel_percent'], legend_label="Front")
    rear_hist = TravelHistogram(bins=bins, travel_data=suspension_data['rear_travel_percent'], legend_label="Rear")
    new_histogram = front_hist.create_figure(figure_instance=None)
    new_histogram = rear_hist.create_figure(figure_instance=new_histogram)
    api.show_figure(new_histogram)

def velocity_distribution_figure(api : UserApi, suspension_data : dict):
    api.header("Velocity distribution")
    api.display_text('This section describes the distribution of the velocity of our suspension')
    api.subheader('Front')
    front_hist = VelocityHistogram(travel_data=suspension_data['front_travel_percent'], velocity_data=suspension_data['front_velocity'], legend_label="Front")
    front_histogram = front_hist.create_figure(figure_instance=None)
    api.show_figure(front_histogram)

    api.subheader('Rear')
    rear_hist = VelocityHistogram(travel_data=suspension_data['rear_travel_percent'], velocity_data=suspension_data['rear_velocity'], legend_label="Rear")
    rear_histogram = rear_hist.create_figure(figure_instance=None)
    api.show_figure(rear_histogram)