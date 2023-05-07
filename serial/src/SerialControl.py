import serial

class SerialControl:

  def __init__(self, configs) -> None:
    self.__serial = None
  
    port = configs['port']
    baud_rate = configs['baud_rate']
  
    print(f"Start serial at port {port} with baud rate {baud_rate}")
    
    try:
      self.__serial = serial.Serial(port, baud_rate)
    except Exception as err:
      print(f"Failed at start serial with {port}: {str(err)}")


  def serialAvailable(self) -> bool:
    return self.__serial != None
  

  def write(self, message) -> int:
    return self.__serial.write(message.encode())