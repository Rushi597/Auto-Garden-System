#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include <fmt/core.h>
#include "garden_manager.h"
#include <iostream>
#include <vector>

/**
 * Entry point for the Automated Garden Management System application.
 * This function initializes the SFML window, loads resources, and manages the main event loop.
 */
int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "Automated Garden Management System");

    // Load background image
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.png")) {
        spdlog::error("Failed to load background image");
        return -1;
    }
    sf::Sprite background(backgroundTexture);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("DejaVuSans-Bold.ttf")) {
        spdlog::error("Failed to load font");
        return -1;
    }

    GardenManager gardenManager;

    // Text element to display plant health
    sf::Text healthText;
    healthText.setFont(font);
    healthText.setCharacterSize(24);
    healthText.setFillColor(sf::Color::Black);
    healthText.setPosition(10, 10);

    // Text element to display water message
    sf::Text waterMessageText;
    waterMessageText.setFont(font);
    waterMessageText.setCharacterSize(24);
    waterMessageText.setFillColor(sf::Color::Black);
    waterMessageText.setPosition(10, 50);
    waterMessageText.setString("");

    // Names of the sensors
    std::vector<std::string> sensorNames = {"Soil Moisture", "Temperature", "Light Intensity"};
    size_t currentSensorIndex = 0;

    // Text element to display current sensor name
    sf::Text sensorNameText;
    sensorNameText.setFont(font);
    sensorNameText.setCharacterSize(24);
    sensorNameText.setFillColor(sf::Color::Black);
    sensorNameText.setPosition(312, 540);
    sensorNameText.setString(sensorNames[currentSensorIndex]);

    // Text element to display sensor data at the top right
    sf::Text sensorDataText;
    sensorDataText.setFont(font);
    sensorDataText.setCharacterSize(24);
    sensorDataText.setFillColor(sf::Color::Black);
    sensorDataText.setPosition(700, 10);

    // Text element to display the plant name
    sf::Text plantNameText;
    plantNameText.setFont(font);
    plantNameText.setCharacterSize(60);
    plantNameText.setFillColor(sf::Color::Black);
    plantNameText.setPosition(412, 10);
    plantNameText.setString("Rose");

    // Create a button for watering plants
    sf::RectangleShape waterButton(sf::Vector2f(200, 50));
    waterButton.setFillColor(sf::Color::Blue);
    waterButton.setPosition(400, 800);

    // Text on the water button
    sf::Text waterButtonText;
    waterButtonText.setFont(font);
    waterButtonText.setString("Water");
    waterButtonText.setCharacterSize(24);
    waterButtonText.setFillColor(sf::Color::White);
    waterButtonText.setPosition(460, 810);

    // Create a button for setting sensor data
    sf::RectangleShape sensorButton(sf::Vector2f(200, 50));
    sensorButton.setFillColor(sf::Color::Red);
    sensorButton.setPosition(400, 700);

    // Text on the sensor button
    sf::Text sensorButtonText;
    sensorButtonText.setFont(font);
    sensorButtonText.setString("Set Sensor");
    sensorButtonText.setCharacterSize(24);
    sensorButtonText.setFillColor(sf::Color::White);
    sensorButtonText.setPosition(420, 710);

    // Create an input field for sensor data
    sf::RectangleShape inputField(sf::Vector2f(400, 50));
    inputField.setFillColor(sf::Color::White);
    inputField.setPosition(312, 580);

    // Text element to display the user's input in the input field
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(320, 590);

    std::string inputString;
    bool isEnteringInput = false;

    sf::Clock clock;
    sf::Time lastWatered = clock.getElapsedTime();
    sf::Time waterMessageDisplayTime;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered && isEnteringInput) {
                // Handle backspace
                if (event.text.unicode == '\b') {
                    if (!inputString.empty()) {
                        inputString.pop_back();
                    }
                } 
                // Handle enter key
                else if (event.text.unicode == '\r') {
                    isEnteringInput = false;
                    if (!inputString.empty()) {
                        gardenManager.setSensorData(currentSensorIndex, std::stoi(inputString));
                        inputString.clear();
                        currentSensorIndex = (currentSensorIndex + 1) % sensorNames.size();
                        sensorNameText.setString(sensorNames[currentSensorIndex]);
                    }
                } 
                // Handle other characters
                else if (event.text.unicode < 128) {
                    inputString += static_cast<char>(event.text.unicode);
                }
                inputText.setString(inputString);
            } else if (event.type == sf::Event::MouseButtonPressed) {
                // Check if water button is clicked
                if (waterButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    gardenManager.waterPlants();
                    lastWatered = clock.getElapsedTime();
                    waterMessageText.setString("The Plant have been watered!");
                    waterMessageDisplayTime = clock.getElapsedTime();
                } 
                // Check if sensor button is clicked
                else if (sensorButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    isEnteringInput = true;
                }
            }
        }

        // Update plant health over time
        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed - lastWatered > sf::seconds(180)) {
            if (elapsed - lastWatered > sf::seconds(240)) {
                if (elapsed - lastWatered > sf::seconds(300)) {
                    gardenManager.decreaseHealth();
                    lastWatered = clock.getElapsedTime() - sf::seconds(60);
                }
            }
        }

        // Hide water message after 5 seconds
        if (clock.getElapsedTime() - waterMessageDisplayTime > sf::seconds(5)) {
            waterMessageText.setString("");
        }

        window.clear();
        window.draw(background);
        window.draw(waterButton);
        window.draw(waterButtonText);
        window.draw(sensorButton);
        window.draw(sensorButtonText);
        window.draw(inputField);
        window.draw(inputText);
        window.draw(waterMessageText);
        window.draw(sensorNameText);
        window.draw(plantNameText);

        // Update and draw health text
        healthText.setString(fmt::format("Health: {}", gardenManager.getHealth()));
        window.draw(healthText);

        // Update and draw sensor data text
        std::string sensorDataStr = fmt::format("Soil Moisture: {}\nTemperature: {}C\nLight Intensity: {}%",
                                                gardenManager.getSensorData(0),
                                                gardenManager.getSensorData(1),
                                                gardenManager.getSensorData(2));
        sensorDataText.setString(sensorDataStr);
        window.draw(sensorDataText);

        window.display();
    }

    return 0;
}
