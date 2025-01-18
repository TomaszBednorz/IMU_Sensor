from turtle import home
from xsens_types import Xsens_utcTime, Xsens_deltaV, Xsens_acceleration, Xsens_rateOfTurn, Xsens_deltaQ, Xsens_magneticField
import struct
from math import floor

Xsens_Temperature = 0
Xsens_UtcTime = 1
Xsens_PacketCounter = 2
Xsens_SampleTimeFine = 3
Xsens_SampleTimeCoarse = 4
Xsens_DeltaV = 5
Xsens_Acceleration = 6
Xsens_AccelerationHR = 7
Xsens_RateOfTurn = 8
Xsens_DeltaQ = 9
Xsens_RateOfTurnHR = 10
Xsens_MagneticField = 11
Xsens_StatusByte = 12
Xsens_StatusWord = 13
Xsens_UserRtcTime = 14
Xsens_NB = 15

class Xsens():
    def __init__(self) -> None:
        self.Temperature = 0
        self.UtcTime = Xsens_utcTime()
        self.PacketCounter = 0
        self.SampleTimeFine = 0
        self.SampleTimeCoarse = 0
        self.DeltaV = Xsens_deltaV()
        self.Acceleration = Xsens_acceleration()
        self.AccelerationHR = Xsens_acceleration()
        self.RateOfTurn = Xsens_rateOfTurn()
        self.DeltaQ = Xsens_deltaQ()
        self.RateOfTurnHR = Xsens_rateOfTurn()
        self.MagneticField = Xsens_magneticField()
        self.StatusByte = 0
        self.StatusWord = 0
        self.UserRtcTime = 0

        self.DataStatus = [False] * Xsens_NB

        self.Log = ""

    def processData(self, data: bytes):
        
        length = data[1]
        iter = 2

        while length > 0:
            packet_group = data[iter]
            iter += 1
            length -= 1

            packet_type = data[iter]
            iter += 1
            length -= 1
            packet_len = data[iter]
            iter += 1
            length -= 1
            self._updateData(packet_group, packet_type, data[iter:(iter+packet_len)])

            iter += (packet_len)
            length -= packet_len

    def getLog(self, type):
        self.Log = ""
        for idx, status in enumerate(self.DataStatus):
            if status == True:
                if type == 0:
                    self._updateLog(idx)
                self.DataStatus[idx] = False
        if type == 1:
            self._updateRawLog()
        return self.Log


    def _updateData(self, group, type, data:bytes):
        if group == 0X08:  # Temperature
            if type == 0X10:
                [value] = struct.unpack('>f', data)
                self.Temperature = value
                self.DataStatus[Xsens_Temperature] = True
        elif group == 0X10:  # Timestamp
            if type == 0X10:
                self.UtcTime.update(data)
                self.DataStatus[Xsens_UtcTime] = True
            elif type == 0X20: 
                [int_val] = struct.unpack('>H', data)
                self.PacketCounter = int_val
                self.DataStatus[Xsens_PacketCounter] = True
            elif type == 0X60:  
                [int_val] = struct.unpack('>I', data)
                self.SampleTimeFine = int_val
                self.DataStatus[Xsens_SampleTimeFine] = True
            elif type == 0X70:
                [int_val] = struct.unpack('>I', data)
                self.SampleTimeCoarse = int_val
                self.DataStatus[Xsens_SampleTimeCoarse] = True
        elif group == 0X40:  # Acceleration
            if type == 0X10:
                self.DeltaV.update(data)
                self.DataStatus[Xsens_DeltaV] = True
            elif type == 0X20:
                self.Acceleration.update(data)
                self.DataStatus[Xsens_Acceleration] = True
            elif type == 0X40:
                self.AccelerationHR.update(data)
                self.DataStatus[Xsens_AccelerationHR] = True
        elif group == 0X80:  # Angular Velocity
            if type == 0X20:
                self.RateOfTurn.update(data)
                self.DataStatus[Xsens_RateOfTurn] = True
            elif type == 0X30:
                self.DeltaQ.update(data)
                self.DataStatus[Xsens_DeltaQ] = True
            elif type == 0X40:
                self.RateOfTurnHR.update(data)
                self.DataStatus[Xsens_RateOfTurnHR] = True
        elif group == 0XC0:  # Magnetic
            if type == 0X20:
                self.MagneticField.update(data)
                self.DataStatus[Xsens_MagneticField] = True
        elif group == 0xE0:  # Status
            if type == 0X10:
                [int_val] = struct.unpack('>B', data)
                self.StatusByte = int_val
                self.DataStatus[Xsens_StatusByte] = True
            elif type == 0X20:
                [int_val] = struct.unpack('>I', data)
                self.StatusWord = int_val
                self.DataStatus[Xsens_StatusWord] = True
        elif group == 0Xf0:  # RTC Time
            if type == 0X10:
                [value] = struct.unpack('>I', data)
                self.UserRtcTime = value
                self.DataStatus[Xsens_UserRtcTime] = True
        
    def _updateLog(self, idx):
        if(idx == Xsens_Temperature):
            self.Log += "Temperature: {}°C\n".format(self.Temperature)
        elif(idx == Xsens_UtcTime):
            self.Log += "Year: {}, Month: {}, Day: {}, Hour: {}, Minute: {}, Seconds: {}, Ns: {}, Flags: {}\n".format(self.UtcTime.Year, self.UtcTime.Month, self.UtcTime.Day,
                                                                                self.UtcTime.Hour, self.UtcTime.Minute, self.UtcTime.Second, self.UtcTime.Ns, self.UtcTime.Flags)
        elif(idx == Xsens_PacketCounter):
            self.Log += "PacketCounter: {}\n".format(self.PacketCounter)
        elif(idx == Xsens_SampleTimeFine):
            currentTime = self.SampleTimeFine / 10000
            self.Log += ("Timestamp: %.4fs\n" % currentTime)
        elif(idx == Xsens_SampleTimeCoarse):
            self.Log += "Timestamp: {}s\n".format(self.SampleTimeCoarse)
        elif(idx == Xsens_DeltaV):
            self.Log += "DeltaV: vX: {:.4f} [m/s], vY: {:.4f} [m/s], vZ: {:.4f} [m/s]\n".format(self.DeltaV.vX, self.DeltaV.vY, self.DeltaV.vZ)
        elif(idx == Xsens_Acceleration):
            self.Log += "Acceleration: accX: {:.4f} [m/s2], accY: {:.4f} [m/s2], accZ: {:.4f} [m/s2]\n".format(self.Acceleration.accX, self.Acceleration.accY, self.Acceleration.accZ)
        elif(idx == Xsens_AccelerationHR):
            self.Log += "AccelerationHR: accX: {:.4f} [m/s2], accY: {:.4f} [m/s2], accZ: {:.4f} [m/s2]\n".format(self.AccelerationHR.accX, self.AccelerationHR.accY, self.AccelerationHR.accZ)
        elif(idx == Xsens_RateOfTurn):
            self.Log += "RateOfTurn: gyrX: {:.4f} [rad/s], gyrY: {:.4f} [rad/s], gyrZ: {:.4f} [rad/s]\n".format(self.RateOfTurn.gyrX, self.RateOfTurn.gyrY, self.RateOfTurn.gyrZ)
        elif(idx == Xsens_DeltaQ):
            self.Log += "DeltaQ: q1: {:.4f}, q2: {:.4f}, q3: {:.4f}, q4: {:.4f}\n".format(self.DeltaQ.q1, self.DeltaQ.q2, self.DeltaQ.q3, self.DeltaQ.q4)
        elif(idx == Xsens_RateOfTurnHR):
            self.Log += "RateOfTurnHR: gyrX: {:.4f} [rad/s], gyrY: {:.4f} [rad/s], gyrZ: {:.4f} [rad/s]\n".format(self.RateOfTurnHR.gyrX, self.RateOfTurnHR.gyrY, self.RateOfTurnHR.gyrZ)
        elif(idx == Xsens_MagneticField):
            self.Log += "MagneticField: magX: {:.4f}, magY: {:.4f}, magZ: {:.4f}\n".format(self.MagneticField.magX, self.MagneticField.magY, self.MagneticField.magZ)
        elif(idx == Xsens_StatusByte):
            self.Log += "StatusByte: {}\n".format(self.StatusByte)
        elif(idx == Xsens_StatusWord):
            self.Log += "StatusWord: {}\n".format(self.StatusWord)
        elif(idx == Xsens_UserRtcTime):
            currentTime = self.UserRtcTime / 10000
            self.Log += ("UserRtcTime: %.4fs\n" % currentTime)

    def _updateRawLog(self):
        # timestamp, accX, accY, accZ, gyrX, gyrY, gyrZ, magX, magY, magZ
        currentTime = self.UserRtcTime / 10000
        self.Log += ("%.4f," % currentTime)
        self.Log += ("%.4f," % self.Acceleration.accX)
        self.Log += ("%.4f," % self.Acceleration.accY)
        self.Log += ("%.4f," % self.Acceleration.accZ)
        self.Log += ("%.4f," % self.RateOfTurn.gyrX)
        self.Log += ("%.4f," % self.RateOfTurn.gyrY)
        self.Log += ("%.4f," % self.RateOfTurn.gyrZ)
        self.Log += ("%.4f," % self.MagneticField.magX)
        self.Log += ("%.4f," % self.MagneticField.magY)
        self.Log += ("%.4f" % self.MagneticField.magZ)
