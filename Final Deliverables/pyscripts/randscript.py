import paho.mqtt.client as mqtt
import time
import random
import json
import sms
import requests


def run():
    ORG ="q6sux6"
    DEVICE_TYPE ="ESP32"
    DEVICE_ID ="GokulEsp32"
    TOKEN ="##############" 

    server = ORG + ".messaging.internetofthings.ibmcloud.com";
    pubTopic1 = "iot-2/evt/temp/fmt/json"
    pubTopic2 = "iot-2/evt/pH/fmt/json"
    pubTopic3 = "iot-2/evt/turb/fmt/json"
    #pubTopic3 = "iot-2/evt/wf/fmt/json"

    authMethod = "use-token-auth";
    token = TOKEN;
    clientId = "d:" + ORG + ":" + DEVICE_TYPE + ":" + DEVICE_ID;

    mqttc = mqtt.Client(client_id=clientId)
    mqttc.username_pw_set(authMethod, token)
    mqttc.connect(server, 1883, 60)

    while True:
        try:

            # Print the values to the serial p  ort
            temperature_c = random.randint(30,40) * 1.0
            temperature_f = temperature_c * (9 / 5) + 32.0
            pH = random.randint(0,14)* 1.0
            turb=random.uniform(0,2)

            print(
                "Temp: {:.2f} F / {} C    pH: {}  Turbidity:{:.2f}NTU".format(
                    temperature_f, temperature_c, pH,turb
                )
            )
            payload={"temp":temperature_c,"pH":pH,"turb":round(turb,2)}

            if(temperature_c>35 or not(6.5<pH<8.5) or (turb>1)):
                sms.send_sms()
                print("WATER IN BAD QUALITY,SMS SEND SUCCESSFULLY!")

            mqttc.publish(pubTopic1,json.dumps(payload))

            #mqttc.publish(pubTopic2,pH)
            #mqttc.publish(pubTopic3,round(turb,2))

            print("Published")
            req=requests.get("http://169.51.206.87:30996/motor")
            cmd=req.json()
            if(cmd!={}):
                print("MOTOR IS",cmd['motor'])
            time.sleep(5)

        except RuntimeError as error:
            print(error.args[0])
            time.sleep(2.0)
        except Exception as error:
            print("Error encountered!")
            time.sleep(5.0)
    mqttc.loop_forever()
if __name__=='__main__':
    run()

