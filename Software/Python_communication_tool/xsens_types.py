import struct
from enum import Enum


class Xsens_utcTime():
    def __init__(self) -> None:
        self.Ns = 0
        self.Year = 0
        self.Month = 0
        self.Day = 0
        self.Hour = 0
        self.Minute = 0
        self.Second = 0
        self.Flags = 0

    def update(self, data:bytes):
        [value] = struct.unpack('>I', data[0:4])
        self.Ns = value
        [value] = struct.unpack('>H', data[4:6])
        self.Year = value
        [value] = struct.unpack('>B', data[6:7])
        self.Month = value
        [value] = struct.unpack('>B', data[7:8])
        self.Day = value
        [value] = struct.unpack('>B', data[8:9])
        self.Hour = value
        [value] = struct.unpack('>B', data[9:10])
        self.Minute = value
        [value] = struct.unpack('>B', data[10:11])
        self.Second = value
        [value] = struct.unpack('>B', data[11:12])
        self.Flags = value


class Xsens_deltaV():
    def __init__(self) -> None:
        self.vX = 0
        self.vY = 0
        self.vZ = 0

    def update(self, data:bytes):
        [value] = struct.unpack('>f', data[0:4])
        self.vX = value
        [value] = struct.unpack('>f', data[4:8])
        self.vY = value
        [value] = struct.unpack('>f', data[8:12])
        self.vZ = value 

class Xsens_acceleration():
    def __init__(self) -> None:
        self.accX = 0
        self.accY = 0
        self.accZ = 0

    def update(self, data:bytes):
        [value] = struct.unpack('>f', data[0:4])
        self.accX = value
        [value] = struct.unpack('>f', data[4:8])
        self.accY = value
        [value] = struct.unpack('>f', data[8:12])
        self.accZ = value 


class Xsens_rateOfTurn():
    def __init__(self) -> None:
        self.gyrX = 0
        self.gyrY = 0
        self.gyrZ = 0

    def update(self, data:bytes):
        [value] = struct.unpack('>f', data[0:4])
        self.gyrX = value
        [value] = struct.unpack('>f', data[4:8])
        self.gyrY = value
        [value] = struct.unpack('>f', data[8:12])
        self.gyrZ = value 


class Xsens_deltaQ():
    def __init__(self) -> None:
        self.q1 = 0
        self.q2 = 0
        self.q3 = 0
        self.q4 = 0

    def update(self, data:bytes):
        [value] = struct.unpack('>f', data[0:4])
        self.q1 = value
        [value] = struct.unpack('>f', data[4:8])
        self.q2 = value
        [value] = struct.unpack('>f', data[8:12])
        self.q3 = value 
        [value] = struct.unpack('>f', data[12:16])
        self.q4 = value 


class Xsens_magneticField():
    def __init__(self) -> None:
        self.magX = 0
        self.magY = 0
        self.magZ = 0

    def update(self, data:bytes):
        [value] = struct.unpack('>f', data[0:4])
        self.magX = value
        [value] = struct.unpack('>f', data[4:8])
        self.magY = value
        [value] = struct.unpack('>f', data[8:12])
        self.magZ = value
