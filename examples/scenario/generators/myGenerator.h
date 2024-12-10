// Optimized MyGenerator.h

#ifndef MYGENERATOR_H
#define MYGENERATOR_H

#include "../GeneratorBase.h"
#include <vector>
#include <string>

class MyGenerator : public ScenarioGeneratorBase {
public:
  std::vector<Color32> Generate(int sideSize, float displacement = 0.0f) override;
  std::string GetName() override;
  std::vector<float> HydraulicErosion(const std::vector<float>& heightMap, int sideSize);

private:
  std::vector<float> heightMap;
  int previousSideSize = 0;
};

#endif // MYGENERATOR_H