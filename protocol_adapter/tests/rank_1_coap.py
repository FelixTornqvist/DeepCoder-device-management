import sys, os
sys.path.insert(1, os.path.join(sys.path[0], '..'))
import paho.mqtt.client as paho
from timeit import Timer
import coap_adapter as coap
import adapter_list


connected_adapters = {}
ada_useage = adapter_list.adapter_list([coap.coap_adapter])

failed = False 
def try_all_protocols():
    found = True
    ip = '134.102.218.18'

    for (_, init_func) in ada_useage.adapters:
        try:
            new_adapter = init_func(ip)
            while not new_adapter.connected:
                if new_adapter.connected:
                    found = True
                    break

        except Exception as e:
            global failed
            failed = True
            found = False

        if found:
            connected_adapters[str(init_func) + ip] = new_adapter
            ada_useage.add(init_func)
            break


t = Timer(lambda: try_all_protocols())
execution_time = t.timeit(number=1)
if failed:
    print "fail!"
else:
    print round(execution_time, 4)

for i in connected_adapters:
    connected_adapters[i].stop()
