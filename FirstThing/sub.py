import time
import paho.mqtt.client as paho
import random
import led_serial as ls

broker = "broker.emqx.io"

def on_message(client, userdata, message):
    time.sleep(1)
    data = str(message.payload.decode("utf-8"))
    print("recieved message = ", data)
    if data == "u":
        ls.send_command("u", 6)
    elif data == "d":
        ls.send_command("d", 7)


client = paho.Client("client-isu-002")
client.on_message=on_message

print("Connecting to broker", broker)
client.connect(broker)
client.loop_start()
print("Subscribing")
client.subscribe("zlata/zlata")
time.sleep(100)
client.disconnect()
client.loop_stop()
