API_KEY = "3495b03e-6d9d-441d-8f42-00c47bd81033"

import json
import ssl

import paho.mqtt.client as mqtt

host = "mqtt.cloud.pozyxlabs.com"
port = 443
topic = "5aa3ab594e136e0005b1cdd6"
username = "5aa3ab594e136e0005b1cdd6"
password = "3495b03e-6d9d-441d-8f42-00c47bd81033"

ip = "127.0.0.1"  # IP for the OSC UDP
network_port = 8888  # network port for the OSC UDP


def on_connect(client, userdata, flags, rc):
    print(mqtt.connack_string(rc))


def on_message(client, userdata, msg):
    print("Received message")
    tag_data = json.loads(msg.payload.decode())
    print(tag_data)
    with open("data-log.txt", "a") as file_pointer:
        for tag in tag_data:
            try:
                network_id = tag["tagId"]
                position = tag["data"]["coordinates"]
                file_pointer.write("{},{},{},{}".format(network_id, position["x"], position["y"], position["z"]))
            except Exception as e:
                print("Error using tag data, {}".format(e))
    # print("Positioning update:", msg.payload.decode())
    # print(coordinates)


def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed to topic!")


client = mqtt.Client(transport="websockets")

client.username_pw_set(username, password=password)

client.tls_set_context(context=ssl.create_default_context())

client

client.on_connect = on_connect

client.on_message = on_message
client.on_subscribe = on_subscribe

client.connect(host, port=port)
client.subscribe(topic)

client.loop_forever()
