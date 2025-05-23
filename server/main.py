from fastapi import FastAPI
from pydantic import BaseModel
import paho.mqtt.client as mqtt
import threading
import json
import asyncio
import ollama

app = FastAPI()

# MQTT settings
MQTT_BROKER = "192.168.31.108"
MQTT_PORT = 1883
MQTT_TOPIC = "device/sensors/data"
MQTT_CLIENT_ID = "fastapi-mqtt-server"

# Ollama model name
OLLAMA_MODEL = "phi3:latest"

class SensorData(BaseModel):
    temperature: float
    airPPM: int
    lightLux: int
    presence: bool
    presenceDistance: float
    motion: bool
    noiseDB: int
    vibration: bool
    fireScore: float
    fireDetected: bool
    gasScore: float
    gasDetected: bool
    humanScore: float
    humanDetected: bool
    damageScore: float
    damageDetected: bool

class SensorAnalysisResult:
    def __init__(self, data: SensorData):
        self.data = data
        self.single_sensor_insights: list[str] = []
        self.combined_sensor_insights: list[str] = []
        self.ai_report: str | None = None

    def add_single(self, insight: str):
        self.single_sensor_insights.append(insight)

    def add_combined(self, insight: str):
        self.combined_sensor_insights.append(insight)

# Store latest result globally for scheduled reporting
latest_result: SensorAnalysisResult | None = None

async def generate_environment_report(readings: dict, previous: str | None) -> str:
    prompt = (
        (f"Previous: {previous}\n" if previous else "") +
        f"Current readings: {readings}\n" +
        "You are an environmental monitoring specialist. Write a single concise (2–3 sentences), professional paragraph in plain English describing the current environment. Refer to values only in digits, skip sensor explanations, and mention only meaningful changes compared to the previous snapshot. Focus on comfort, air quality, light, presence/motion, vibration, or human detection."
    )
    loop = asyncio.get_running_loop()
    # Use ollama.generate directly
    response = await loop.run_in_executor(
        None,
        lambda: ollama.generate(model=OLLAMA_MODEL, prompt=prompt)
    )
    return response['response'].strip()

async def periodic_report_task():
    global latest_result
    previous_report = None
    while True:
        await asyncio.sleep(60)  # wait 1 minute
        if latest_result is None:
            continue
        report = await generate_environment_report(latest_result.data.dict(), previous_report)
        latest_result.ai_report = report
        previous_report = report
        print(f"\nAI Report (scheduled): {report}\n")

# Detailed sensor analysis functions
def analyze_single_sensors(data: SensorData, result: SensorAnalysisResult):
    if data.temperature < 14:
      result.add_single("Temperature is cold and uncomfortable.")
    elif data.temperature < 18:
      result.add_single("Temperature is cool, slightly below comfort zone.")
    elif data.temperature < 22:
      result.add_single("Temperature is comfortable and ideal.")
    elif data.temperature < 25:
      result.add_single("Temperature is comfortably warm.")
    elif data.temperature < 28:
      result.add_single("Temperature is warm, approaching discomfort.")
    elif data.temperature < 32:
      result.add_single("Temperature is hot and uncomfortable.")
    else:
      result.add_single("Temperature is extremely hot, ventilation needed.")

    if data.airPPM < 700:
      result.add_single("Air quality is excellent.")
    elif data.airPPM < 1000:
      result.add_single("Air quality is good.")
    elif data.airPPM < 1500:
      result.add_single("Air quality is moderate.")
    elif data.airPPM < 2500:
      result.add_single("Air quality is unhealthy for sensitive groups.")
    else:
      result.add_single("Air quality is poor, consider ventilation.")

    if data.lightLux < 10:
      result.add_single("Lighting is very low, environment is dark.")
    elif data.lightLux < 100:
      result.add_single("Lighting is dim; low visibility.")
    elif data.lightLux < 500:
      result.add_single("Lighting is moderate, comfortable for tasks.")
    elif data.lightLux < 2000:
      result.add_single("Lighting is bright, suitable for most activities.")
    else:
      result.add_single("Lighting is very bright, comparable to daylight.")

    if data.noiseDB < 20:
      result.add_single("Noise level is extremely low, unusually quiet environment.")
    elif data.noiseDB < 30:
      result.add_single("Noise level is very low, typical of a quiet room.")
    elif data.noiseDB < 45:
      result.add_single("Noise level is low, similar to a residential area at night.")
    elif data.noiseDB < 60:
      result.add_single("Noise level is moderate, comparable to normal conversation.")
    elif data.noiseDB < 70:
      result.add_single("Noise level is elevated.")
    elif data.noiseDB < 85:
      result.add_single("Noise level is high, may cause discomfort over time.")
    elif data.noiseDB < 95:
      result.add_single("Noise level is very high, potentially harmful for prolonged exposure.")
    else:
      result.add_single("Noise level is dangerously high, hearing protection recommended.")

    if data.presence:
        if data.presenceDistance < 1.0:
            result.add_single("Presence detected very close (<1m).")
        elif data.presenceDistance < 3.0:
            result.add_single("Presence detected at moderate distance.")
        else:
            result.add_single("Presence detected but far away.")
    else:
        result.add_single("No presence detected.")

    result.add_single("Motion detected." if data.motion else "No motion detected.")
    result.add_single("Vibration detected." if data.vibration else "No vibration detected.")


