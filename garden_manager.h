#ifndef GARDEN_MANAGER_H
#define GARDEN_MANAGER_H

#include <vector>

/**
 * GardenManager class manages the health of plants and sensor data.
 */
class GardenManager {
public:
    GardenManager();
    void waterPlants();
    void decreaseHealth();
    void setSensorData(int index, int data);
    int getHealth() const;
    int getSensorData(int index) const;

private:
    void updatePlantHealth();
    int plantHealth;
    std::vector<int> sensorData;
};

#endif // GARDEN_MANAGER_H
