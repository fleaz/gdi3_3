#ifndef BVH_NODE_H
#define BVH_NODE_H

#include <vector>

#include "mesh.h"
#include "ray.h"
#include "triangle.h"

// Maximum number of triangles in the leaf nodes.
// In your implementation, make sure leafs contain at most this many triangles.
// What is the tradeoff when increasing/decreasing this number?
#define MAX_LEAF_TRIANGLES 10

// A single node in the BVH. Every node stores its left and right children
// or NULL if the node is a leaf. If the node is a leaf, it contains
// a list of triangles.
//
// The insert method takes a list of face ID as input. To obtain the
// vertices corresponding a faceID, the following has to be done:
//
//   /* Vertex IDs corresponding to the face. */
//   unsigned int vid0 = mesh.faces[faceID * 3 + 0];
//   unsigned int vid1 = mesh.faces[faceID * 3 + 1];
//   unsigned int vid2 = mesh.faces[faceID * 3 + 2];
//   /* Vertices corresponding to the vertex IDs. */
//   Vec3f v0 = mesh.vertices[vid0];
//   Vec3f v1 = mesh.vertices[vid1];
//   Vec3f v2 = mesh.vertices[vid2];
//
// The mesh itself remains unchanged during recursion. The list of faceIDs
// is passed as pointer such that the data structure can be changed. This
// is useful to free memory once the faceIDs are not needed anymore before
// recursion.
//
// IMPORTANT: This interface can be extended, but existing public
// functions must not be changed.
class BVHNode
{
public:
    BVHNode();
    ~BVHNode();

    void insert(Mesh const& mesh, std::vector<unsigned int>* faceIDs);
    bool intersect(Ray const& ray, Intersection* intersection) const;

public:
    AABB aabb;
    std::vector<Triangle> triangles;
    BVHNode* left;
    BVHNode* right;
};

#endif // BVH_NODE_H
