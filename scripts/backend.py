import firebase_admin
from firebase_admin import credentials, firestore
from datetime import datetime, timezone
import random
import time
import numpy as np
from flask import Flask, jsonify
import threading

# -------- Initialize Firebase --------
cred = credentials.Certificate(r"..\firebase\serviceAccountKey.json")  # Windows path
firebase_admin.initialize_app(cred)
db = firestore.client()
sensor_ref = db.collection("sensor_readings")

# -------- Configuration --------
NUM_SENSORS = 3       # number of simulated devices
SEND_INTERVAL = 5     # seconds between sending data
ANOMALY_INTERVAL = 10 # seconds between anomaly checks
FETCH_LIMIT = 20      # readings to check for anomalies

# -------- Generate Sensor Data --------
def generate_sensor_data(sensor_id):
    return {
        "sensor_id": sensor_id,
        "heart_rate": random.randint(60, 100),
        "SpO2": random.randint(95, 100),
        "motion": {
            "x": round(random.uniform(-2, 2), 2),
            "y": round(random.uniform(-2, 2), 2),
            "z": round(random.uniform(-2, 2), 2)
        },
        "temperature": round(random.uniform(20, 40), 2),
        "gas_dust": round(random.uniform(0, 300), 2),
        "battery_type": random.choice(["Li-ion", "NiMH", "Li-Po"]),
        "microcontroller": random.choice(["ESP32", "Arduino Uno"]),
        "timestamp": datetime.now(timezone.utc)
    }

# -------- Send Data to Firebase --------
def send_data():
    while True:
        for sensor_id in range(1, NUM_SENSORS + 1):
            data = generate_sensor_data(sensor_id)
            sensor_ref.add(data)
            print("Data sent:", data)
        time.sleep(SEND_INTERVAL)

# -------- Fetch Recent Readings --------
def fetch_recent_readings(limit=FETCH_LIMIT):
    docs = sensor_ref.order_by("timestamp", direction=firestore.Query.DESCENDING).limit(limit).stream()
    return [doc.to_dict() for doc in docs]

# -------- Anomaly Detection --------
def detect_anomalies():
    while True:
        data_list = fetch_recent_readings()
        if not data_list:
            time.sleep(ANOMALY_INTERVAL)
            continue
        
        # Example: anomaly if heart_rate < 60 or > 100, temperature < 20 or > 40
        anomalies = []
        for d in data_list:
            if d['heart_rate'] < 60 or d['heart_rate'] > 100:
                anomalies.append({"sensor_id": d['sensor_id'], "type": "heart_rate", "value": d['heart_rate']})
            if d['temperature'] < 20 or d['temperature'] > 40:
                anomalies.append({"sensor_id": d['sensor_id'], "type": "temperature", "value": d['temperature']})
            if d['SpO2'] < 95:
                anomalies.append({"sensor_id": d['sensor_id'], "type": "SpO2", "value": d['SpO2']})
        if anomalies:
            print("Anomalies detected:", anomalies)
        time.sleep(ANOMALY_INTERVAL)

# -------- Flask API --------
app = Flask(__name__)

@app.route("/latest", methods=["GET"])
def latest_reading():
    docs = sensor_ref.order_by("timestamp", direction=firestore.Query.DESCENDING).limit(NUM_SENSORS).stream()
    return jsonify([doc.to_dict() for doc in docs])

@app.route("/all", methods=["GET"])
def all_readings():
    docs = sensor_ref.order_by("timestamp", direction=firestore.Query.DESCENDING).limit(100).stream()
    return jsonify([doc.to_dict() for doc in docs])

@app.route("/anomalies", methods=["GET"])
def anomalies_endpoint():
    data_list = fetch_recent_readings()
    anomalies = []
    for d in data_list:
        if d['heart_rate'] < 60 or d['heart_rate'] > 100:
            anomalies.append({"sensor_id": d['sensor_id'], "type": "heart_rate", "value": d['heart_rate']})
        if d['temperature'] < 20 or d['temperature'] > 40:
            anomalies.append({"sensor_id": d['sensor_id'], "type": "temperature", "value": d['temperature']})
        if d['SpO2'] < 95:
            anomalies.append({"sensor_id": d['sensor_id'], "type": "SpO2", "value": d['SpO2']})
    return jsonify(anomalies)

# -------- Run everything concurrently --------
if __name__ == "__main__":
    threading.Thread(target=send_data, daemon=True).start()
    threading.Thread(target=detect_anomalies, daemon=True).start()
    app.run(host="0.0.0.0", port=5000)
