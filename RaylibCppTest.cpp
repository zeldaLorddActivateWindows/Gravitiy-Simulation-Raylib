#include <cmath>
#include <vector>
#include "raylib.h"
#include "raymath.h"

const int SCREEN_WIDTH = 2500;
const int SCREEN_HEIGHT = 1500;
const float G = 500.0f;
const float MAX_VELOCITY_RATIO = 1.5f;  

class Planet {
public:
    Vector3 position;
    Vector3 velocity;
    float radius;
    float mass;
    Color color;

    std::vector<Vector3> trail;
    static const int MAX_TRAIL_LENGTH = 150;

    Planet(Vector3 pos, Vector3 vel, float rad, float m, Color col)
        : position(pos), velocity(vel), radius(rad), mass(m), color(col) {
    }

    void ApplyForce(Vector3 force) {
        velocity.x += force.x / mass;
        velocity.y += force.y / mass;
        velocity.z += force.z / mass;
    }

    void ConstrainVelocity(float maxVelocity) {
        float velocityMagnitude = Vector3Length(velocity);

        if (velocityMagnitude > maxVelocity) {
            velocity = Vector3Scale(Vector3Normalize(velocity), maxVelocity);
        }
    }

    void UpdatePosition(float maxVelocity) {
        ConstrainVelocity(maxVelocity);

        position = Vector3Add(position, velocity);

        trail.push_back(position);
        if (trail.size() > MAX_TRAIL_LENGTH) trail.erase(trail.begin());
    }

    void Draw() const {
        DrawSphere(position, radius, color);

        for (size_t i = 1; i < trail.size(); i += 5) {
            Vector3 start = trail[i - 1];
            Vector3 end = trail[i];
            Vector3 direction = Vector3Subtract(end, start);
            float length = Vector3Length(direction);
            if (length > 0.01f) {
                DrawCylinderEx(start, end, 1.8f, 1.8f, length, Fade(color, 0.3f));
            }
        }
    }
};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D sim");
    Camera3D camera = { 0 };
    camera.position = { 0.0f, 500.0f, 1000.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 110.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(165);

    Planet star({ 0.0f, 0.0f, 0.0f }, { 0, 0, 0 }, 50.0f, 6000.0f, GOLD);

    std::vector<Planet> planets;

    auto AddPlanet = [&](float orbitRadius, float y, float size, float mass, Color col, float orbitFactor = 1.0f) {
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        Vector3 pos = { orbitRadius * cosf(angle), y, orbitRadius * sinf(angle) };
        float speed = sqrtf(G * star.mass / orbitRadius) * orbitFactor;
        Vector3 vel = { speed * -sinf(angle), 0, speed * cosf(angle) };
        planets.emplace_back(pos, vel, size, mass, col);
        };

    AddPlanet(200.0f, 25.0f, 20.0f, 100.0f, BLUE);
    AddPlanet(350.0f, -25.0f, 15.0f, 50.0f, RED, 0.9f);
    AddPlanet(700.0f, 54.5f, 20.0f, 100.0f, GREEN);
    AddPlanet(1500.0f, 100.0f, 200.0f, 100.0f, ORANGE);

    for (int i = 0; i < 20; i++) {
        float distance = GetRandomValue(750, 1000);
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        Vector3 pos = { distance * cosf(angle), 50.0f, distance * sinf(angle) };
        float speed = sqrtf(G * star.mass / distance) * GetRandomValue(80, 120) / 100.0f;
        Vector3 vel = { speed * -sinf(angle), GetRandomValue(-10, 10) / 100.0f, speed * cosf(angle) };
        planets.emplace_back(pos, vel, GetRandomValue(2, 5), GetRandomValue(1, 10), GRAY);
    }

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_W)) camera.position.z -= 10.0f;
        if (IsKeyDown(KEY_S)) camera.position.z += 10.0f;
        if (IsKeyDown(KEY_A)) camera.position.x -= 10.0f;
        if (IsKeyDown(KEY_D)) camera.position.x += 10.0f;
        if (IsKeyDown(KEY_SPACE)) camera.position.y += 10.0f;
        if (IsKeyDown(KEY_LEFT_SHIFT)) camera.position.y -= 10.0f;

        for (size_t i = 0; i < planets.size(); i++) {
            Vector3 totalForce = { 0, 0, 0 };

            Vector3 dirToStar = Vector3Subtract(star.position, planets[i].position);
            float distToStar = fmaxf(Vector3Length(dirToStar), 10.0f);
            float forceStarMag = G * star.mass * planets[i].mass / (distToStar * distToStar);
            Vector3 forceFromStar = Vector3Scale(Vector3Normalize(dirToStar), forceStarMag);
            totalForce = Vector3Add(totalForce, forceFromStar);

            for (size_t j = 0; j < planets.size(); j++) {
                if (i == j) continue;
                Vector3 dir = Vector3Subtract(planets[j].position, planets[i].position);
                float dist = fmaxf(Vector3Length(dir), 5.0f);
                float forceMag = G * planets[i].mass * planets[j].mass / (dist * dist);
                Vector3 force = Vector3Scale(Vector3Normalize(dir), forceMag);
                totalForce = Vector3Add(totalForce, force);
            }

            float distanceToStar = Vector3Length(Vector3Subtract(planets[i].position, star.position));
            float maxVelocity = MAX_VELOCITY_RATIO * sqrtf(G * star.mass / distanceToStar);
            planets[i].ApplyForce(totalForce);
            planets[i].UpdatePosition(maxVelocity);  
        }

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        DrawSphereWires(star.position, star.radius * 1.1f, 16, 16, YELLOW);
        DrawSphere(star.position, star.radius, star.color);

        for (const auto& planet : planets) {
            planet.Draw();
        }

        DrawGrid(1000, 50.0f);
        EndMode3D();

        DrawText("3D sim", 10, 10, 20, WHITE);
        DrawFPS(10, 100);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
