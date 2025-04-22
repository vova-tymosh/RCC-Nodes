import time, re
from infra import *
import paho.mqtt.client as mqtt
from paho.mqtt.subscribeoptions import SubscribeOptions

MQ_MESSAGE = re.compile("cab/(.*?)/(.*)")
MQ_PREFIX = "cab"
MQ_INTRO = "intro"
MQ_SET_THROTTLE = "throttle";
MQ_SET_DIRECTION = "direction";
MQ_HEARTBEAT_VALUES = "heartbeat/values"
MQ_GET_FUNCTION = "function/get"
MQ_SET_FUNCTION = "function/"
MQ_GET_VALUE = "value/get"
MQ_LIST_VALUE_ASK = "value/list"
MQ_LIST_VALUE_RES = "keys"
MQ_SET_VALUE = "value/"

MQ_DIRECTIONS = ["REVERSE", "FORWARD", "STOP", "NEUTRAL"]
MQ_ON = "ON"
MQ_OFF = "OFF"

MQTT_NODE_NAME = 'RCC_Test'
MQTT_BROKER = '192.168.20.61'

ADDR = 3


class TransportMqtt:
    def __init__(self):
        self.mqttClient = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, MQTT_NODE_NAME)
        self.mqttClient.on_message = self.onReceive
        self.received = ["", ""]

    def start(self):
        self.mqttClient.connect(MQTT_BROKER)
        options = SubscribeOptions(qos = 1, noLocal = True)
        self.mqttClient.subscribe(f'{MQ_PREFIX}/#', options=options)
        #self.mqttClient.loop_forever()

    def loop(self):
        for i in range(5):
            self.mqttClient.loop(timeout=1.0)
        return self.received

    def write(self, topic, message, retain = False):
        logging.info(f"[MQ] >: {topic} {message}")
        self.cache = f"{topic} {message}"
        self.mqttClient.publish(topic, message, retain)

    def onReceive(self, client, userdata, msg):
        topic = MQ_MESSAGE.match(msg.topic)
        if (topic is None):
            return
        message = str(msg.payload, 'UTF-8')

        logging.info(f"[MQ] <: {msg.topic} {message}")
        addr, action = topic.groups()
        self.received = (msg.topic, message)


mq = TransportMqtt()


def test_mqtt_start(s):
    mq.start()
    return (True, 'Test MQ Setup')


    
def test_throttle(s):
    test_name = 'Test MQ Throttle'
    t = f'cab/{ADDR}/throttle'
    m = '63'
    mq.write(t, m)
    mq.loop()
    t = f'cab/{ADDR}/value/get'
    m = 'Throttle'
    mq.write(t, m)
    rt, rm = mq.loop()
    logging.warning(f"  ###: {rt} {rm}")
    testResult = (rt == f'cab/{ADDR}/value/Throttle') and (rm == '63')

    return (testResult, test_name)
