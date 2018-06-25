import mqtt_adapter as mqtt
import coap_adapter as coap
import adapter_list
import paho.mqtt.client as paho
import time

connected_adapters = {}
ada_useage = adapter_list.adapter_list([])
ada_useage.add(coap.coap_adapter)
ada_useage.add(coap.coap_adapter)
ada_useage.add(mqtt.mqtt_adapter)

def try_all_protocols(ip):
    found = True

    for (_, init_func) in ada_useage.adapters:
        if str(init_func) + ip in connected_adapters:
            print "Broker already registered"
            return

    for (_, init_func) in ada_useage.adapters:
        try:
            new_adapter = init_func(ip)
            found = True

        except Exception as e:
            print "Unable to connect using", init_func
            print "Connection exception:", e
            found = False

        if found:
            connected_adapters[str(init_func) + ip] = new_adapter
            ada_useage.add(init_func)
            print 'Connected'
            break


in_str = ''
while in_str != 'end':
    print
    in_str = raw_input('>')
    args = in_str.split(' ')
    
    if args[0] == 'end':
        break

    elif args[0] == 'listSA':
        key = args[1]
        connected_adapters[key].print_connections()

    elif args[0] == 'addSA':
        key = args[1]
        connected_adapters[key].register_sa(args[2], args[3])

    elif args[0] == 'listBrokers':
        print "Connected brokers:"
        for i in connected_adapters:
            print i, connected_adapters[i]
        
        print "\nUseage of adapters:"
        for (use, adapter) in ada_useage.adapters:
            print use, adapter

    elif args[0] == 'addBroker':
        try_all_protocols(args[1])
        
for i in connected_adapters:
    print i
    connected_adapters[i].stop()
    print '\n'
quit()
