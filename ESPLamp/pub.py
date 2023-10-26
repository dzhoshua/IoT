
import time
import paho.mqtt.client as paho
//import questionary


broker = 'broker.emqx.io'
client = paho.Client('client-isu-004')
print('Connecting to broker', broker)
client.connect(broker)
client.loop_start()
print("Publishing")

//topic = questionary.text("What is your topic?").ask()
topic = "esp8266/zlataveronika"
print(topic)

i = 0
lim_time = 20
while True:
    client.publish(topic, "0")
    print("off")
    time.sleep(20)
    client.publish(topic, "1")
    print("on")
    time.sleep(lim_time - i)
    client.publish(topic, "0")
    print("off")
    time.sleep(lim_time + i)
    i += 1
    if i > 10:
        i = 0

client.disconnect()
client.loop_stop()
