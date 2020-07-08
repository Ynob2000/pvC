#pragma once
#include "hlapi/hlapi.h"
#include "vmath.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <codecvt>
#include <locale>
#include <random>
#include <chrono>
#include <list>
#include <map>
#include <math.h>

uint64_t ReadPtrChain(WinProcess *GameProc, uint64_t Base, std::vector<uint64_t> Chain);

bool WorldToScreen(Matrix4f CameraMatrix, Vector3f WorldPosition, Vector2f &ScreenPosition, float width, float height);

float clamp(float x, float min, float max);

float CalcInFov(const Vector3f& position, Matrix4f& cameraMatrix, const Vector3f& vector);

Vector3f toEuler(const Vector3f& v, float angle);

Vector3f get_transform_position(WinProcess* GameProcess, uint64_t transform);