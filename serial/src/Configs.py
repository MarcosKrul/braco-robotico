import os
import sys
import yaml

class Configs():

  def __init__(self) -> None:
    self.__fileName = os.path.join(sys.prefix, "..", "config", "configs.yaml")

    try:
      with open(self.__fileName, "rt", encoding="utf-8") as file:
        self.__config = yaml.safe_load(file)
    except IOError as err:
      print(f"Error at open yaml config file: {str(err)}")


  def getProperty(self, path):
    config = self.__config

    for prop in path.split('.'):
      config = config[prop]

    return config