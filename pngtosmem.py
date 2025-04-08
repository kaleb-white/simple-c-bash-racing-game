"""A simple script to conver a 30x40 png file into array indices."""

import cv2, sys, os, re
from enum import Enum

if len(sys.argv) != 2:
  raise ValueError("Please give a frame number as the second arg!")


img = cv2.imread(f"./frames/frame{sys.argv[1]}.png", cv2.IMREAD_COLOR)


""" enum BackgroundColor
{
	bgBLACK = 40,
	bgRED = 41,
	bgGREEN = 42,
	bgORANGE = 43,
	bgBLUE = 44,
	bgPURPLE = 45,
	bgCYAN = 46,
	bgGREY = 47,
	bgGRAY = 47
} """

class BackgroundColor(Enum): # (b, g, r)
  bgBLACK = (0, 0, 0)
  bgRED = (0, 0, 255)
  bgGREEN = (0, 128, 0)
  bgORANGE = (0, 165, 255)
  bgBLUE = (255, 0, 0)
  bgPURPLE = (128, 0, 128)
  bgCYAN = (255, 255, 0)
  bgGREY = (77, 75, 61)


with open(f"smemframes/frame{sys.argv[1]}.mem", "w") as f:
  for i in range(30):
    for j in range(40):
      b,g,r = img[i][j]
      #debug print("at position x:%s and y: %s, blue:%s green:%s red:%s" % (j, i, b, g, r) + " line: " + str(j + 40*i + 1))
      match (b, g, r):
        case (0, 0, 0):
          f.write("0")
        case BackgroundColor.bgRED.value:
          f.write("1")
        case BackgroundColor.bgGREEN.value:
          f.write("2")
        case BackgroundColor.bgORANGE.value:
          f.write("3")
        case BackgroundColor.bgBLUE.value:
          f.write("4")
        case BackgroundColor.bgPURPLE.value:
          f.write("5")
        case BackgroundColor.bgCYAN.value:
          f.write("6")
        case BackgroundColor.bgGREY.value:
          f.write("7")
      f.write("\n")
