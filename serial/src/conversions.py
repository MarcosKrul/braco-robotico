from math import pi


def degress2radians(degress: float) -> float:
  return degress * (pi / 180)


def radians2degress(radians: float) -> float:
  return radians * (180 / pi)