#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "vec3.h"
#include "aabb.h"
#include "ray.h"
#include "mesh.h"

// Representation of a mesh/ray intersection.
//
// - position: The position of the intersection point
// - normal: The normal of the triangle which caused the intersection
// - distance: The distance from the ray origin to the intersection point.
//     This field is also used to test whether an intersection happened.
//     In case of failure, it is set to 0.0f.
struct Intersection
{
    Vec3f position;
    Vec3f normal;
    float distance;
};

// Representation of a triangle which contains an intersection test
// and a few convenience functions.
class Triangle
{
public:
    Triangle(void);
    Triangle(Mesh const* mesh, unsigned int faceID);

    Vec3f getCentroid(void) const;
    Vec3f getNormalVector(void) const;
    Vec3f getAABBMin(void) const;
    Vec3f getAABBMax(void) const;
    AABB getAABB(void) const;

    // Computes an intersection between a ray and the triangle.
    // Returns true iff the ray hits the triangle. If this case, additional
    // information is returned in the intersection field.
    bool intersect(Ray const& ray, Intersection* intersection) const;

    Vec3f const& operator[] (int index) const;
    Vec3f& operator[] (int index);

private:
    Mesh const* mesh;
    unsigned int faceID;
};

/* ------------------------ Implementation ------------------------ */

inline Triangle::Triangle(void)
    : mesh(NULL)
{
}

inline Triangle::Triangle(Mesh const* mesh, unsigned int faceID)
    : mesh(mesh), faceID(faceID)
{
}

inline Vec3f const& Triangle::operator[] (int index) const
{
    return this->mesh->vertices[this->mesh->faces[this->faceID * 3 + index]];
}

#endif /* TRIANGLE_H_ */
