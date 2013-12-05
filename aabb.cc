#include "aabb.h"

void AABB::merge(AABB const& bb)
{
    for (int i = 0; i < 3; ++i)
    {
        this->min[i] = std::min(this->min[i], bb.min[i]);
        this->max[i] = std::max(this->max[i], bb.max[i]);
    }
}

void AABB::merge(Vec3f const& vec)
{
    for (int i = 0; i < 3; ++i)
    {
        this->min[i] = std::min(this->min[i], vec[i]);
        this->max[i] = std::max(this->max[i], vec[i]);
    }
}

int AABB::getLongestAxis() const
{
    Vec3f diff = this->max - this->min;
    if (diff[0] >= diff[1] && diff[0] >= diff[2])
        return 0;
    if (diff[1] >= diff[0] && diff[1] >= diff[2])
        return 1;
    return 2;
}

bool AABB::inside(Vec3f const& point) const
{
    for (int i = 0; i < 3; ++i)
        if (point[i] > this->max[i] || point[i] < this->min[i])
            return false;
    return true;
}

bool AABB::intersect(Ray const& ray) const
{
    // Smits ray-box intersection test using slabs
    // http://www.cs.utah.edu/~awilliam/box/box.pdf
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    float div = 1.0f / ray.direction[0];
    if (div >= 0)
    {
        tmin = (this->min[0] - ray.position[0]) * div;
        tmax = (this->max[0] - ray.position[0]) * div;
    }
    else
    {
        tmin = (this->max[0] - ray.position[0]) * div;
        tmax = (this->min[0] - ray.position[0]) * div;
    }

    div = 1 / ray.direction[1];
    if (div >= 0)
    {
        tymin = (this->min[1] - ray.position[1]) * div;
        tymax = (this->max[1] - ray.position[1]) * div;
    }
    else
    {
        tymin = (this->max[1] - ray.position[1]) * div;
        tymax = (this->min[1] - ray.position[1]) * div;
    }

    if (tmin > tymax || tymin > tmax)
        return false;

    tmin = std::max(tmin, tymin);
    tmax = std::min(tmax, tymax);

    div = 1 / ray.direction[2];
    if (div >= 0)
    {
        tzmin = (this->min[2] - ray.position[2]) * div;
        tzmax = (this->max[2] - ray.position[2]) * div;
    }
    else
    {
        tzmin = (this->max[2] - ray.position[2]) * div;
        tzmax = (this->min[2] - ray.position[2]) * div;
    }

    if (tmin > tzmax || tzmin > tmax)
        return false;

    tmin = std::max(tmin, tzmin);
    tmax = std::min(tmax, tzmax);

    return tmin < 100000.0f && tmax > 0.0f;
}
