#include <iostream>
#include <cmath>
#include <vector>
#include "glfw3.h"

// A simple structure to hold our X and Y coordinates
struct Point {
    float x, y;
};

int main()
{
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 800, "Double Pendulum with Trail", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // --- Physics Variables ---
    float l1 = 0.4f;
    float l2 = 0.4f;
    float m1 = 10.0f;
    float m2 = 10.0f;
    
    float a1 = 3.14159f / 2.0f;
    float a2 = 3.14159f / 2.0f;
    float a1_v = 0.0f;
    float a2_v = 0.0f;
    float g = 9.81f;

    float px = 0.0f;
    float py = 0.6f;

    float lastTime = glfwGetTime();

    // Create a vector to store the trail points
    std::vector<Point> trail;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        float currentTime = glfwGetTime();
        float dt = currentTime - lastTime;
        lastTime = currentTime;

        if (dt > 0.05f) dt = 0.05f;
        
        float timeScale = 1.0f;
        dt *= timeScale;

        // Calculate Accelerations
        float num1 = -g * (2 * m1 + m2) * sin(a1) - m2 * g * sin(a1 - 2 * a2) - 2 * sin(a1 - a2) * m2 * (a2_v * a2_v * l2 + a1_v * a1_v * l1 * cos(a1 - a2));
        float den1 = l1 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
        float a1_a = num1 / den1;

        float num2 = 2 * sin(a1 - a2) * (a1_v * a1_v * l1 * (m1 + m2) + g * (m1 + m2) * cos(a1) + a2_v * a2_v * l2 * m2 * cos(a1 - a2));
        float den2 = l2 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
        float a2_a = num2 / den2;

        a1_v += a1_a * dt;
        a2_v += a2_a * dt;

        float damping = 0.999f;
        a1_v *= damping;
        a2_v *= damping;
        
        a1 += a1_v * dt;
        a2 += a2_v * dt;

        float x1 = px + l1 * sin(a1);
        float y1 = py - l1 * cos(a1);
        
        float x2 = x1 + l2 * sin(a2);
        float y2 = y1 - l2 * cos(a2);

        // --- TRAIL LOGIC ---
        // Add the current position of the bottom bob to the trail
        trail.push_back({x2, y2});
        
        // If the trail gets too long, remove the oldest point at the beginning
        if (trail.size() > 1000)
        {
            trail.erase(trail.begin());
        }

        // --- RENDERING LOGIC ---
        
        // 1. Draw the Trail first so it renders behind the pendulum
        glBegin(GL_LINE_STRIP);
            glColor3f(1.0f, 0.5f, 0.0f); // Orange color for the trail
            for (const auto& point : trail)
            {
                glVertex2f(point.x, point.y);
            }
        glEnd();

        // 2. Draw the Rods
        glBegin(GL_LINES);
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex2f(px, py);
            glVertex2f(x1, y1);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        glEnd();

        // 3. Draw the Bobs
        float radius = 0.06f;
        glBegin(GL_QUADS);
            glColor3f(0.2f, 0.8f, 0.2f);
            glVertex2f(x1 - radius, y1 - radius);
            glVertex2f(x1 + radius, y1 - radius);
            glVertex2f(x1 + radius, y1 + radius);
            glVertex2f(x1 - radius, y1 + radius);
        glEnd();

        glBegin(GL_QUADS);
            glColor3f(0.2f, 0.6f, 1.0f);
            glVertex2f(x2 - radius, y2 - radius);
            glVertex2f(x2 + radius, y2 - radius);
            glVertex2f(x2 + radius, y2 + radius);
            glVertex2f(x2 - radius, y2 + radius);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

