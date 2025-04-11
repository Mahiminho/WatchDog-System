# from fastapi import FastAPI
# import asyncio
# import random
# from datetime import datetime
# import httpx

# app = FastAPI()

# TARGET_URL = "http://localhost:5173/"

# def generate_data():
#     return {
#         "time": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
#         "airData": random.randint(1, 150),
#         "lightData": random.randint(1, 150),
#         "soundData": random.randint(1, 150),
#         "tempData": random.randint(-10, 55)
#     }

# async def send_data_forever():
#     async with httpx.AsyncClient() as client:
#         while True:
#             data = generate_data()
#             try:
#                 response = await client.post(TARGET_URL, json=data)
#                 print(f"Sent: {data}, Status: {response.status_code}")
#             except Exception as e:
#                 print(f"Failed to send data: {e}")
#             await asyncio.sleep(1)

# @app.on_event("startup")
# async def startup_event():
#     asyncio.create_task(send_data_forever())

# @app.get("/")
# async def root():
#     return {"status": "Background sender is running"}

# from fastapi import FastAPI
# import random
# from datetime import datetime

# app = FastAPI()

# def generate_data():
#     return {
#         "time": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
#         "airData": random.randint(1, 150),
#         "lightData": random.randint(1, 150),
#         "soundData": random.randint(1, 150),
#         "tempData": random.randint(-10, 55)
#     }

# @app.get("/data")
# def get_data():
#     return generate_data()

from fastapi import FastAPI
import asyncio
import random
from datetime import datetime
import logging
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()

# CORS (щоб React міг звертатися)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # або ["http://localhost:5173"]
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Увімкнемо логування
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# Поточні дані, які віддаватимуться по GET
latest_data = {}

def generate_data():
    return {
        "time": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        "airData": random.randint(1, 150),
        "lightData": random.randint(1, 150),
        "soundData": random.randint(1, 150),
        "tempData": random.randint(-10, 55)
    }

async def update_data_forever():
    global latest_data
    while True:
        latest_data = generate_data()
        logger.info(f"[Generated] {latest_data}")
        await asyncio.sleep(1)

@app.on_event("startup")
async def startup_event():
    asyncio.create_task(update_data_forever())

@app.get("/")
async def root():
    return {"status": "Backend is alive"}

@app.get("/data")
async def get_data():
    logger.info("[/data] React frontend fetched the data")
    return latest_data