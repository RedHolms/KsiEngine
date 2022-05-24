#pragma once
#include "KsiMain.hpp"

#include "Render.hpp"
#include "Vector.hpp"

KSI_START

class GameObject {
public:
   GameObject();

private:
   Vector3 m_position;
   Vector3 m_rotation;

public:
   KSI_API Render::DrawObject GetDrawObject();
};

KSI_END