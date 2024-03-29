#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <iostream>

#include "vec3.h"
#include "mesh.h"

void
load_off_mesh (std::string const& filename, Mesh* mesh)
{
    if (filename.empty())
        throw std::invalid_argument("No filename given");

    /* Open file. */
    std::fstream input(filename.c_str());
    if (input.fail())
        throw std::runtime_error("Cannot read file");

    /* Start parsing. */
    std::string buffer;

    /* Read "OFF" file signature. */
    input >> buffer;
    if (buffer != "OFF")
    {
        input.close();
        throw std::runtime_error("File not recognized as OFF model");
    }

    /* Clear the model data and init some values. */
    std::size_t num_vertices = 0;
    std::size_t num_faces = 0;
    std::size_t num_edges = 0;

    /* Read vertex, face and edge information. */
    input >> num_vertices >> num_faces >> num_edges;
    mesh->vertices.reserve(num_vertices);
    mesh->faces.reserve(num_faces * 3 +3);

    /* Read vertices. */
    for (std::size_t i = 0; i < num_vertices; ++i)
    {
        float x, y, z;
        input >> x >> y >> z;
        mesh->vertices.push_back(Vec3f(x, y, z));
    }

    /* Read faces. */
    for (std::size_t i = 0; i < num_faces; ++i)
    {
        std::size_t n_vertices;
        input >> n_vertices;

        if (n_vertices == 3)
        {
            /* Polygon is a triangle. */
            unsigned int vidx[3];
            bool indices_good = true;
            for (int j = 0; j < 3; ++j)
            {
                input >> vidx[j];
                if (vidx[j] >= num_vertices)
                {
                    std::cout << "OFF Loader: Warning: Face " << i
                        << " has invalid vertex " << vidx[j]
                        << ", skipping face." << std::endl;
                    indices_good = false;
                }
            }

            if (indices_good)
                for (int j = 0; j < 3; ++j)
                    mesh->faces.push_back(vidx[j]);
        }
        else
            throw std::runtime_error("Invalid face with != 3 vertices");
    }


  /* Close file stream. */
  input.close();
}

/* ---------------------------------------------------------------- */

void
save_off_mesh (Mesh const& mesh, std::string const& filename)
{
    if (filename.empty())
        throw std::invalid_argument("No filename given");

    /* Open file for writing. */
    std::ofstream out(filename.c_str());
    if (out.fail())  throw std::runtime_error("File Exception.\n");
        //throw util::FileException(filename, std::strerror(errno));

    std::size_t num_verts = mesh.vertices.size();
    std::size_t num_faces = mesh.faces.size() / 3;

    /* Write the header. */
    out << "OFF" << std::endl;

    /* Write number of vertices and faces (and 0 for edges). */
    out << num_verts << " " << num_faces << " 0" << std::endl;

    /* Write at least 7 digits for float values. */
    out << std::fixed << std::setprecision(7);

    /* Write vertices. */
    for (std::size_t i = 0; i < num_verts; i+= 3)
    {
        out << mesh.vertices[i+0] << "\n" << mesh.vertices[i+1]
            << "\n" << mesh.vertices[i+2] << std::endl;
    }

    /* Write faces. */
    for (std::size_t i = 0; i < num_faces * 3; i += 3)
    {
        out << "3 " << mesh.faces[i + 0] << " "
            << mesh.faces[i + 1] << " " << mesh.faces[i + 2] << std::endl;
    }

    /* Close file stream. */
    out.close();
}
