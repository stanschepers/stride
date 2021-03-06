#################################################################
# Script to obtain the infection count for a range of R0 values #
#################################################################

import time

import pystride
from pystride.PyRunner import PyRunner
from pystride.PyController import PyController

t_start = time.mktime(time.localtime())

# Configure simulation
controller = PyController("../config/run_default.xml")
controller.runConfig.setParameter("output_prefix", "R0Runs")
controller.runConfig.setParameter("num_days", "3")

# Create forks with different values for R0
r0_values = [0, 4, 8]
for r0 in r0_values:
    # Create fork
    fork = controller.fork("r0_" + str(r0))
    fork.runConfig.setParameter("r0", r0)

# Run all forks
controller.controlForks()

for fork in controller.forks:
    num_infected = fork.runner.getSimulator().GetPopulation().GetInfectedCount()
    r0 = fork.runConfig.getParameter("r0")
    print("R0 = {0:2d} : infection count = {1:7d}".format(int(r0), num_infected))

t_elapsed = time.mktime(time.localtime()) - t_start
print("Total time elapsed: " + str(round(t_elapsed)) + " seconds")
