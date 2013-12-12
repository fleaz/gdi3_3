#include <limits>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "timer.h"
#include "mesh.h"
#include "vec3.h"
#include "bvh.h"
#include "triangle.h"
#include "visual.h"

// Generates a gray scale image color for a ray/intersection combination.
unsigned char generateColor(Ray const& ray, Intersection const& intersection)
{
    if (intersection.distance == 0)
        return 0;
    float shading = -ray.direction.dot(intersection.normal);
    return std::min(1.0f, std::max(0.0f, shading)) * 255.0f;
}

// Renders an image with hard-coded camera parameters.
void renderImage(BVH const& bvh, std::string const& outImage)
{
    // Assume mesh is centered and AABB size is 1^3. The camera is
    // centered at (0, 0, 2) and looks along the negative z-axis.
    // The field of view is 2 * atan(0.5) ~ 53 degree (focal length 1).

    // Setup viewing parameters.
    int const width = 800;
    int const height = 600;
    float const focal_length = 1.0f;
    float const ax = focal_length * (width > height ? width : height);
    float const ay = focal_length * (width > height ? width : height);

    // Generate image.
    std::vector<unsigned char> image(width * height, 0);
    for (int i = 0, y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x, ++i)
        {
            Ray ray;
            ray.position = Vec3f(0.0f, 0.0f, 2.0f);
            ray.direction = Vec3f((x + 0.5f) / ax - width / (2.0f * ax),
                (y + 0.5f) / ay - height / (2.0f * ay), 1.0f);
            ray.direction[1] *= -1.0f;  // Invert y-axis for image.
            ray.direction[2] *= -1.0f;  // Look along negative z-axis.
            ray.direction = ray.direction / ray.direction.length();

            Intersection intersection;
            bvh.intersect(ray, &intersection);
            image[i] = generateColor(ray, intersection);
        }

    // Write output image.
    std::ofstream out(outImage.c_str());
    if (!out.good())
    {
        std::cerr << "Error opening output file!" << std::endl;
        return;
    }
    out << "P5 " << width << " " << height << " 255\n";
    out.write(reinterpret_cast<char const*>(&image[0]), width * height);
    out.close();
}

// Main routine.
int main (int argc, char** argv)
{
    // Check arguments.
    if (argc != 3)
    {
        std::cerr << "Syntax: " << argv[0] << " <IN_MESH.OFF> <OUT_IMAGE.PGM>" << std::endl;
        return 1;
    }
    std::string inMesh(argv[1]);
    std::string outImage(argv[2]);

    // Read input mesh.
    std::cout << "Reading input mesh..." << std::endl;
    Mesh mesh;
    load_off_mesh(inMesh, &mesh);
    std::cout << "  Vertices: " << mesh.vertices.size() << std::endl;
    std::cout << "  Triangles: " << (mesh.faces.size() / 3) << std::endl;

    // Build BVH.
    std::cout << "Building BVH..." << std::flush;
    BVH bvh;
    {
        ClockTimer timer;
        bvh.buildBVH(mesh);
        std::cout << " took " << timer.get_elapsed() << "ms." << std::endl;
    }
    bvh.printStatistics(std::cout);

    // Render output image.
    std::cout << "Rendering image..." << std::flush;
    {
        ClockTimer timer;
        renderImage(bvh, outImage);
        std::cout << " took " << timer.get_elapsed() << "ms." << std::endl;
    }

    // Change the following to "#if 1" to use the code for debugging.
    // The code visualizes the BVH bounding boxes using 3D points.
    // The resulting mesh can be loaded with, for example, Meshlab or UMVE.
    // Warning: On bigger models, the resulting mesh will be huge.
#if 0
    // Visualize BVH.
    std::cout << "Visualizing BVH..." << std::endl;
    Mesh meshBVH;
    bvh.addAABBsToMesh(&meshBVH, 10);
    std::cout << "  Vertices: " << meshBVH.vertices.size() << std::endl;
    std::cout << "  Triangles: " << (meshBVH.faces.size() / 3) << std::endl;
    std::cout << "Saving BVH visualization..." << std::endl;
    save_off_mesh(meshBVH, "/tmp/mesh_bvh.off");
#endif

    return 0;
}
