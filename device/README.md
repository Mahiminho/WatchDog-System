# SET MQTT FROM WINDOWS

## INSTALL mosquitto
https://mosquitto.org/download/

## AS ADMIN EDIT FILE mosquitto.conf, AND ADD IN BEGINNING OF FILE
```
listener 1883
allow_anonymous true
```

## RUN MQTT FROM CMD FROM MOSQUITTO FOLDER
mosquitto -v -c mosquitto.conf

## RUN CMD AS ADMIN AND PASTE THIS COMMANDS (NECESSARY TO EXEC THEM EVERY SINGLE TIME BEFORE START)
1) netsh interface portproxy delete v4tov4 listenaddress=192.168.31.108 listenport=1883 **to clear**
2) netsh interface portproxy add v4tov4 listenaddress=192.168.31.108 listenport=1883 connectaddress=127.0.0.1 connectport=1883 **(but set ipv4 address or your PC in listenaddress)**
3) netsh interface portproxy show all

## INSTALL MQTT EXPLORER
https://mqtt-explorer.com/

## SET FIREWALL IN WINDOWS
1) Go to Windows Defender Firewall
2) Advanced security
3) Inbound rules
4) New Rule
5) Choose "Port" and click "Next"
6) Choose "TCP" and set "1883" and click "Next"
7) "Allow the connection" and "Next"
8) Choose all checkboxes and "Next"
9) Set name for rule (e.g. "MQTT Broker Port") and "Finish"

## IF SOMETHING DOESN'T CLEAR
https://youtu.be/5rHWeV0dwxo?si=WDT38q5fgN4hd2TB

# SET NETWORK SETTINGS IN MAIN.CPP (NETWORK MUST BE ONLY 2.4 GHz NOT 5 GHz)
const char* ssid = "NETWORK_NAME";
const char* password = "PASSWORD";
const char* mqttServer = "ipv4_of_your_machine_in_this_network";
const uint16_t mqttPort = 1883; // NECESSARY THIS PORT FOR MQTT
const char* mqttClientId = "esp32-client-01"; // nameof your device, whatever you want
const char* mqttTopic = "device/sensors/data"; // topic name, also whatever you want