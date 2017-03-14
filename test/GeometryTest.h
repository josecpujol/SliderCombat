#pragma once

#include "../src/Geometry.h"

bool AreSame(float a, float b) {
  return fabs(a - b) < 0.01f;
}

void GeometryTest_PointClosestToLineSegment() {
  LineSegment line_segment;
  line_segment.points[0] = glm::vec2(2, 1);
  line_segment.points[1] = glm::vec2(1, 2);

  glm::vec2 point_result = Geometry::closestPointInLineSegmentToPoint(line_segment, glm::vec2(0,0));
  assert(glm::vec2(1.5f, 1.5f) == point_result);
}

void runTests() {
  GeometryTest_PointClosestToLineSegment();
}

