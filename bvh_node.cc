#include <limits>
#include <stdexcept>

#include "visual.h"
#include "bvh_node.h"

#include <cmath>

BVHNode::BVHNode()
{
    // TODO
}

BVHNode::~BVHNode()
{
    // TODO
}

void BVHNode::insert(Mesh const& mesh, std::vector<unsigned int>* faceIDs)
{
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    minX = minY = minZ = 9999999.99;
    maxX = maxY = maxZ = -9999999.99;

    unsigned int vid0, vid1, vid2;
    Vec3f v0, v1, v2;

    for(unsigned int i = 0; i < faceIDs->size(); i++) {
        int x = faceIDs->operator [](i);
        vid0 = mesh.faces[x * 3 + 0];
        vid1 = mesh.faces[x * 3 + 1];
        vid2 = mesh.faces[x * 3 + 2];

        v0 = mesh.vertices[vid0];
        v1 = mesh.vertices[vid1];
        v2 = mesh.vertices[vid2];

        if(v0[0] < minX || v1[0] < minX || v2[0] < minX) {
            minX = fmin(v0[0], fmin(v1[0], v2[0]));
        }

        if(v0[1] < minY || v1[1] < minY || v2[1] < minY) {
            minY = fmin(v0[1], fmin(v1[1], v2[1]));
        }

        if(v0[2] < minZ || v1[2] < minZ || v2[2] < minZ) {
            minX = fmin(v0[2], fmin(v1[2], v2[2]));
        }

        if(v0[0] > maxX || v1[0] > maxX || v2[0] > maxX) {
            maxX = fmax(v0[0], fmax(v1[0], v2[0]));
        }

        if(v0[1] > maxY || v1[1] >maxY || v2[1] > maxY) {
            maxY = fmax(v0[1], fmax(v1[1], v2[1]));
        }

        if(v0[2] > maxZ || v1[2] > maxZ || v2[2] > maxZ) {
            maxZ = fmax(v0[2], fmax(v1[2], v2[2]));
        }
    }

    Vec3f minI = Vec3f(minX, minY, minZ);
    Vec3f maxI = Vec3f(maxX, maxY, maxZ);

    this->aabb = AABB(minI, maxI);
}

bool BVHNode::intersect(Ray const& ray, Intersection* intersection) const
{
    // TODO
    return false;
}
