#include "batchRenderer.h"

#include "renderer.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "texture.h"

#include <iostream>


#include "glm/gtx/string_cast.hpp"

#define GLM_ENABLE_EXPERIMENTAL



static unsigned int* populateIndices()
{
    unsigned int *p = new unsigned int[MaxIndices];
    for (int i = 0; i < MaxQuads; i++)
    {
        // each quad has indices
        // 0, 1, 2 -> 4i, 4i+1, 4i+2
        // 2, 3, 0 -> 4i+2, 4i+3, 4i
        p[6*i] = 4*i;
        p[6*i+1] = 4*i+1;
        p[6*i+2] = 4*i+2;
        p[6*i+3] = 4*i+2;
        p[6*i+4] = 4*i+3;
        p[6*i+5] = 4*i;
    }
    return p;
}

static unsigned int *indices = populateIndices();

struct RendererData
{
    VertexArray vao = VertexArray();
    VertexBuffer vb = VertexBuffer();
    VertexBufferLayout layout = VertexBufferLayout();
    IndexBuffer ibo = IndexBuffer();
    Vertex* quadBuffer = nullptr;
    Vertex* quadBufferNext = nullptr;
    // how many indices to draw
    unsigned int indexCount = 0;
    unsigned int quadCount = 0;
    unsigned int vertexCount = 0;
};

static RendererData s_Data;

void BatchRenderer::Init()
{
    // enable alpha
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));

    // allocate memory for all possible Vertices
    s_Data.quadBuffer = new Vertex[MaxVertices];
    s_Data.quadBufferNext = s_Data.quadBuffer;

    s_Data.vao.Init();
    s_Data.vb.Init(MaxVertices * sizeof(Vertex));
    s_Data.layout.Push<glm::vec2>(1); // vertex coord
    s_Data.layout.Push<glm::vec4>(1); // color
    s_Data.layout.Push<glm::vec2>(1); // tex coord
    s_Data.layout.Push<float>(1); // tex slot
    s_Data.vao.AddBuffer(s_Data.vb, s_Data.layout);

    s_Data.ibo.Init(indices, MaxIndices);
}


void BatchRenderer::Shutdown()
{
    s_Data.vao.Unbind();
    s_Data.ibo.Unbind();
    s_Data.vb.Unbind();
    s_Data.layout.Reset();

    // free memory
    delete[] s_Data.quadBuffer;
}

void BatchRenderer::BeginBatch()
{
    // reset the next free position to the start
    s_Data.quadBufferNext = s_Data.quadBuffer;
}

void BatchRenderer::EndBatch()
{
    // calculate size of quad buffer and send data
    GLsizeiptr size = (char*)s_Data.quadBufferNext - (char*)s_Data.quadBuffer;
    std::cout << "size: " << size << std::endl;
    // s_Data.vb.Bind();
    std::cout << "layout m_Stride: " << s_Data.layout.GetStride() << std::endl;
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, s_Data.vertexCount * sizeof(Vertex), s_Data.quadBuffer));
}

void BatchRenderer::Flush()
{
    // clear stats, make a draw
    // s_Data.vao.Unbind();
    s_Data.vao.Bind();
    s_Data.ibo.Bind();
    std::cout << "index count: " << s_Data.indexCount << std::endl;
    std::cout << "buffer: " << std::endl;

    Vertex* temp = s_Data.quadBuffer;
    for (int i = 0; i < 6; i++)
    {
        std::cout << glm::to_string(temp->position) << std::endl;
        temp++;
    }
    GLCall(glDrawElements(GL_TRIANGLES, s_Data.indexCount, GL_UNSIGNED_INT, nullptr));
    // s_Data.vao.Unbind();

    s_Data.indexCount = 0;
    s_Data.quadCount = 0;
    s_Data.vertexCount = 0;
}

void BatchRenderer::MakeQuad(float px, float py, float dx, float dy, glm::vec4 color)
{
    // if buffer is full...
    // end the batch, make a manual flush, then start new batch
    if (s_Data.indexCount >= MaxIndices)
    {
        EndBatch();
        Flush();
        BeginBatch();
    }

    // start creating quad vertices
    // s_Data.quadBufferNext->position = glm::vec2(px, py);
    s_Data.quadBufferNext->position = { px, py };
    s_Data.quadBufferNext->color = color;
    s_Data.quadBufferNext->texCoord = glm::vec2(0.f, 0.f);
    s_Data.quadBufferNext->texSlot = 0.f;
    s_Data.quadBufferNext++;

    s_Data.quadBufferNext->position = { px + dx, py };
    s_Data.quadBufferNext->color = color;
    s_Data.quadBufferNext->texCoord = glm::vec2(1.f, 0.f);
    s_Data.quadBufferNext->texSlot = 0.f;
    s_Data.quadBufferNext++;

    s_Data.quadBufferNext->position = { px + dx, py + dy };
    s_Data.quadBufferNext->color = color;
    s_Data.quadBufferNext->texCoord = glm::vec2(1.f, 1.f);
    s_Data.quadBufferNext->texSlot = 0.f;
    s_Data.quadBufferNext++;

    s_Data.quadBufferNext->position = { px, py + dy };
    s_Data.quadBufferNext->color = color;
    s_Data.quadBufferNext->texCoord = glm::vec2(0.f, 1.f);
    s_Data.quadBufferNext->texSlot = 0.f;
    s_Data.quadBufferNext++;

    s_Data.indexCount += 6;
    s_Data.quadCount++;
    s_Data.vertexCount += 4;
}