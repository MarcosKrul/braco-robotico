import serial
from serial import Serial
from os import getenv
from dotenv import load_dotenv


def startSerial() -> Serial:
  load_dotenv()
  port = getenv("SERIAL_COM")
  baud_rate = getenv("SERIAL_BAUD_RATE")

  print(f"Start serial at port {port} with baud rate {baud_rate}")
  
  try:
    ser = serial.Serial(port, baud_rate)
    return ser
  except Exception as err:
    print(f"Failed at start serial with {port}: {str(err)}")

  return None