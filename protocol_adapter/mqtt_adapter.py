import paho.mqtt.client as mqtt
import time as t
from threading import Thread

class mqtt_adapter:
    connections = {}
    client = mqtt.Client()
    ip_address = ""
    connected = False

    def forward(self, msg):
        print 'sends', msg.payload.decode(), 'to', self.connections.get(msg.topic)

    def create_sa(self, topic):
        print "asks Instatiator to instatiate for MQTT,", topic, ',', self.ip_address

    def register_sa(self, topic, sa):
        self.connections[topic] = sa

    def print_connections(self):
        print 'mqtt <' + str(self.ip_address) + '> topic connections to SAs:'
        for k in self.connections:
            print k, "->", self.connections.get(k)

    def stop(self):
        print "stopping SA adapter " + self.ip_address
        self.connected = False
        self.client.reinitialise()
        self.client.loop_stop()

    # Paho mqtt listener functions
    def on_connect(self, client, userdata, flags, rc):
        print '(MQTT) Connected with result code ' + str(rc) #ruin test data
        if rc == 0:
            self.connected = True
            self.client.subscribe('home/testing1')   # Get all sensor readings from the home/testing1

    def on_message(self, client, userdata, msg):
        if msg.topic in self.connections:
            self.forward(msg)
        else:
            self.create_sa(msg.topic)

    def __init__(self, ip_addr):
        self.ip_address = ip_addr
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message # ruin test data

        self.client.connect_async(self.ip_address, port=1883, keepalive=60)
        self.client.loop_start()
        
        timeout = t.time() + 0.8
        while self.connected != True and t.time() < timeout:
            t.sleep(0.01)

        if self.connected == False:
            self.stop()
            raise Exception
