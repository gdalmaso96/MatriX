import pylab
import numpy as np

x, y = np.loadtxt("SiPM_det_eff.txt", unpack=True)

pylab.plot(x, y)
pylab.show()
