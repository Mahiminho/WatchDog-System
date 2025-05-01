# from fastapi import FastAPI
# import asyncio
# import random
# from datetime import datetime
# import logging
# from fastapi.middleware.cors import CORSMiddleware

# app = FastAPI()

# # CORS (щоб React міг звертатися)
# app.add_middleware(
#     CORSMiddleware,
#     allow_origins=["*"],  # або ["http://localhost:5173"]
#     allow_credentials=True,
#     allow_methods=["*"],
#     allow_headers=["*"],
# )

# # Увімкнемо логування
# logging.basicConfig(level=logging.INFO)
# logger = logging.getLogger(__name__)

# # Поточні дані, які віддаватимуться по GET
# latest_data = {}

# def generate_data():
#     return {
#         "time": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
#         "airData": random.randint(1, 150),
#         "lightData": random.randint(1, 150),
#         "soundData": random.randint(1, 150),
#         "tempData": random.randint(-10, 55)
#     }

# async def update_data_forever():
#     global latest_data
#     while True:
#         latest_data = generate_data()
#         logger.info(f"[Generated] {latest_data}")
#         await asyncio.sleep(1)

# @app.on_event("startup")
# async def startup_event():
#     asyncio.create_task(update_data_forever())

# @app.get("/")
# async def root():
#     return {"status": "Backend is alive"}

# @app.get("/data")
# async def get_data():
#     logger.info("[/data] React frontend fetched the data")
#     return latest_data

from fastapi import FastAPI
from datetime import datetime
import logging
from fastapi.middleware.cors import CORSMiddleware
import json
import paho.mqtt.client as mqtt

app = FastAPI()

# CORS (для фронтенду, напр. React)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Логування
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# Дані, які віддаються на фронтенд
latest_data = {}

# MQTT налаштування (це IP брокера, який ESP32 використовує)
MQTT_BROKER = "192.168.31.108"
MQTT_PORT = 1883
MQTT_TOPIC = "sensors/json"

# MQTT-клієнт
mqtt_client = mqtt.Client()

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        logger.info("[MQTT] Connected to broker")
        client.subscribe(MQTT_TOPIC)
    else:
        logger.error(f"[MQTT] Connection failed with code {rc}")

def on_message(client, userdata, msg):
    global latest_data
    try:
        payload = msg.payload.decode("utf-8")
        logger.info(f"[MQTT] Received message: {payload}")
        data = json.loads(payload)
        data["time"] = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        latest_data = {
            "airData": data.get("airQualityPPM", 0),
            "lightData": data.get("lightIntensityLux", 0),
            "soundData": data.get("noiseLevelDB", 0),
            "tempData": data.get("temperatureCelsius", 0),
            "fireScore": data.get("fireScore", 0),
            "fireDetected": data.get("fireDetected", False),
            "presence": data.get("presenceDetected", False),
            "motion": data.get("motionDetected", False),
            "vibration": data.get("vibrationDetected", False),
            "time": data["time"]
        }
    except Exception as e:
        logger.error(f"[MQTT] Error handling message: {e}")

def start_mqtt():
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message
    mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)
    mqtt_client.loop_start()

@app.on_event("startup")
async def startup_event():
    logger.info("[Startup] Launching MQTT client")
    start_mqtt()

@app.get("/")
async def root():
    return {"status": "Backend is alive"}

@app.get("/data")
async def get_data():
    logger.info("[/data] React frontend fetched the data")
    return latest_data