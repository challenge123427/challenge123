# upload_data.py

import os
import random
import time
import firebase_admin
from firebase_admin import credentials, firestore

# 1. Build path to JSON file
base_dir = os.path.dirname(os.path.dirname(__file__))  # goes up one folder from scripts/
json_path = os.path.join(base_dir, "firebase", "serviceAccountKey.json")

# 2. Check if file exists
if not os.path.exists(json_path):
    raise FileNotFoundError(f"Firebase JSON file not found at {json_path}")

# 3. Initialize Firebase
cred = credentials.Certificate(json_path)
firebase_admin.initialize_app(cred)

# 4. Connect to Firestore
db = firestore.client()

# 5. Function to generate random sensor data
def generate_sensor_data():
    return {
        "temperature": round(random.uniform(20, 30), 2),
        "humidity": round(random.uniform(40, 70), 2),
        "light": round(random.uniform(200, 1000), 2),
        "timestamp": firestore.SERVER_TIMESTAMP
    }

# 6. Upload multiple sensor readings and print actual timestamp
collection_name = "sensors"

for i in range(10):  # change 10 to number of readings
    data = generate_sensor_data()
    doc_ref = db.collection(collection_name).document(f"sensor_{i+1}")
    doc_ref.set(data)

    # Fetch the document back to get the actual server timestamp
    uploaded_doc = doc_ref.get()
    uploaded_data = uploaded_doc.to_dict()
    print(f"Uploaded reading {i+1}: {uploaded_data}")

    time.sleep(0.5)  # optional delay between uploads
