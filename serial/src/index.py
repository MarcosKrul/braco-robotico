import roboticstoolbox as rtb

from startSerial import startSerial


if __name__ == '__main__':
  serial = startSerial()
  if serial == None:
    exit(1)
