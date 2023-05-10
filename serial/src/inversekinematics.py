from spatialmath import SE3
from prettytable import PrettyTable

from Configs import Configs
from SerialControl import SerialControl
from conversions import radians2degress, degress2radians
from AkeBabaRobot import AkeBabaRobot

if __name__ == '__main__':
  configs = Configs()

  robot = AkeBabaRobot(
    l1=configs.getProperty('l1'),
    l2=configs.getProperty('l2'),
    l3=configs.getProperty('l3'),
  )

  serialControl = SerialControl(configs=configs.getProperty('serial'))
  if not serialControl.serialAvailable():
    exit(1)

  console_table = PrettyTable()
  console_table.field_names = ["Aceitou?", "Iterações", "Ponto", "Ângulos"]
  
  while True:
    input_string = str(input("Ponto: "))
    input_values = list(map(float, input_string.split(",")))
    x, y, z = input_values

    frame = SE3([x, y, z])
    print(frame)

    inverse_kinematics_result = robot.ikine_min(frame, ilimit=100, qlim=True)
    
    console_table.add_row([
      inverse_kinematics_result.success,
      inverse_kinematics_result.iterations,
      "{}".format(input_string),
      "{:.2f},{:.2f},{:.2f}".format(
        radians2degress(inverse_kinematics_result.q[0]), 
        radians2degress(inverse_kinematics_result.q[1]), 
        radians2degress(inverse_kinematics_result.q[2]
      )),
    ])

    print("RESULTADO DA CINEMÁTICA INVERSA")
    print(console_table)

    serialControl.write(input_string)
    print(serialControl.read())
