"""
Train tiny placeholder models for VitaLink edge AI and export to models/*.tflite.

This script creates minimal dense networks and exports quantized TFLite models.
In production, replace datasets with labeled industrial data and tune architectures.

Usage:
  python train_models.py
"""

import numpy as np
import tensorflow as tf
import os

out_dir = os.path.join(os.path.dirname(__file__), "..", "models")
os.makedirs(out_dir, exist_ok=True)

def make_and_save(name, input_dim=10):
    x = np.random.randn(1000, input_dim).astype(np.float32)
    y = (np.random.rand(1000) > 0.95).astype(np.float32)
    model = tf.keras.Sequential([
        tf.keras.layers.Input(shape=(input_dim,)),
        tf.keras.layers.Dense(16, activation='relu'),
        tf.keras.layers.Dense(8, activation='relu'),
        tf.keras.layers.Dense(1, activation='sigmoid'),
    ])
    model.compile(optimizer='adam', loss='binary_crossentropy')
    model.fit(x, y, epochs=3, batch_size=32, verbose=0)
    # convert to quantized tflite
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    tflite_model = converter.convert()
    path = os.path.join(out_dir, f"{name}.tflite")
    with open(path, "wb") as f:
        f.write(tflite_model)
    print("Saved", path)

if __name__ == "__main__":
    make_and_save("heat_stress", input_dim=6)
    make_and_save("fatigue", input_dim=8)
    make_and_save("hr_anomaly", input_dim=4)
    make_and_save("gas_anomaly", input_dim=3)
    make_and_save("unsafe_behavior", input_dim=6)
