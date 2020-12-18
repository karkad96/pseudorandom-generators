import matplotlib.pyplot as plt
from subprocess import call
import numpy
import sys

def getStatsStocha():

    f = open("./stocha/stats_s", "r")
    lines = f.readlines()
    nums = []

    for i in lines:
        s = i.strip().split()[2]
    
        if s == '':
            continue

        nums.append(float(s))

    f.close()

    return nums

def getPercentagesStocha():

    f = open("./stocha/percentage_s", "r")
    lines = f.readlines()
    perc = dict()

    perc["other functions"] = 0.0

    for i in lines[-8:]:
        cols = i.split()
        perc[cols[0]] = float(cols[1])

    perc["other functions"] = 100.0 - sum(perc.values())

    f.close()

    return perc

def getPlotStocha():

    nums = getStatsStocha()
    x_axis = list(range(1, len(nums) + 1))

    average = str(numpy.average(nums))[0:9]
    std =  str(numpy.std(nums))[0:9]

    plt.subplot(121)
    plt.xlabel('n-th probe', fontsize=11)
    plt.ylabel('time [sec]', fontsize=11)
    plt.suptitle('Reading 450000 pseudorandom numbers from character device using random method')

    plt.grid(True)
    plt.plot(x_axis, nums, '-o')

def getPieChartStocha():
    
    perc = getPercentagesStocha()
    nums = getStatsStocha()

    average = str(numpy.average(nums))[0:9]
    std =  str(numpy.std(nums))[0:9]
  
    plt.subplot(122)
    plt.pie([i for i in perc.values()], autopct='%1.1f%%', shadow=True, startangle=90)
    plt.legend([i for i in perc],loc=3)
    plt.text(.8, .8, r'$\mu=' + average + r',\ \sigma=' + std + r'$')

def drawSubplotsStocha():

    getPlotStocha()
    getPieChartStocha()
    plt.show()

def getStatsDeterm(method):
 
    f = open("./determ/" + method.upper() + "_stats_d", "r")
    lines = f.readlines()
    nums = []

    for i in lines:
        s = i.strip().split()[1]
    
        if s == '':
            continue

        nums.append(float(s))

    f.close()

    return nums

def getPercentagesDeterm(method):

    f = open("./determ/" + method.upper() + "_percentage_d", "r")
    lines = f.readlines()
    perc = dict()

    perc["other functions"] = 0.0

    for i in lines[-4:]:
        cols = i.split()
        perc[cols[0]] = float(cols[1])

    perc["other functions"] = 100.0 - sum(perc.values())

    f.close()

    return perc

def getPlotDeterm(method):

    nums = getStatsDeterm(method)
    x_axis = list(range(1, len(nums) + 1))

    average = str(numpy.average(nums))[0:9]
    std =  str(numpy.std(nums))[0:9]

    plt.subplot(121)
    plt.xlabel('n-th probe', fontsize=11)
    plt.ylabel('time [sec]', fontsize=11)
    plt.suptitle('Reading 150000 pseudorandom numbers from character device using ' + method + ' method')

    plt.grid(True)
    plt.plot(x_axis, nums, '-o')

def getPieChartDeterm(method):
    
    perc = getPercentagesDeterm(method)
    
    nums = getStatsDeterm(method)

    average = str(numpy.average(nums))[0:9]
    std =  str(numpy.std(nums))[0:9]
  
    plt.subplot(122)
    plt.pie([i for i in perc.values()], autopct='%1.1f%%', shadow=True, startangle=90)
    plt.legend([i for i in perc],loc=3)
    plt.text(.8, .8, r'$\mu=' + average + r',\ \sigma=' + std + r'$')

def drawSubplotsDeterm(method):

    getPlotDeterm(method)
    getPieChartDeterm(method)
    plt.show()

drawSubplotsDeterm("Sobol")
drawSubplotsDeterm("Halton")
drawSubplotsDeterm("LCG")

drawSubplotsStocha()
