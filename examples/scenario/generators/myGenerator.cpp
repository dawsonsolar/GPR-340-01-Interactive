#include "MyGenerator.h"
#include "../PerlinNoise.hpp"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include <iostream>
#include <cmath>

std::vector<Color32> MyGenerator::Generate(int sideSize, float displacement)
{
    std::vector<Color32> colors;
    siv::PerlinNoise perlinNoise(sideSize);
    float octave = perlinNoise.octave3D_01(1, 1, 1, 3);

    if (previousSideSize != sideSize)
    {
        heightMap.clear();
        heightMap.reserve(sideSize * sideSize);

        for (int i = 0; i < sideSize; ++i)
        {
            for (int j = 0; j < sideSize; ++j)
            {
                float posX = static_cast<float>(i - sideSize / 2) / (sideSize / 2);
                float posY = static_cast<float>(j - sideSize / 2) / (sideSize / 2);
                float islandInfluence = (1 - posX * posX) * (1 - posY * posY);
                float noise = (perlinNoise.noise3D_01(posY * 2, posX * 2, displacement * 50) + 1) / 2;
                float heightValue = std::lerp(0.0f, 1.0f, 0.5f) * octave * 1.8f + islandInfluence * 0.4f + noise * 0.7f;
                heightMap.push_back(heightValue);
            }
        }
    }
    colors.reserve(heightMap.size());
    for (const auto& height : heightMap)
    {
        if (height < 1.0f)
        {
            colors.push_back(Color32::LerpColor(Color::DarkBlue, Color::Blue, height / 1.0f));
        } else if (height < 1.15f)
        {
            colors.push_back(Color32::LerpColor(Color::Blue, Color::DarkGoldenrod, (height - 1.0f) / 0.15f));
        } else if (height < 1.3f)
        {
            colors.push_back(Color32::LerpColor(Color::DarkGoldenrod, Color::ForestGreen, (height - 1.15f) / 0.15f));
        } else if (height < 1.45f)
        {
            colors.push_back(Color32::LerpColor(Color::ForestGreen, Color::Tan, (height - 1.3f) / 0.15f));
        } else if (height < 1.6f)
        {
            colors.push_back(Color32::LerpColor(Color::Tan, Color::White, (height - 1.45f) / 0.15f));
        }
        else
        {
            colors.push_back(Color::White);
        }
    }

    previousSideSize = sideSize;
    return colors;
}

std::string MyGenerator::GetName()
{
    return "My Generator";
}

std::vector<float> MyGenerator::HydraulicErosion(const std::vector<float>& heightMap, int sideSize)
{
    std::vector<float> updatedHeightMap = heightMap;
    float evaporationFactor = 0.1f;

    Vector2<float> position(static_cast<float>(rand() % sideSize), static_cast<float>(rand() % sideSize));
    float waterRemaining = 150.0f;

    while (waterRemaining > 0.0f)
    {
        std::vector<Vector3> neighbors;
        neighbors.reserve(8);


        if (position.x > 0) neighbors.emplace_back(position.x - 1, position.y, updatedHeightMap[position.y * sideSize + position.x - 1]);
        if (position.y > 0) neighbors.emplace_back(position.x, position.y - 1, updatedHeightMap[(position.y - 1) * sideSize + position.x]);
        if (position.y < sideSize - 1) neighbors.emplace_back(position.x, position.y + 1, updatedHeightMap[(position.y + 1) * sideSize + position.x]);
        if (position.x < sideSize - 1) neighbors.emplace_back(position.x + 1, position.y, updatedHeightMap[position.y * sideSize + position.x + 1]);

        auto minNeighbor = *std::min_element(neighbors.begin(), neighbors.end(), [](const Vector3& a, const Vector3& b) {
            return a.z < b.z;
        });


        if (minNeighbor.z >= updatedHeightMap[position.y * sideSize + position.x]) {
            break;
        }


        if (updatedHeightMap[minNeighbor.y * sideSize + minNeighbor.x] > 1.15f) {
            updatedHeightMap[minNeighbor.y * sideSize + minNeighbor.x] -= 0.01f;
        }

        position.x = minNeighbor.x;
        position.y = minNeighbor.y;
        waterRemaining -= evaporationFactor;
    }

    return updatedHeightMap;
}
