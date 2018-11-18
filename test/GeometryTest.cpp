
#include "GeometryTest.h"
#include "mathnphysics/Geometry.h"

bool AreSame(float a, float b) {
  return fabs(a - b) < 0.01f;
}

void GeometryTest_PointClosestToLineSegment() {
  LineSegment line_segment{glm::vec2(2, 1), glm::vec2(1, 2)};

  glm::vec2 point_result = Geometry::closestPointInLineSegmentToPoint(line_segment, glm::vec2(0,0));
  assert(glm::vec2(1.5f, 1.5f) == point_result);
}

void GeometryTest_closestPointInLineSegmentToPoint() {
  LineSegment segment{glm::vec2(0, 0), glm::vec2(0, 1)};
  glm::vec2 result;
  result = Geometry::closestPointInLineSegmentToPoint(segment, glm::vec2(1, 0));
  assert(result == glm::vec2(0, 0));

  result = Geometry::closestPointInLineSegmentToPoint(segment, glm::vec2(0, 0));
  assert(result == glm::vec2(0, 0));

  result = Geometry::closestPointInLineSegmentToPoint(segment, glm::vec2(1, 2));
  assert(result == glm::vec2(0, 1));
 
  result = Geometry::closestPointInLineSegmentToPoint(segment, glm::vec2(-1, 0));
  assert(result == glm::vec2(0, 0));

  result = Geometry::closestPointInLineSegmentToPoint(segment, glm::vec2(0, 0.5));
  assert(result == glm::vec2(0, 0.5));

  result = Geometry::closestPointInLineSegmentToPoint(segment, glm::vec2(-1, -1));
  assert(result == glm::vec2(0, 0));
}

void GeometryTest::runTests() {
  GeometryTest_PointClosestToLineSegment();
  GeometryTest_closestPointInLineSegmentToPoint();
}

