import roboticstoolbox as rtb
from dotenv import load_dotenv

from SerialControl import SerialControl


if __name__ == '__main__':
  load_dotenv()

  serialControl = SerialControl()
  if not serialControl.serialAvailable():
    exit(1)
