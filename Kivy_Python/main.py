from kivy.app import App
from kivy.lang import Builder
from kivy.clock import Clock
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.uix.scrollview import ScrollView
from kivy.uix.gridlayout import GridLayout
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.core.window import Window
from kivy.uix.popup import Popup
from kivy.uix.floatlayout import FloatLayout
from kivy.properties import ObjectProperty
from kivy.properties import StringProperty

import bluetooth

#GLOBAL VARIABLES
nearby_devices = []   #used for scanning pop up
scanning_flag = False #used for scanning pop up
sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
connected_device = "NO DEVICE"
connected_device_addr = None

#returns names and addresses of nearby devices
#UPDATE GLOBAL VARIABLE
def BT_scan_nearby():
    global scanning_flag
    global nearby_devices
    scanning_flag = True
    print("LOG: BT_scan_nearby called")
    nearby_devices = bluetooth.discover_devices(lookup_names = True)
    scanning_flag = False


#stolen code to make an expanding label
#check kv
class CustomLabel(Label):
    pass

#used to display all the found addresses so user can choose wheelson
class ScrollableLabel(ScrollView):

    def __init__(self, **kwargs):   
        super().__init__(**kwargs)

        self.size_hint_y = None

        self.layout = GridLayout(cols=1,size_hint_y=None)
        self.add_widget(self.layout)

        #scroll_to_point used as a way to scroll to most recent message
        self.scroll_to_point = Label()
        self.found_devices = CustomLabel(size_hint_y=None, markup=True)
        global connected_device

        #console information
        self.found_devices.text =  "CURRENTLY CONNECTED TO: " + connected_device
        self.found_devices.text += "\nSCAN FINISHED: \n DEVICE LIST:"

        self.layout.add_widget(self.found_devices)
        self.layout.add_widget(self.scroll_to_point)

        Clock.schedule_interval(self.BT_update_service, 2)

    #reset device list
    def clear_device_list(self):
        global connected_device
        self.found_devices.text =  "CURRENTLY CONNECTED TO: " + connected_device
        self.found_devices.text += "\nSCAN FINISHED: \n DEVICE LIST:"

    #adds items to view
    def update_device_list(self,name,addr,num):
        self.found_devices.text += "\ndevice number: "+ str(num) +"\n" + "name: " + name + "\n" + "addr: " + addr
        self.layout.height = self.found_devices.texture_size[1] + 15
        self.found_devices.height = self.found_devices.texture_size[1]
        self.found_devices.text_size = (self.found_devices.width *0.98,None)
        self.scroll_to(self.scroll_to_point)

    #perform bluetooth scan and then update the view
    #DO NOT CALL BT SCAN HERE
    def BT_update_service(self,_):
        count = 0
        #print("LOG: BT_update_service called")
        self.clear_device_list()
        self.found_devices.text +="\n****************************************"
        for addr,name in nearby_devices:
            self.update_device_list(name,addr,count)
            self.found_devices.text +="\n****************************************"
            count += 1
        self.found_devices.text += "\nENTER DEVICE NUMBER BELOW"
        self.found_devices.text += "\nMUST PAIR WITH DEVICE BEFORE CONNECTING"


'''
Name: MainWindow
Desc: right now displays the direction buttons for the robot
      and the current BT device it's connected to(should be a robot)

NOTE: commands are hardcoded right now
TODO: add commands to setting menu
'''
class MainWindow(Screen):
    global connected_device
    device_text = StringProperty(None)
    device_text = "Connected Device: " + connected_device
    device_label = ObjectProperty(None)

    def __init__(self,**kwargs):
        super().__init__(**kwargs)
        Clock.schedule_interval(self.update_device,1)

    def update_device(self,_):
        global connected_device
        #print(connected_device)
        self.device_text = "Connected Device: " + connected_device
        self.ids.device_label.text = self.device_text

    def send_foward(self):
        try:
            sock.send("W".encode("ascii"))
        except:
            pass

    def send_reverse(self):
        try:
            sock.send("S".encode("ascii"))
        except:
            pass
    
    def send_turn_right(self):
        try:
            sock.send("D".encode("ascii"))
        except:
            pass

    def send_turn_left(self):
        try:
            sock.send("A".encode("ascii"))
        except:
            pass

    def send_stop(self):
        try:
            sock.send("E")
        except:
            pass


class SettingsWindow(Screen):
    pass 


class ScanPopup(FloatLayout):
    pass


'''
Name: Scanner Page
Desc: Label for found scanner devices
'''
class ScannerPage(GridLayout):
    def __init__(self,**kwargs):
        super().__init__(**kwargs)
        self.cols = 1

        #scroll label
        self.devices = ScrollableLabel(height = Window.size[1] * 0.9,size_hint_x = 0.8)
        self.add_widget(self.devices)
        

'''
Name: ScannerWindow
Desc: user interface for displaying 
      local bluetooth devices and connecting to 
      target device
NOTE: hitting scan will freeze gui(the discover function is slow)
TODO: add popup displaying that a scan is occuring and the app hasn't 
      crashed
'''
class ScannerWindow(Screen):

    text_input = ObjectProperty(None)

    #function used to connect to the actual device
    def connect_to_device(self):
        global sock
        global connected_device
        global nearby_devices
        print(nearby_devices)
        
        #try to connect to device
        #this should work unless bad user input
        try:
            addr,name = nearby_devices[int(self.text_input.text)]
            connected_device = name
            sock.connect((addr,1)) #just use port 1
        except:
            connected_device = "INVALID DEVICE: NO SERVICE"

        print(self.text_input.text)
     
    #scans for nearby BT devices so the user can 
    #connect to the devices
    def BT_scan_nearby(self):
        global nearby_devices
        global scanning_flag
        scanning_flag = True
        print("LOG: BT_scan_nearby called")
        nearby_devices = bluetooth.discover_devices(lookup_names = True)
        scanning_flag = False

#just needed to manage windows
class WindowManager(ScreenManager):
    pass

#import kv file
kv = Builder.load_file("my.kv")
class MyMainApp(App):
    def build(self):
        return kv

if __name__ == "__main__":
    BT_scan_nearby()
    MyMainApp().run()