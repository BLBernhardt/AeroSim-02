#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "../0-Math/vector3.h"

class Heightmap {
public:
  Heightmap(): m_initialized(false) { };
  bool is_initialized() { return m_initialized; };

  float   GetHeightAt(float x, float y);
  Vector3 GetNormalAt(float x, float y);
  
private:
  bool m_initialized;
};

#endif
