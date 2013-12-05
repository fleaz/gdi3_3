#include <limits>
#include <stdexcept>

#include "visual.h"
#include "bvh_node.h"

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
    // TODO
}

bool BVHNode::intersect(Ray const& ray, Intersection* intersection) const
{
    // TODO
    return false;
}
