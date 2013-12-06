#include <limits>
#include <stdexcept>
#include <iostream>

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
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = -std::numeric_limits<float>::max();

    unsigned int vid0, vid1, vid2;
    std::vector<Vec3f> vec;
    vec.reserve(faceIDs->size());

    for(unsigned int i = 0; i < faceIDs->size(); i++) {
        int x = faceIDs->operator [](i);
        vid0 = mesh.faces[x * 3 + 0];
        vid1 = mesh.faces[x * 3 + 1];
        vid2 = mesh.faces[x * 3 + 2];

        vec[0] = mesh.vertices[vid0];
        vec[1] = mesh.vertices[vid1];
        vec[2] = mesh.vertices[vid2];

        for (int i=0; i < 3; i++){
            if (vec[i][0] < minX){
                minX = vec[i][0];
            }
            if (vec[i][0] > maxX){
                maxX = vec[i][0];
            }

            if (vec[i][1] < minY){
                minY = vec[i][1];
            }
            if (vec[i][1] > maxY){
                maxY = vec[i][1];
            }

            if (vec[i][2] < minZ){
                minZ = vec[i][2];
            }
            if (vec[i][2] > maxZ){
                maxZ = vec[i][2];
            }
        }
    }

    Vec3f minI = Vec3f(minX, minY, minZ);
    Vec3f maxI = Vec3f(maxX, maxY, maxZ);

    this->aabb = AABB(minI, maxI);

    if (faceIDs->size() <= MAX_LEAF_TRIANGLES){
        this->triangles.reserve(faceIDs->size());
        for(unsigned int i = 0; i < faceIDs->size(); i++){
            unsigned int id = faceIDs->operator [](i);
            this->triangles[i] = Triangle(&mesh, id);
        }
    }
    else{
        // Liste mit dreiecken.
        std::vector<Triangle> allTriangles;
        allTriangles.reserve(faceIDs->size());
        for(unsigned int i = 0; i < faceIDs->size(); i++){
            unsigned int id = faceIDs->operator [](i);
            allTriangles[i] = Triangle(&mesh, id);
        }

        // Liste mit Schwerpunkten
        std::vector<Vec3f> centroids;
        centroids.reserve(faceIDs->size());
        for(unsigned int i = 0; i < faceIDs->size(); i++){
            centroids[i] = allTriangles[i].getCentroid();
        }

        // BB um die Schwerpunkte
        minX = minY = minZ = std::numeric_limits<float>::max();
        maxX = maxY = maxZ = -std::numeric_limits<float>::max();
        for (int i=0; i < 3; i++){
            if (centroids[i][0] < minX){
                minX = centroids[i][0];
            }
            if (centroids[i][0] > maxX){
                maxX = centroids[i][0];
            }

            if (centroids[i][1] < minY){
                minY = centroids[i][1];
            }
            if (centroids[i][1] > maxY){
                maxY = centroids[i][1];
            }

            if (centroids[i][2] < minZ){
                minZ = centroids[i][2];
            }
            if (centroids[i][2] > maxZ){
                maxZ = centroids[i][2];
            }
        }
        minI = Vec3f(minX, minY, minZ);
        maxI = Vec3f(maxX, maxY, maxZ);

        AABB centroidsBB = AABB(minI, maxI);

        // Längste Achse der BB
        unsigned int longestAxis = centroidsBB.getLongestAxis();

        // BB teilen
        // 2 neuen BB erstellen. damit lässt sich inside() für jeden Punkt prüfen.
        // liste der faceIDs für links und rechts anlegen
        // 2 rekursive Aufrufe
    }

}

bool BVHNode::intersect(Ray const& ray, Intersection* intersection) const
{
    // TODO
    return false;
}
