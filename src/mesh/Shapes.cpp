/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "Shapes.h"
#include <array>

void Shapes::GenCube(std::vector<Vertex>& outVertices, std::vector<std::uint32_t>& outIndices, float size)
{
    std::array<Vertex, 24> vertices = 
            {
                    // Back Face
                    Vertex{{ (size/2.0f),  (size/2.0f), -(size/2.0f)}, { 0.0f,  0.0f, -1.0f}, {0, 1}}, /* Top Left     */
                    Vertex{{-(size/2.0f),  (size/2.0f), -(size/2.0f)}, { 0.0f,  0.0f, -1.0f}, {1, 1}}, /* Top Right    */
                    Vertex{{ (size/2.0f), -(size/2.0f), -(size/2.0f)}, { 0.0f,  0.0f, -1.0f}, {0, 0}}, /* Bottom Left  */
                    Vertex{{-(size/2.0f), -(size/2.0f), -(size/2.0f)}, { 0.0f,  0.0f, -1.0f}, {1, 0}}, /* Bottom Right */

                    // Front Face
                    Vertex{{-(size/2.0f),  (size/2.0f),  (size/2.0f)}, { 0.0f,  0.0f,  1.0f}, {0, 1}}, /* Top Left     */
                    Vertex{{ (size/2.0f),  (size/2.0f),  (size/2.0f)}, { 0.0f,  0.0f,  1.0f}, {1, 1}}, /* Top Right    */
                    Vertex{{-(size/2.0f), -(size/2.0f),  (size/2.0f)}, { 0.0f,  0.0f,  1.0f}, {0, 0}}, /* Bottom Left  */
                    Vertex{{ (size/2.0f), -(size/2.0f),  (size/2.0f)}, { 0.0f,  0.0f,  1.0f}, {1, 0}}, /* Bottom Right */

                    // Left Face
                    Vertex{{-(size/2.0f),  (size/2.0f), -(size/2.0f)}, {-1.0f,  0.0f,  0.0f}, {0, 1}}, /* Top Left     */
                    Vertex{{-(size/2.0f),  (size/2.0f),  (size/2.0f)}, {-1.0f,  0.0f,  0.0f}, {1, 1}}, /* Top Right    */
                    Vertex{{-(size/2.0f), -(size/2.0f), -(size/2.0f)}, {-1.0f,  0.0f,  0.0f}, {0, 0}}, /* Bottom Left  */
                    Vertex{{-(size/2.0f), -(size/2.0f),  (size/2.0f)}, {-1.0f,  0.0f,  0.0f}, {1, 0}}, /* Bottom Right */

                    // Right Face
                    Vertex{{ (size/2.0f),  (size/2.0f),  (size/2.0f)}, { 1.0f,  0.0f,  0.0f}, {0, 1}}, /* Top Left     */
                    Vertex{{ (size/2.0f),  (size/2.0f), -(size/2.0f)}, { 1.0f,  0.0f,  0.0f}, {1, 1}}, /* Top Right    */
                    Vertex{{ (size/2.0f), -(size/2.0f),  (size/2.0f)}, { 1.0f,  0.0f,  0.0f}, {0, 0}}, /* Bottom Left  */
                    Vertex{{ (size/2.0f), -(size/2.0f), -(size/2.0f)}, { 1.0f,  0.0f,  0.0f}, {1, 0}}, /* Bottom Right */

                    // Top Face
                    Vertex{{-(size/2.0f),  (size/2.0f), -(size/2.0f)}, { 0.0f,  1.0f,  0.0f}, {0, 1}}, /* Top Left     */
                    Vertex{{ (size/2.0f),  (size/2.0f), -(size/2.0f)}, { 0.0f,  1.0f,  0.0f}, {1, 1}}, /* Top Right    */
                    Vertex{{-(size/2.0f),  (size/2.0f),  (size/2.0f)}, { 0.0f,  1.0f,  0.0f}, {0, 0}}, /* Bottom Left  */
                    Vertex{{ (size/2.0f),  (size/2.0f),  (size/2.0f)}, { 0.0f,  1.0f,  0.0f}, {1, 0}}, /* Bottom Right */

                    // Bottom Face
                    Vertex{{-(size/2.0f), -(size/2.0f),  (size/2.0f)}, { 0.0f, -1.0f,  0.0f}, {0, 1}}, /* Top Left     */
                    Vertex{{ (size/2.0f), -(size/2.0f),  (size/2.0f)}, { 0.0f, -1.0f,  0.0f}, {1, 1}}, /* Top Right    */
                    Vertex{{-(size/2.0f), -(size/2.0f), -(size/2.0f)}, { 0.0f, -1.0f,  0.0f}, {0, 0}}, /* Bottom Left  */
                    Vertex{{ (size/2.0f), -(size/2.0f), -(size/2.0f)}, { 0.0f, -1.0f,  0.0f}, {1, 0}}, /* Bottom Right */
            };
    outVertices.reserve(vertices.size());
    outVertices.insert(outVertices.end(), std::begin(vertices), std::end(vertices));

    outIndices.reserve(vertices.size()*3);
    for(int i = 0; i < 6; i++)
    {
        // Stride for each pair of triangles (3 verts per triangle * 2 triangles = 6)
        // Stride for vertex-index group (1 index per vertex * 4 vertices for face = 4)
        int groupStride = 4*i;
        outIndices.push_back(1+groupStride);
        outIndices.push_back(0+groupStride);
        outIndices.push_back(2+groupStride);
        outIndices.push_back(2+groupStride);
        outIndices.push_back(3+groupStride);
        outIndices.push_back(1+groupStride);
    }
}

