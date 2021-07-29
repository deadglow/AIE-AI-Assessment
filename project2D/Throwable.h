#pragma once
#include "Component.h"
#include "Sound.h"

#define IMPACT_FORCE 500
#define SOUND_RADIUS 500
#define SOUND_LOUDNESS 500

class Transform;

class Throwable :
    public Component
{
public:
    using Component::Component;

    ~Throwable();

    Throwable* CloneTo(Entity* ent) override;

    void Update() override;
    void OnCollision(Collision collision) override;

    void Throw(Vector2 force, float spin);
    void Drop();

    void PickUp(Transform* parent, Vector2 offset);

protected:
    float soundRadius = SOUND_RADIUS;
    int soundLoudness = SOUND_LOUDNESS;
};

