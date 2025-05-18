#pragma once
#include <string>
#include <cmath>
#include <raylib.h>

class Planet2D
{
public:
    struct PlanetData
    {
        float posX_;
        float posY_;
        float radius_;
        float mass_;
        std::string name_;
        float inertiaX_;
        float inertiaY_;
        Color color_;
    };

    Planet2D(float posX, float posY, float radius, float mass,
        std::string name, float inertiaX, float inertiaY,
        Color color = WHITE)
        : data{ posX, posY, radius, mass, name, inertiaX, inertiaY, color } {
    }

    float GetPosX() const { return data.posX_; }
    float GetPosY() const { return data.posY_; }
    float GetRadius() const { return data.radius_; }
    float GetMass() const { return data.mass_; }
    std::string GetName() const { return data.name_; }
    float GetInertiaX() const { return data.inertiaX_; }
    float GetInertiaY() const { return data.inertiaY_; }
    Color GetColor() const { return data.color_; }

    void SetPosX(float posX) { data.posX_ = posX; }
    void SetPosY(float posY) { data.posY_ = posY; }
    void SetRadius(float radius) { data.radius_ = radius; }
    void SetMass(float mass) { data.mass_ = mass; }
    void SetName(std::string name) { data.name_ = name; }
    void SetInertiaX(float inertiaX) { data.inertiaX_ = inertiaX; }
    void SetInertiaY(float inertiaY) { data.inertiaY_ = inertiaY; }
    void SetColor(Color color) { data.color_ = color; }

    void ApplyForce(float fx, float fy)
    {
        data.inertiaX_ += fx / data.mass_;
        data.inertiaY_ += fy / data.mass_;
    }

    void UpdatePosition()
    {
        data.posX_ += data.inertiaX_;
        data.posY_ += data.inertiaY_;
    }

    void Draw() const
    {
        DrawCircle(data.posX_, data.posY_, data.radius_, data.color_);
    }

    bool CheckCollision(const Planet2D& other) const
    {
        float dx = other.data.posX_ - data.posX_;
        float dy = other.data.posY_ - data.posY_;
        float distance = sqrtf(dx * dx + dy * dy);
        return distance < (data.radius_ + other.data.radius_);
    }

private:
    PlanetData data;
};
