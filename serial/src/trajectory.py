import time
import numpy as np
from spatialmath import SE3

from Configs import Configs
from SerialControl import SerialControl
from AkeBabaRobot import AkeBabaRobot
from conversions import radians2degress

if __name__ == "__main__":
  configs = Configs()

  robot = AkeBabaRobot(
    l1=configs.getProperty("l1"),
    l2=configs.getProperty("l2"),
    l3=configs.getProperty("l3"),
  )

  serialControl = SerialControl(configs=configs.getProperty("serial"))
  if not serialControl.serialAvailable():
    exit(1)

  initialPoint = list(map(float, str(input("Ponto inicial: ")).split(",")))
  finalPoint = list(map(float, str(input("Ponto final: ")).split(",")))

  initialFrame = SE3.Trans(initialPoint) * SE3.OA([0, 1, 0], [0, 0, 1])
  initialSolution = robot.ikine_LM(initialFrame, ilimit=1000)
  
  finalFrame = SE3.Trans(finalPoint) * SE3.OA([0, 1, 0], [0, 0, 1])
  finalSolution = robot.ikine_LM(finalFrame, ilimit=1000)

  step = 50
  pos = []
  inc = []
  current = []

  for i in range(0, 4):
    inc.append((finalSolution.q[i] - initialSolution.q[i]) / step)
    current.append(initialSolution.q[i])

  for i in range(step):
    current = [
      current[0] + inc[0],
      current[1] + inc[1],
      current[2] + inc[2],
      current[3] + inc[3]
    ]
    pos.append([current[0], current[1], current[2], current[3]])

  print("=== FRAME INICIAL DA TRAJETÓRIA - ESPERADO ==="); print(initialFrame)
  print("=== FRAME INICIAL DA TRAJETÓRIA - OBTIDO ===");   print(robot.fkine(initialSolution.q))
  print("=== FRAME FINAL DA TRAJETÓRIA - ESPERADO ===");   print(finalFrame)
  print("=== FRAME FINAL DA TRAJETÓRIA - OBTIDO ===");     print(robot.fkine(finalSolution.q))
  print("=== ÂNGULOS INICIAIS DA TRAJETÓRIA ===");         print(radians2degress(initialSolution.q))
  print("=== ÂNGULOS FINAIS DA TRAJETÓRIA ===");           print(radians2degress(finalSolution.q))

  serialControl.write(
    "{:.2f},{:.2f},{:.2f}".format(
      radians2degress(initialSolution.q[0]),
      radians2degress(initialSolution.q[1]),
      radians2degress(initialSolution.q[2])
    )
  )
  print(serialControl.read())

  time.sleep(5)

  serialControl.write(
    "{:.2f},{:.2f},{:.2f}".format(
      radians2degress(finalSolution.q[0]),
      radians2degress(finalSolution.q[1]),
      radians2degress(finalSolution.q[2])
    )
  )
  print(serialControl.read())
    
  traj = np.array(pos)
  robot.plot(traj).hold()