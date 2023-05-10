from roboticstoolbox import DHRobot, RevoluteDH

from conversions import degress2radians

class AkeBabaRobot(DHRobot):
  
  def __init__(self, l1, l2, l3, print_robot=True):
    super().__init__(
      [
        self.__get_revolute_dh__(l1),
        self.__get_revolute_dh__(l2),
        self.__get_revolute_dh__(l3),
        RevoluteDH()
      ],
      name="AkeBaba robot",
      manufacturer="alarmouse solutions"
    )

    if print_robot:
      print("PARÂMETROS DO ROBÔ")
      print(self)


  def __get_revolute_dh__(self, link_params) -> RevoluteDH:
    return RevoluteDH(
      d=link_params['d'],
      a=link_params['a'],
      alpha=degress2radians(link_params['alpha']),
      qlim=[
        degress2radians(link_params['min']), 
        degress2radians(link_params['max'])
      ]
    )
