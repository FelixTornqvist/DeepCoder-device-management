from coapthon.client.helperclient import HelperClient
import coapthon.messages.response
from threading import Thread

class coap_adapter:
    port = 5683
    path = "test"
    ip_addr = ""
    connected = False

    def stop(self):
        print "(not actually) stopping CoAP", self.ip_addr
        self.client.stop()

    def __init__(self, ip):
        self.ip_addr = ip
        
        try:
            self.client = HelperClient(server=(self.ip_addr, self.port))
            response = self.client.get(self.path, timeout=0.2)
            if response:
                self.connected = True
                return

            print response.pretty_print() # ruin test data

        except Exception as e:
            raise e
        finally:
            if hasattr(self, 'client'):
                thread = Thread(target=lambda: self.client.stop())
                thread.start()
