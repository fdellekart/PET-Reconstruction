#!/usr/bin/env python3
"""Utility script to display images saved in a very basic CSV format

First and only parameter is the path to the file
"""
import sys

import numpy as np
import matplotlib.pyplot as plt

with open(sys.argv[1]) as f:
    datastr = f.read()

data = np.array([substr.split(",") for substr in datastr.split("\n")][:-1]).astype(np.float64)

plt.imshow(data)
plt.show()