// http://www.songho.ca/opengl/gl_sphere.html was referenced for generating the sphere vertices/indices
void Shapes::GenSphere(std::vector<Vertex>& outVertices, std::vector<std::uint32_t>& outIndices, float radius, int stacks, int slices)
{
    // Expand vectors to needed size in advance so time isn't wasted on resizing during calculations
    outVertices.reserve(stacks * (slices + 1));
    outIndices.reserve(outVertices.capacity()*3);

    /// Calculate vertices and normals of corners for each face of normal. aka slice-stack intersects
    // Size of a single slice and single stack in sphere
    float sliceSize = glm::radians(360.0f) / static_cast<float>(slices);
    float stackSize = glm::radians(180.0f) / static_cast<float>(stacks);

    for(int i = 0; i <= stacks; i++)
    {
        // Angle of stack along sphere radius (vertically)
        float stackAngle = glm::radians(90.0f) - (static_cast<float>(i) * stackSize);
        // Base vertex of current stack
        float xy = radius * glm::cos(stackAngle);
        float z = radius * glm::sin(stackAngle);

        for(int j = 0; j <= slices; j++)
        {
            // Angle of slice around entire sphere (horizontally)
            float sliceAngle = static_cast<float>(j) * sliceSize;

            // Vertex of where the current stack and slice intersect
            glm::vec3 vertex(xy * glm::cos(sliceAngle), xy * glm::sin(sliceAngle), z);

            outVertices.push_back(
                    {
                            vertex,
                            glm::normalize(vertex),
                            {
                                    static_cast<float>(j) / static_cast<float>(slices),
                                    static_cast<float>(i) / static_cast<float>(stacks)
                            }
                    });

        }
    }

    /// Get the indices needed to the triangle(s) for each stack face
    for(int i = 0; i < stacks; i++)
    {
        // Top & Bottom left of the current stack face
        int topLeft = i * (slices + 1);
        int bottomLeft = topLeft + slices + 1;

        for(int j = 0; j < slices; j++, topLeft++, bottomLeft++)
        {
            // Top & Bottom stacks only need one triangle per face since
            // "top left" and "top right" are both polar north/south

            // If this isn't the first stack
            if(i != 0)
            {
                // TL       TR
                // *--------*
                // |\       |
                // | \      |
                // |  \     |
                // |   \    |
                // |    \   |
                // |     \  |
                // |      \ |
                // *--------*
                // BL       BR
                // Top Left . Bottom Left . Top Right
                outIndices.push_back(topLeft);
                outIndices.push_back(bottomLeft);
                outIndices.push_back(topLeft+1);
            }

            // If this isn't the last stack
            if(i != stacks-1)
            {
                // TL       TR
                // *--------*
                // |       /|
                // |      / |
                // |     /  |
                // |    /   |
                // |   /    |
                // |  /     |
                // | /      |
                // *--------*
                // BL       BR
                // Top Right . Bottom Left . Bottom Right
                outIndices.push_back(topLeft+1);
                outIndices.push_back(bottomLeft);
                outIndices.push_back(bottomLeft+1);
            }
        }
    }
}

