#pragma once

#include "glm/glm.hpp"

#include "vertexArray.h"
#include "indexBuffer.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "shader.h"

#include <memory>

// max number of quads to render at a time
static const unsigned int MaxQuads = 1000;
static const unsigned int MaxVertices = MaxQuads * 4;
static const unsigned int MaxIndices = MaxQuads * 6;

struct Vertex
{
    glm::vec2 position;
    glm::vec2 texCoord;
    float texSlot;
};

class BatchRenderer
{
public:
    // BatchRenderer();
    // ~BatchRenderer();
    /* setup va, vb, ib, allocate memory for buffer */
    static void Init();
    /* clear memory, unbind va, vb, ib */
    static void Shutdown();

    /* reset the buffer
       i.e. set the next free buffer location to the beginning */
    static void BeginBatch();
    /* flush the data on buffer to GPU by glBufferSubdata */
    static void EndBatch();
    /* bind textures, bind va, reset index count */
    static void Flush();

    /* make a quad, store the data into buffer */
    static void MakeQuad(float px, float py, float dx, float dy, glm::vec4 color);
};