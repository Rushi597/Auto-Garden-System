#include "garden_manager.h"
#include <spdlog/spdlog.h>

/**
 * Constructor for GardenManager class.
 * Initializes plant health and sensor data.
 */
GardenManager::GardenManager() : plantHealth(100), sensorData(3, 0) {}

/**
 * Water the plants, setting their health to 100.
 */
void GardenManager::waterPlants() {
    spdlog::info("Watering plants");
    plantHealth = 100;
}

/**
 * Decrease the health of the plants by 1.
 */
void GardenManager::decreaseHealth() {
    if (plantHealth > 0) {
        plantHealth--;
        spdlog::info("Decreased plant health to {}", plantHealth);
    }
}

/**
 * Set sensor data for a specific sensor index.
 * 
 * @param index Index of the sensor.
 * @param data Sensor data value.
 */
void GardenManager::setSensorData(int index, int data) {
    if (index >= 0 && index < sensorData.size()) {
        sensorData[index] = data;
        spdlog::info("Sensor data set to {} at index {}", data, index);
        updatePlantHealth();
    }
}

/**
 * Get the current health of the plants.
 * 
 * @return Plant health.
 */
int GardenManager::getHealth() const {
    return plantHealth;
}

/**
 * Get the sensor data for a specific sensor index.
 * 
 * @param index Index of the sensor.
 * @return Sensor data value.
 */
int GardenManager::getSensorData(int index) const {
    if (index >= 0 && index < sensorData.size()) {
        return sensorData[index];
    }
    return -1;
}

/**
 * Update plant health based on sensor data.
 */
void GardenManager::updatePlantHealth() {
    plantHealth = 100;
    if (sensorData[0] < 30 || sensorData[0] > 70) {
        plantHealth -= 10;
    }
    if (sensorData[1] < 15 || sensorData[1] > 30) {
        plantHealth -= 10;
    }
    if (sensorData[2] < 50 || sensorData[2] > 90) {
        plantHealth -= 10;
    }
}