void Shapes::GenCylinder(std::vector<Vertex>& outVertices, std::vector<std::uint32_t>& outIndices,
                        float height, float radius, int stacks, int slices, float sweep, bool capped)
{
    // Reserve space ahead of time for performance
    outVertices.reserve(stacks*(slices+1));
    outIndices.reserve(outVertices.capacity()*3);

    float stackSize = height / (float)(stacks-1);
    float sliceSize = glm::radians(sweep) / (float)slices;

    for(int stack = 0; stack < stacks; stack++)
    {
        float y = -height / 2.0f + (float)stack * stackSize;

        for(int slice = 0; slice <= slices; slice++)
        {
            float sliceAngle = (float)slice * sliceSize;
            glm::vec3 vertex(radius * glm::cos(sliceAngle), y, radius * glm::sin(sliceAngle));

            outVertices.push_back(
                    {
                            vertex,
                            glm::normalize(vertex),
                            {
                                    static_cast<float>(slice) / static_cast<float>(slices),
                                    static_cast<float>(stack) / static_cast<float>(stacks)
                            }
                    }
            );
        }
    }

    for(int i = 0; i < stacks - 1; i++)
    {
        int topLeft = i * (slices + 1);
        int bottomLeft = topLeft + slices + 1;

        for (int j = 0; j < slices; j++, topLeft++, bottomLeft++)
        {

            // TL       TR
            // *--------*
            // |\       |
            // | \      |
            // |  \     |
            // |   \    |
            // |    \   |
            // |     \  |
            // |      \ |
            // *--------*
            // BL       BR
            // Top Left . Bottom Left . Top Right
            outIndices.push_back(topLeft);
            outIndices.push_back(bottomLeft);
            outIndices.push_back(topLeft + 1);

            // TL       TR
            // *--------*
            // |       /|
            // |      / |
            // |     /  |
            // |    /   |
            // |   /    |
            // |  /     |
            // | /      |
            // *--------*
            // BL       BR
            // Top Right . Bottom Left . Bottom Right
            outIndices.push_back(topLeft + 1);
            outIndices.push_back(bottomLeft);
            outIndices.push_back(bottomLeft + 1);
        }
    }
    
    /// Generate cylinder caps
    if(capped)
    {
        // Last index
        int index = outVertices.size();

        // Center of the cap
        float capCenter = height/2.0f;

        // Center point for the cap
        glm::vec3 center(0.0f, -capCenter, 0.0f);

        outVertices.push_back(
                {
                        center,
                        glm::normalize(center),
                        {0.5f, 0.5f}
                }
        );

        // Create vertices around the cap top
        for(int slice = 0; slice <= slices; slice++)
        {
            float sliceAngle = (float)slice * sliceSize;
            glm::vec3 vertex(radius * glm::cos(sliceAngle), -capCenter, radius * glm::sin(sliceAngle));

            outVertices.push_back(
                    {
                            vertex,
                            glm::normalize(vertex),
                            {glm::cos(sliceAngle), glm::sin(sliceAngle)}
                    }
            );
        }

        // Index the cap top
        for (int j = 0; j < slices; j++)
        {
            outIndices.push_back(index);
            outIndices.push_back(index + j + 1);
            outIndices.push_back(index + j + 2);
        }

        // Same as above but reverse ordered indices
        index = outVertices.size();

        center = glm::vec3(0.0f, capCenter, 0.0f);
        outVertices.push_back(
                {
                        center,
                        glm::normalize(center),
                        {0.5f, 0.5f}
                }
        );

        for(int slice = 0; slice <= slices; slice++)
        {
            float sliceAngle = (float)slice * sliceSize;
            glm::vec3 vertex(radius * glm::cos(sliceAngle), capCenter, radius * glm::sin(sliceAngle));

            outVertices.push_back(
                    {
                            vertex,
                            glm::normalize(vertex),
                            {glm::cos(sliceAngle), glm::sin(sliceAngle)}
                    }
            );
        }

        for (int j = 0; j < slices; j++)
        {
            outIndices.push_back(index + j + 2);
            outIndices.push_back(index + j + 1);
            outIndices.push_back(index);
        }
    }
}