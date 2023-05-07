import roboticstoolbox as rtb

from Configs import Configs
from SerialControl import SerialControl

if __name__ == '__main__':
  configs = Configs()

  serialControl = SerialControl(configs=configs.getProperty('serial'))
  if not serialControl.serialAvailable():
    exit(1)
