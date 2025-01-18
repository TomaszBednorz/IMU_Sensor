from re import X
import socket
from _thread import *
import sys
from xmlrpc.client import SafeTransport
from PyQt5.QtWidgets import (QWidget, QApplication, QLabel, QHBoxLayout, QVBoxLayout, QTextEdit, QCheckBox, QPushButton)
from PyQt5.QtCore import QTimer
from copy import copy



from xsens import Xsens


class XsensClient():
    def __init__(self, _ip: str, _clientHandler):
        self.ip = _ip
        self.clientHandler = _clientHandler
        self.lastLog = ""
        self.fileHandler = None


# Configuration variables
host = "169.254.240.83"
port = 5006

# Global variables
XsensClients = {}  # IP : XsensClient()
typeOfData = 0  # 0 - raw data, 1 = enhanced data (displayed)

# Parameters
XsensReset = 128
XsensChangeConfig = 129

# QT WINDOW

class Window(QWidget):
    def __init__(self):
        super().__init__()

        self.outputLogs = {}  # IP : QTextEdit()
        self.pauseDisplay = False
        self.freqDisplay = 10

        self.setWindowTitle("Xsens CDK")
        self.setGeometry(50, 100, 1800, 800)

        self.initUI()

        # Connections
        self.updateConfigButton.clicked.connect(self.updateConfig)
        self.synchronizeSensorsButton.clicked.connect(self.synchronizeSensors)
        self.pauseButton.clicked.connect(self.pause)
        self.clearButton.clicked.connect(self.clear)
        self.freqButton.clicked.connect(self.freq)
        self.dataTypeButton.clicked.connect(self.changeDataType)

        # Timers
        self.getDataTimer = QTimer()
        self.getDataTimer.timeout.connect(self.getData)
        self.getDataTimer.start(100)

        self.updateUiTimer = QTimer()
        self.updateUiTimer.timeout.connect(self.updateUi)
        self.updateUiTimer.start(100)


    def initUI(self):
        self.mainLayout = QVBoxLayout()

        self.configLayout1 = QHBoxLayout()
        self.configLayout2 = QHBoxLayout()
        self.ipLayout = QHBoxLayout()
        self.measurementsLayout = QHBoxLayout()

        checkBoxTxt = ["Temperature", "UtcTime", "PacketCounter", "SampleTimeFine", "SampleTimeCoarse", "DeltaV", "Acceleration",
                        "AccelerationHR", "RateOfTurn", "DeltaQ", "RateOfTurnHR", "MagneticField", "StatusByte", "StatusWord"]
        self.configCheckBoxes = []
        for txt in checkBoxTxt:
            checkB = QCheckBox(txt)
            self.configCheckBoxes.append(checkB)
            self.configLayout1.addWidget(checkB)

        self.updateConfigButton = QPushButton("Update config")
        self.configLayout2.addWidget(self.updateConfigButton)

        self.synchronizeSensorsButton = QPushButton("Synchronize sensors")
        self.configLayout2.addWidget(self.synchronizeSensorsButton)

        self.clearButton = QPushButton("Clear")
        self.configLayout2.addWidget(self.clearButton)

        self.pauseButton = QPushButton("Pause")
        self.configLayout2.addWidget(self.pauseButton)

        self.freqButton = QPushButton("10 Hz")
        self.configLayout2.addWidget(self.freqButton)

        self.dataTypeButton = QPushButton("Enhanced data")
        self.configLayout2.addWidget(self.dataTypeButton)

        self.mainLayout.addLayout(self.configLayout1)
        self.mainLayout.addLayout(self.configLayout2)
        self.mainLayout.addLayout(self.ipLayout)
        self.mainLayout.addLayout(self.measurementsLayout)
        self.setLayout(self.mainLayout)
        self.show()

    def getData(self):
        global XsensClients

        for ip, client in XsensClients.items():
            if ip in self.outputLogs:
                if client.lastLog != "":
                    if self.pauseDisplay == False:
                        self.outputLogs[ip].append(client.lastLog)
                        client.lastLog = ""

    def updateUi(self):
        global XsensClients  # self.outputLogs    /  IP : QTextEdit()


        if(len(XsensClients) > len(self.outputLogs)):
            for ip in XsensClients:
                if ip not in self.outputLogs:
                    ipLabel = QLabel("Sensor {}".format(ip))
                    self.ipLayout.addWidget(ipLabel)
                    dataList = QTextEdit("")
                    dataList.setReadOnly(True)
                    self.outputLogs[ip] = dataList
                    self.measurementsLayout.addWidget(dataList)
        elif(len(XsensClients) < len(self.outputLogs)):
            pass

    def updateConfig(self) -> None:
        global XsensClients

        newConfig = [XsensChangeConfig]
        for cb in self.configCheckBoxes:
            cbState = 1 if cb.checkState() else 0
            newConfig.append(cbState)

        for _, client in XsensClients.items():
            client.clientHandler.send(bytes(newConfig))

    def synchronizeSensors(self) -> None:
        global XsensClients

        synchronizeMsg = bytes([XsensReset])
            
            
        for _, client in XsensClients.items():
            client.clientHandler.send(synchronizeMsg)

        for ip, client in XsensClients.items():  # Clear log files
            info_txt = "timestamp, accX, accY, accZ, gyrX, gyrY, gyrZ, magX, magY, magZ"
            client.fileHandler.truncate(0)
            client.fileHandler.seek(0, 0)
            client.fileHandler.write(info_txt + '\n')

            if ip in self.outputLogs:
                if self.pauseDisplay == False:
                    self.outputLogs[ip].append('\n' + info_txt)

    def pause(self) -> None:
        if self.pauseDisplay == False:
            self.pauseDisplay = True
            self.pauseButton.setText("Resume")
        else:
            self.pauseDisplay = False
            self.pauseButton.setText("Pause")

    def clear(self) -> None:
        for ip, _ in XsensClients.items():
            if ip in self.outputLogs:
                self.outputLogs[ip].clear()

    def freq(self) -> None:
        if self.freqDisplay == 100:
            self.freqButton.setText("1 Hz")
            self.freqDisplay = 1
            self.getDataTimer.start(1000)
        elif self.freqDisplay == 10:
            self.freqButton.setText("100 Hz")
            self.freqDisplay = 100
            self.getDataTimer.start(10)
        elif self.freqDisplay == 1:
            self.freqButton.setText("10 Hz")
            self.freqDisplay = 10
            self.getDataTimer.start(100)

    def changeDataType(self) -> None:
        global typeOfData, XsensClients
        info_txt = "timestamp, accX, accY, accZ, gyrX, gyrY, gyrZ, magX, magY, magZ"
        if typeOfData == 0:
            typeOfData = 1
            self.dataTypeButton.setText("Raw data")
            for ip, _ in XsensClients.items():
                if ip in self.outputLogs:
                    if self.pauseDisplay == False:
                        self.outputLogs[ip].append("\n" + info_txt)

        elif typeOfData == 1:
            typeOfData = 0
            self.dataTypeButton.setText("Enhanced data")