def analyze_combined_sensors(data: SensorData, result: SensorAnalysisResult):
    fire_factor = 0
    if data.temperature > 40:
        fire_factor += 1
    if data.gasScore > 0.5:
        fire_factor += 1
    if data.lightLux > 500:
        fire_factor += 1
    if data.fireDetected:
        fire_factor += 2
    if fire_factor >= 4:
        result.add_combined("Possible fire outbreak; immediate action required.")
    elif fire_factor >= 2:
        result.add_combined("Conditions indicate potential fire risk.")
    else:
        result.add_combined("No significant fire indicators.")

    if data.gasDetected:
        result.add_combined("High probability of hazardous gas presence.")
    elif data.airPPM > 1500:
        result.add_combined("Elevated particulate levels, air pollution risk.")
    else:
        result.add_combined("Air pollution levels are within acceptable range.")

    human_factor = 0
    if data.humanDetected:
        human_factor += 2
    if data.presence:
        human_factor += 1
    if data.motion:
        human_factor += 1
    if data.humanScore > 50:
        human_factor += 1
    if data.noiseDB > 50:
        human_factor += 1
    if data.presenceDistance < 2:
        human_factor += 1
    if human_factor >= 5:
        result.add_combined("Strong evidence of human presence detected.")
    elif human_factor >= 3:
        result.add_combined("Possible human presence in the area.")
    else:
        result.add_combined("No clear human presence detected.")

    damage_factor = 0
    if data.damageDetected:
        damage_factor += 2
    if data.vibration:
        damage_factor += 1
    if data.damageScore > 0.5:
        damage_factor += 1
    if data.presence and data.presenceDistance < 1.5:
        damage_factor += 1
    if damage_factor >= 4:
        result.add_combined("Potential structural damage detected; inspect immediately.")
    elif damage_factor >= 2:
        result.add_combined("Signs of possible damage; monitor closely.")
    else:
        result.add_combined("No structural damage suspected.")

def print_sensor_data(data: SensorData, result: SensorAnalysisResult):
    print("\nIncoming Sensor Data:")
    for field, value in data.dict().items():
        print(f"{field}: {value}")
    print("\nSensor-based Insights:")
    for insight in result.single_sensor_insights:
        print(f"- {insight}")
    print("\nCombined Sensor Interpretations:")
    for insight in result.combined_sensor_insights:
        print(f"* {insight}")
    if result.ai_report:
        print(f"\nAI Report: {result.ai_report}")
    print("\n" + "-" * 50 + "\n")

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker with result code", rc)
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    global latest_result
    try:
        payload_str = msg.payload.decode()
        data = json.loads(payload_str)
        sensor_data = SensorData(**data)
        result = SensorAnalysisResult(sensor_data)
        analyze_single_sensors(sensor_data, result)
        analyze_combined_sensors(sensor_data, result)
        latest_result = result
        print_sensor_data(sensor_data, result)
    except json.JSONDecodeError as e:
        print("JSON decode error:", e)
    except Exception as e:
        print("Parsing error:", e)

def start_mqtt():
    client = mqtt.Client(client_id=MQTT_CLIENT_ID)
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_forever()

@app.on_event("startup")
def startup_event():
    mqtt_thread = threading.Thread(target=start_mqtt)
    mqtt_thread.daemon = True
    mqtt_thread.start()
    asyncio.create_task(periodic_report_task())

# uvicorn main:app --host 0.0.0.0 --port 8000