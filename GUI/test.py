from bokeh.palettes import Category10,Spectral11
from bokeh.plotting import figure, show
from numpy import histogram,where,array
velocities = array([-500,-400,-300,-200,-100,0,100,200,300,400,500,600])
percentTravel= array([0,10,20,30,40,50,60,70,80,90,100,105])

histTravel, edgeTravel = histogram(percentTravel, 11, range=(0,110))
Travel = [str(element) for element in edgeTravel[:-1]]
data = {'velocities' : None}
for edgeIndex in range(len(edgeTravel) -1):
    if edgeIndex == (len(edgeTravel) -1):
        mask = (percentTravel >= edgeTravel[edgeIndex]) & (percentTravel <= edgeTravel[edgeIndex+1])
    else:
        mask = (percentTravel >= edgeTravel[edgeIndex]) & (percentTravel < edgeTravel[edgeIndex+1])
    tmpVelocity = velocities[mask]
    histVel, edgeVel = histogram(tmpVelocity, 11, range=(-500,600))
    data[str(edgeTravel[edgeIndex])] = histVel
offset = (edgeVel[1] - edgeVel[0])/2
data['velocities'] = edgeVel[:-1] + offset
print(data)
p = figure(x_range=(min(edgeVel), max(edgeVel)), height=250, title="Fruit Counts by Year",
           toolbar_location=None, tools="hover")

# Oblicz szerokość słupka
bar_width = 0.9 * 2 * offset
p.vbar_stack(Travel, x='velocities', width=bar_width, color=Spectral11, source=data,
             legend_label=Travel,)

p.y_range.start = 0
p.x_range.start -= 0.05 * (p.x_range.end - p.x_range.start)  # Add padding to the start
p.x_range.end += 0.05 * (p.x_range.end - p.x_range.start)    # Add padding to the end

p.xgrid.grid_line_color = None
p.axis.minor_tick_line_color = None
p.outline_line_color = None
p.legend.location = "top_left"
p.legend.orientation = "horizontal"

show(p)