# TCP COMMUNICATION

def client_handler(connection, address):
    global XsensClients, typeOfData

    filename = "Log_{}.txt".format(address)
    f = open(filename, "w+")

    XsensClients[address].fileHandler = f
    XsensCDK = Xsens()

    while True:
        data = connection.recv(1024)

        try:
            XsensCDK.processData(data)
            dataLog = XsensCDK.getLog(typeOfData)
            XsensClients[address].lastLog = dataLog
            f.write(dataLog + "\n")
        except:
            print("An exception occurred")
    connection.close()

def accept_connections(ServerSocket):
    Client, address = ServerSocket.accept()
    print('Connected to: ' + address[0] + ':' + str(address[1]))
    new_client = XsensClient(address[0], Client)
    XsensClients[address[0]] = new_client
    start_new_thread(client_handler, (Client, address[0],))

def start_server(host, port):
    ServerSocket = socket.socket()

    try:
        ServerSocket.bind((host, port))
    except socket.error as e:
        print(str(e))
    print(f'Server is listing on the port {port}...')
    ServerSocket.listen(5)

    while True:
        accept_connections(ServerSocket)


# MAIN

if __name__ == "__main__":
    app = QApplication(sys.argv)
    ex = Window()

    start_new_thread(start_server, (host, port,))

    sys.exit(app.exec_())
