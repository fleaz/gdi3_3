#include "triangle.h"

Vec3f Triangle::getCentroid() const
{
    return ((*this)[0] + (*this)[1] + (*this)[2]) / 3.0f;
}

Vec3f Triangle::getAABBMin() const
{
    Vec3f min;
    min[0] = std::min((*this)[0][0], std::min((*this)[1][0], (*this)[2][0]));
    min[1] = std::min((*this)[0][1], std::min((*this)[1][1], (*this)[2][1]));
    min[2] = std::min((*this)[0][2], std::min((*this)[1][2], (*this)[2][2]));
    return min;
}

Vec3f Triangle::getAABBMax() const
{
    Vec3f max;
    max[0] = std::max((*this)[0][0], std::max((*this)[1][0], (*this)[2][0]));
    max[1] = std::max((*this)[0][1], std::max((*this)[1][1], (*this)[2][1]));
    max[2] = std::max((*this)[0][2], std::max((*this)[1][2], (*this)[2][2]));
    return max;
}

AABB Triangle::getAABB() const
{
    return AABB(this->getAABBMin(), this->getAABBMax());
}

Vec3f Triangle::getNormalVector() const
{
    Vec3f n = ((*this)[1] - (*this)[0]).cross((*this)[2] - (*this)[0]);
    return n / n.length();
}

/*
 * Src: http://geomalgorithms.com/a06-_intersect-2.html
 */
bool Triangle::intersect(Ray const& ray, Intersection* intersection) const
{
    float const EPSILON2 = 0.000001;

    // get triangle edge vectors and plane normal
    Vec3f u = (*this)[1] - (*this)[0];
    Vec3f v = (*this)[2] - (*this)[0];
    Vec3f n = u.cross(v);              // cross product

    Vec3f w0 = ray.position - (*this)[0];
    float a = -n.dot(w0);
    float b = n.dot(ray.direction);

    // Check if ray is parallel to triangle plane.
    if (fabs(b) < EPSILON2)
        return false;

    // get intersect point of ray with triangle plane
    float r = a / b;
    if (r < 0.0)                    // ray goes away from triangle
        return false;                   // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    Vec3f intersectionPoint = ray.position + (ray.direction*r);            // intersect point of ray and plane

    // is I inside T?
    float uu = u.dot(u);
    float uv = u.dot(v);
    float vv = v.dot(v);
    Vec3f w = intersectionPoint - (*this)[0];
    float wu = w.dot(u);
    float wv = w.dot(v);
    float D = uv * uv - uu * vv;

    // get and test parametric coords
    float s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)         // I is outside T
        return false;
    float t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return false;

    // Intersection looks good. Fill result.
    float const distance = (intersectionPoint - ray.position).length();
    if (intersection->distance > distance)
    {
        intersection->position = intersectionPoint;
        intersection->normal = this->getNormalVector();
        intersection->distance = distance;
    }

    return true;
}
