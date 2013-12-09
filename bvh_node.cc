#include <limits>
#include <stdexcept>
#include <iostream>

#include "visual.h"
#include "bvh_node.h"

#include <cmath>

BVHNode::BVHNode()
{
    this->left = NULL;
    this->right = NULL;
    this->triangles.reserve(MAX_LEAF_TRIANGLES);
}

BVHNode::~BVHNode()
{
    delete this->left;
    delete this->right;
}

void BVHNode::insert(Mesh const& mesh, std::vector<unsigned int>* faceIDs)
{
    std::cout<<"start"<<std::endl;
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = -std::numeric_limits<float>::max();
    unsigned int vid0, vid1, vid2;
    std::vector<Vec3f> vec;
    vec.reserve(3);

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
        for(unsigned int i = 0; i < faceIDs->size(); i++){
            this->triangles.push_back(Triangle(&mesh, i));
            std::cout<<"triangle"<<std::endl;
        }
        std::cout<<"penis"<<std::endl;
    }
    else{
        // Liste mit dreiecken.
        std::vector<Triangle> allTriangles;
        allTriangles.reserve(faceIDs->size());
        for(unsigned int i = 0; i < faceIDs->size(); i++){
            allTriangles.push_back(Triangle(&mesh, i));
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
        // liste der faceIDs für links und rechts anlegen
        std::vector<unsigned int> faceLeft, faceRight;
        faceLeft.reserve(faceIDs->size());
        faceRight.reserve(faceIDs->size());

        float median = centroidsBB.getAABBMin()[longestAxis] + centroidsBB.getAABBMax()[longestAxis];
        median = median / 2;
        //std::cout<<"Longest: "<<longestAxis<<std::endl;
        //std::cout<<"Min: "<<this->aabb.getAABBMin()[0]<<" "<<this->aabb.getAABBMin()[1]<<" "<<this->aabb.getAABBMin()[2]<<std::endl;
        //std::cout<<"Max: "<<this->aabb.getAABBMax()[0]<<" "<<this->aabb.getAABBMax()[1]<<" "<<this->aabb.getAABBMax()[2]<<std::endl;

        //std::cout<<"Median: "<<median<<std::endl;

        for(unsigned int i = 0; i < faceIDs->size(); i++){
            //std::cout<<"Punkt: "<<centroids[i][0]<<" "<<centroids[i][1]<<" "<<centroids[i][2]<<std::endl;
            if(centroids[i][longestAxis] <= median){
                faceLeft.push_back(faceIDs->operator [](i));
                std::cout<<"Links"<<std::endl;
            }
            else{
                faceRight.push_back(faceIDs->operator [](i));
                std::cout<<"Rechts"<<std::endl;
            }
        }
        // 2 rekursive Aufrufe
        delete this->left;
        delete this->right;
        this->left = new BVHNode();
        this->right = new BVHNode();


        std::cout<<"Links: "<<faceLeft.size()<<std::endl;
        std::cout<<"Rechts: "<<faceRight.size()<<std::endl;
        std::cout<<"#########################"<<std::endl;

        std::cout<<"Linke Rekursion:"<<std::endl;
        this->left->insert(mesh, &faceLeft);
        std::cout<<"Rechte Rekursion:"<<std::endl;
        this->right->insert(mesh, &faceRight);

    }

    return;

}

bool BVHNode::intersect(Ray const& ray, Intersection* intersection) const
{
    // Hier happens the Magic <(O,O)>
    return false;
}
