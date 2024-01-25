#include "batchRenderer.h"

#include "renderer.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "texture.h"

#include <iostream>
#include <array>

static uint32_t* populateIndices()
{
    uint32_t *p = new uint32_t[MaxIndices];
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

static uint32_t *indices = populateIndices();

struct RendererData
{
    VertexArray vao = VertexArray();
    VertexBuffer vb = VertexBuffer();
    VertexBufferLayout layout = VertexBufferLayout();
    IndexBuffer ibo = IndexBuffer();
    Vertex* quadBuffer = nullptr;
    Vertex* quadBufferNext = nullptr;
    // how many indices to draw
    uint32_t indexCount = 0;
    uint32_t quadCount = 0;
    uint32_t vertexCount = 0;
    // an array of texture ids, the client is responsible for
    // the creation, and deletion of these textures
    // the renderer will handle the binding
    std::array<uint32_t, MaxTextures> textures;
    uint32_t whiteTexture = 0;
    uint32_t textureNext = 1;
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

    // manually create a white texture in slot 0
    // cannot create a Texture object as it will get destoried after this scope
    // Texture whiteTexture();
    GLCall(glGenTextures(1, &s_Data.whiteTexture));
    GLCall(glBindTexture(GL_TEXTURE_2D, s_Data.whiteTexture));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    uint32_t whiteColor = 0xffffffff;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whiteColor));

    // clear other texture slots
    for (int i = 1; i < MaxTextures; i++)
        s_Data.textures[i] = 0;
}


void BatchRenderer::Shutdown()
{
    s_Data.vao.Unbind();
    s_Data.ibo.Unbind();
    s_Data.vb.Unbind();
    s_Data.layout.Reset();

    s_Data.textureNext = 1;

    glDeleteTextures(1, &s_Data.whiteTexture);
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
    // std::cout << "size: " << size << std::endl;
    // s_Data.vb.Bind();
    // std::cout << "layout m_Stride: " << s_Data.layout.GetStride() << std::endl;
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, s_Data.vertexCount * sizeof(Vertex), s_Data.quadBuffer));
}

void BatchRenderer::Flush()
{
    // clear stats, make a draw
    s_Data.vao.Bind();
    s_Data.ibo.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, s_Data.indexCount, GL_UNSIGNED_INT, nullptr));

    s_Data.vao.Unbind();
    s_Data.ibo.Unbind();

    s_Data.indexCount = 0;
    s_Data.quadCount = 0;
    s_Data.vertexCount = 0;
    s_Data.textureNext = 1;
}

void BatchRenderer::MakeQuad(glm::vec2 pos, glm::vec2 delta, glm::vec4 color)
{
    // if buffer is full... or texture slots are full
    // end the batch, make a manual flush, then start new batch
    if (s_Data.indexCount >= MaxIndices)
    {
        EndBatch();
        Flush();
        BeginBatch();
    }

    // start creating quad vertices
    // s_Data.quadBufferNext->position = glm::vec2(px, py);
    s_Data.quadBufferNext->position = pos;
    s_Data.quadBufferNext->color = color;
    s_Data.quadBufferNext->texCoord = { 0.f, 0.f };
    s_Data.quadBufferNext->texSlot = 0.f;
    s_Data.quadBufferNext++;

    s_Data.quadBufferNext->position = { pos[0] + delta[0], pos[1] };
    s_Data.quadBufferNext->color = color;
    s_Data.quadBufferNext->texCoord = { 1.f, 0.f };
    s_Data.quadBufferNext->texSlot = 0.f;
    s_Data.quadBufferNext++;

    s_Data.quadBufferNext->position = { pos[0] + delta[0], pos[1] + delta[1] };
    s_Data.quadBufferNext->color = color;
    s_Data.quadBufferNext->texCoord = { 1.f, 1.f };
    s_Data.quadBufferNext->texSlot = 0.f;
    s_Data.quadBufferNext++;

    s_Data.quadBufferNext->position = { pos[0], pos[1] + delta[1] };
    s_Data.quadBufferNext->color = color;
    s_Data.quadBufferNext->texCoord = { 0.f, 1.f };
    s_Data.quadBufferNext->texSlot = 0.f;
    s_Data.quadBufferNext++;

    s_Data.indexCount += 6;
    s_Data.quadCount++;
    s_Data.vertexCount += 4;
}

void BatchRenderer::MakeQuad(glm::vec2 pos, glm::vec2 delta, uint32_t texID)
{
    // if buffer is full... or texture slots are full
    // end the batch, make a manual flush, then start new batch
    if (s_Data.indexCount >= MaxIndices || s_Data.textureNext >= MaxTextures)
    {
        EndBatch();
        Flush();
        BeginBatch();
    }

    // find texture id in the slots
    float texSlot = 0.f;
    for (int i = 1; i < s_Data.textureNext; i++)
    {
        if (texID == s_Data.textures[i])
        {
            texSlot = i;
            break;
        }
    }

    // if didn't find the texture id, assign a new slot
    if (texSlot == 0.f)
    {
        s_Data.textures[s_Data.textureNext] = texID;
        texSlot = s_Data.textureNext;
        s_Data.textureNext++;
    }

    // bind the texture slot
    GLCall(glActiveTexture(GL_TEXTURE0 + texSlot));
    GLCall(glBindTexture(GL_TEXTURE_2D, texID));

    // start creating quad vertices
    s_Data.quadBufferNext->position = pos;
    s_Data.quadBufferNext->color = { 1.f, 1.f, 1.f, 1.f };
    s_Data.quadBufferNext->texCoord = { 0.f, 0.f };
    s_Data.quadBufferNext->texSlot = texSlot;
    s_Data.quadBufferNext++;

    s_Data.quadBufferNext->position = { pos[0] + delta[0], pos[1] };
    s_Data.quadBufferNext->color = { 1.f, 1.f, 1.f, 1.f };
    s_Data.quadBufferNext->texCoord = { 1.f, 0.f };
    s_Data.quadBufferNext->texSlot = texSlot;
    s_Data.quadBufferNext++;

    s_Data.quadBufferNext->position = { pos[0] + delta[0], pos[1] + delta[1] };
    s_Data.quadBufferNext->color = { 1.f, 1.f, 1.f, 1.f };
    s_Data.quadBufferNext->texCoord = { 1.f, 1.f };
    s_Data.quadBufferNext->texSlot = texSlot;
    s_Data.quadBufferNext++;

    s_Data.quadBufferNext->position = { pos[0], pos[1] + delta[1] };
    s_Data.quadBufferNext->color = { 1.f, 1.f, 1.f, 1.f };
    s_Data.quadBufferNext->texCoord = { 0.f, 1.f };
    s_Data.quadBufferNext->texSlot = texSlot;
    s_Data.quadBufferNext++;

    s_Data.indexCount += 6;
    s_Data.quadCount++;
    s_Data.vertexCount += 4;
}