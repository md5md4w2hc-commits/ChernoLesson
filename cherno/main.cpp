#include <iostream>
#include <cmath>
#include <vector>
#include "glfw3.h"

// Structure to hold coordinates for our trail
struct Point {
    float x, y;
};

int main()
{
    // 1. Initialize GLFW and Create Window
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 800, "Orbital Gravity Simulator", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // --- Physics Variables ---
    float G = 0.5f;
    float sunMass = 1.0f;
    float sunX = 0.0f;
    float sunY = 0.0f;

    float planetX = 0.5f;
    float planetY = 0.0f;
    float planetVX = 0.0f;
    float planetVY = 0.8f;   // Change this to 0.4f to see an oval orbit!

    std::vector<Point> trail;
    float lastTime = glfwGetTime();

    // 2. Main Game Loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Calculate delta time (dt)
        float currentTime = glfwGetTime();
        float dt = currentTime - lastTime;
        lastTime = currentTime;
        if (dt > 0.05f) dt = 0.05f;

        // --- UPDATE LOGIC (Physics) ---
        float dx = sunX - planetX;
        float dy = sunY - planetY;
        float distance = sqrt(dx * dx + dy * dy);

        // Prevent division by zero
        if (distance > 0.05f)
        {
            float force = (G * sunMass) / (distance * distance);
            float ax = force * (dx / distance);
            float ay = force * (dy / distance);

            planetVX += ax * dt;
            planetVY += ay * dt;
            planetX += planetVX * dt;
            planetY += planetVY * dt;
        }

        // --- TRAIL LOGIC ---
        trail.push_back({planetX, planetY});
        if (trail.size() > 800) {
            trail.erase(trail.begin()); // Remove oldest point to prevent memory bloat
        }

        // --- RENDERING LOGIC ---
        
        // 1. Draw the Orbit Trail (White Line)
        glBegin(GL_LINE_STRIP);
            glColor3f(1.0f, 1.0f, 1.0f);
            for (const auto& point : trail) {
                glVertex2f(point.x, point.y);
            }
        glEnd();

        // 2. Draw the Sun (Round and Yellow)
        float sunRadius = 0.08f;
        int segments = 40; // Number of sides to make it look smooth
        
        glBegin(GL_POLYGON);
            glColor3f(1.0f, 0.8f, 0.0f);
            for(int i = 0; i < segments; i++)
            {
                float theta = 2.0f * 3.1415926f * float(i) / float(segments);
                float xOffset = sunRadius * cos(theta);
                float yOffset = sunRadius * sin(theta);
                glVertex2f(sunX + xOffset, sunY + yOffset);
            }
        glEnd();

        // 3. Draw the Planet (Round and Blue)
        float planetRadius = 0.03f;
        glBegin(GL_POLYGON);
            glColor3f(0.2f, 0.6f, 1.0f);
            for(int i = 0; i < segments; i++)
            {
                float theta = 2.0f * 3.1415926f * float(i) / float(segments);
                float xOffset = planetRadius * cos(theta);
                float yOffset = planetRadius * sin(theta);
                glVertex2f(planetX + xOffset, planetY + yOffset);
            }
        glEnd();

        // Swap buffers and poll for events (like closing the window)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();
    return 0;
}

