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
    this->aabb = Triangle(&mesh, faceIDs->at(0)).getAABB();
    AABB centroidsBB(Triangle(&mesh, faceIDs->at(0)).getCentroid(), Triangle(&mesh, faceIDs->at(0)).getCentroid());

    for(unsigned int i = 0; i < faceIDs->size(); i++) {
        Triangle t = Triangle(&mesh,faceIDs->at(i));
        this->aabb.merge(t.getAABB());
        centroidsBB.merge(t.getCentroid());
    }

    if (faceIDs->size() <= MAX_LEAF_TRIANGLES){
        for(unsigned int i = 0; i < faceIDs->size(); i++){
            unsigned int id = faceIDs->operator [](i);
            this->triangles.push_back(Triangle(&mesh, id));
        }
        return;
    }
    else{
         // Längste Achse der BB
        unsigned int longestAxis = centroidsBB.getLongestAxis();

        // BB teilen
        // liste der faceIDs für links und rechts anlegen
        std::vector<unsigned int> faceLeft, faceRight;

        float median = centroidsBB.getAABBMin()[longestAxis] + centroidsBB.getAABBMax()[longestAxis];
        median = median / 2;

        AABB leftHalf,rightHalf;

        leftHalf.setMin(centroidsBB.getAABBMin());
        switch(longestAxis){
            case 0:{
                leftHalf.setMax(Vec3f(median, centroidsBB.getAABBMax()[1], centroidsBB.getAABBMax()[2]));
                rightHalf.setMin(Vec3f(median, centroidsBB.getAABBMin()[1], centroidsBB.getAABBMin()[2]));
                rightHalf.setMax(centroidsBB.getAABBMax());
                break;
            }
            case 1:{
                leftHalf.setMax(Vec3f(centroidsBB.getAABBMax()[0], median, centroidsBB.getAABBMax()[2]));
                rightHalf.setMin(Vec3f(centroidsBB.getAABBMin()[0], median, centroidsBB.getAABBMin()[2]));
                rightHalf.setMax(centroidsBB.getAABBMax());
                break;
            }
            case 2:{
                leftHalf.setMax(Vec3f(centroidsBB.getAABBMax()[0], centroidsBB.getAABBMax()[1], median));
                rightHalf.setMin(Vec3f(centroidsBB.getAABBMin()[0], centroidsBB.getAABBMin()[1], median));
                rightHalf.setMax(centroidsBB.getAABBMax());
                break;
            }
        }

        for(unsigned int i = 0; i < faceIDs->size(); i++){
            Triangle t = Triangle(&mesh,faceIDs->at(i));

            if(leftHalf.inside(t.getCentroid())){
                faceLeft.push_back(faceIDs->at(i));
            }
            if(rightHalf.inside(t.getCentroid())){
                faceRight.push_back(faceIDs->at(i));
            }
        }


        // 2 rekursive Aufrufe
        this->left = new BVHNode();
        this->right = new BVHNode();

        std::cout<<faceLeft.size()<<" - "<<faceRight.size()<<std::endl;

        this->left->insert(mesh, &faceLeft);
        this->right->insert(mesh, &faceRight);

    }

    return;
}


bool BVHNode::intersect(Ray const& ray, Intersection* intersection) const
{
    if(this->aabb.intersect(ray)) {
        if(this->left != NULL && this->right != NULL) {
                this->left->intersect(ray, intersection);
                this->right->intersect(ray, intersection);
        }
        else if(this->right == NULL && this->left == NULL) {
            for(unsigned i = 0; i < this->triangles.size(); i++) {
                this->triangles[i].intersect(ray, intersection);
            }
        }
        return true;
    } else return false;
}
