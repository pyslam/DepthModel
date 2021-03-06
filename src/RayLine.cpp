#include <opencv2/opencv.hpp>
#include "RayLine.hpp"

using namespace cv;

namespace DepthModel {


/**
 * RayLine definition
 */

RayLine::RayLine(Vec3f _endPoint, Vec3f _direction)
  : endPoint(_endPoint)
  , direction(normalize(_direction))
{}

RayLine::RayLine(const RayLine& other)
  : RayLine(other.endPoint, other.direction)
{}

Vec3f RayLine::projection(const Vec3f & point) const {
    // Yo here is some math :D
    // The projection point of input point on the ray
    // is actually the projection point of the ray's endpoint
    // on the plane cross the input point with ray's direction
    // as normal vector.
    // And the solution is here:
    // https://math.stackexchange.com/a/100766
    float t = this->direction.dot(point - this->endPoint);
#define sqr(x) ((x) * (x))
    t /= sqr(this->direction[0]) + sqr(this->direction[1]) + sqr(this->direction[2]);
#undef sqr
    return this->endPoint + this->direction * t;
}

RayLine& RayLine::operator=(const RayLine& other) {
    this->endPoint = other.endPoint;
    this->direction = other.direction;
    return *this;
}

viz::WArrow RayLine::toVizWidget(float length) {
    return viz::WArrow(Point3d(this->endPoint), Point3d(this->endPoint + length * this->direction), 0.005);
}


/**
 * ColorRayLine definition
 */
ColorRayLine::ColorRayLine(
    Vec3f _endPoint, Vec3f _direction, Vec3b _color
)
    : RayLine(_endPoint, _direction)
    , color(_color)
{}

ColorRayLine::ColorRayLine(const ColorRayLine& other)
    : ColorRayLine(other.endPoint, other.direction, other.color)
{}


ColorRayLine& ColorRayLine::operator=(const ColorRayLine& other) {
    this->endPoint = other.endPoint;
    this->direction = other.direction;
    this->color = other.color;
    return *this;
}

bool operator==(const RayLine& u, const RayLine& v) {
    return u.endPoint == v.endPoint && u.direction == v.direction;
}

bool operator==(const ColorRayLine& u, const ColorRayLine& v) {
    return (
        u.endPoint == v.endPoint &&
        u.direction == v.direction &&
        u.color == v.color
    );
}

viz::WArrow ColorRayLine::toVizWidget(float length) {
    return viz::WArrow(
        Point3d(this->endPoint),
        Point3d(this->endPoint + length * direction),
        0.05 / length,
        viz::Color(this->color[0], this->color[1], this->color[2])
    );
}

} // DepthModel
