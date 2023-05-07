import serial
from os import getenv


class SerialControl:

  def __init__(self) -> None:
    self.__serial = None
  
    port = getenv("SERIAL_COM")
    baud_rate = getenv("SERIAL_BAUD_RATE")

    print(f"Start serial at port {port} with baud rate {baud_rate}")
    
    try:
      self.__serial = serial.Serial(port, baud_rate)
    except Exception as err:
      print(f"Failed at start serial with {port}: {str(err)}")


  def serialAvailable(self) -> bool:
    return self.__serial != None
  

  def write(self, message) -> int:
    return self.__serial.write(message.encode())