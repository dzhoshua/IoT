import time
import paho.mqtt.client as paho
import random


broker = 'broker.emqx.io'
client = paho.Client('client-isu-220')
print('Connecting to broker', broker)
client.connect(broker)
client.loop_start()

for _ in range(10):
  state = str(input())
  client.publish('zlata/zlata', state)
  time.sleep(random.randint(4, 10))

client.disconnect()
client.loop_stop()