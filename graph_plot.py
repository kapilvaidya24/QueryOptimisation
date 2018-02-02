import numpy as np
import sys
import os
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def plot1(query, data):
    plt.figure()
    
    plot_lines = []
    x = [val[0] for val in data]
    y1 = [val[1] for val in data]
    y2 = [val[2] for val in data]
    line, = plt.plot(x, y1, label = "DPCCP")
    plot_lines.append(line)

    line, = plt.plot(x, y2, label = "Proposed Algorithm")
    plot_lines.append(line)

    plt.xlabel("Query Size")
    plt.ylabel("Time taken (in s)")
    plt.legend(handles = plot_lines, loc = 'best')
    plt.title("Time for {} queries".format(query))
    plt.savefig("graph_{}.png".format(query), bbox_inches = "tight")

def plot2(query, data):
    plt.figure()
    
    plot_lines = []
    x = [val[0] for val in data]
    y1 = [val[3] for val in data]
    y2 = [val[4] for val in data]
    line, = plt.plot(x, y1, label = "DPCCP")
    plot_lines.append(line)

    line, = plt.plot(x, y2, label = "Proposed Algorithm")
    plot_lines.append(line)

    plt.xlabel("Query Size")
    plt.ylabel("No of nodes explored")
    plt.legend(handles = plot_lines, loc = 'best')
    plt.title("Explored Nodes for {} queries".format(query))
    plt.savefig("graph_explored_{}.png".format(query), bbox_inches = "tight")

# def plot2(query1, query2, data):
#     plt.figure()
    
#     plot_lines = []
    
#     x = xrange(6, 16)
#         x = [val[0] for val in data[key]]
#         y1 = [data[key][s] for s in x]
#         y2 = [val[2] for val in data]
#         line, = plt.plot(x, y1, label = "DPCCP")
#         plot_lines.append(line)

#     line, = plt.plot(x, y2, label = "Proposed Algorithm")
#     plot_lines.append(line)

#     plt.xlabel("Query Size")
#     plt.ylabel("Time taken (in s)")
#     plt.legend(handles = plot_lines, loc = 'best')
#     plt.savefig("graph_{}.png".format(query), bbox_inches = "tight")
# def plot2(lambdas):
#     plt.figure()
#     plot_lines = []
#     for lamb in lambdas:
#         values = np.loadtxt("data/exp2_lamb{}".format(lamb))
#         x = range(values.shape[0])
#         y = np.mean(values, axis = 1)
#         line, = plt.plot(x, y, label = "Lambda = {}".format(lamb))
#         plot_lines.append(line)

#     plt.xlabel("Number of updates")
#     plt.ylabel("Average State Value")
#     plt.legend(handles = plot_lines, loc = 'best')
#     plt.savefig("experiement_2.png", bbox_inches = "tight")


if __name__ == '__main__':
    results = {}
    query = ''
    size = 0
    count = 0
    time_dpccp = 0.0
    time_algo = 0.0
    explored_dpccp = 0
    explored_algo = 0
    with open('results.txt', 'r') as resultsFile:
    
        for line in resultsFile:
            values = [val for val in line.split()]
            print values
            if values[0] != query or size != int(values[1]):
                if size != 0:
                    if query not in results:
                        results[query] = []
                    results[query].append([size, time_dpccp/count, time_algo/count, explored_dpccp/count, explored_algo/count])
                query = values[0]
                size = int(values[1])
                count = 0
                time_dpccp = 0.0
                time_algo = 0.0
                explored_dpccp = 0
                explored_algo = 0

            time_dpccp += float(values[8])
            time_algo += float(values[9])
            explored_dpccp += int(values[4])
            explored_algo += int(values[6])
            count += 1
        results[query].append([size, time_dpccp/count, time_algo/count, explored_dpccp/count, explored_algo/count])
    for key in results:
        plot1(key, results[key])
        plot2(key, results[key])
