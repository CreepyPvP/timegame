#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "geometry_buffer.hpp"

#include <string.h>
#include <math.h>


unsigned int setup_gpu_buffers() {
    unsigned int arr;
    glGenVertexArrays(1, &arr);
    glBindVertexArray(arr);

    unsigned int buffers[2];
    GL(glGenBuffers(2, buffers));
    unsigned int vertex_buffer = buffers[0];
    unsigned int index_buffer = buffers[1];

    GL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
    GL(glBufferData(GL_ARRAY_BUFFER, 
                    sizeof(Vertex) * VERTEX_BUFFER_CAPACITY, 
                    NULL, 
                    GL_DYNAMIC_DRAW));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
    GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                    sizeof(unsigned int) * INDEX_BUFFER_CAPACITY, 
                    NULL, 
                    GL_DYNAMIC_DRAW));

    GL(glEnableVertexAttribArray(0));
    GL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
    return arr;
}

void GeometryBuffer::update_gpu_buffers(unsigned int buffer)
{
    if (vertex_count == 0 || index_count == 0)
        return;

    glBindVertexArray(buffer);
    Vertex* v_buff = (Vertex*) glMapBufferRange(GL_ARRAY_BUFFER, 
                                                0, 
                                                sizeof(Vertex) * vertex_count, 
                                                GL_MAP_WRITE_BIT);
    memcpy(v_buff, vertex_buffer, sizeof(Vertex) * vertex_count);
    GL(glUnmapBuffer(GL_ARRAY_BUFFER));
    unsigned int* i_buff = (unsigned int*) glMapBufferRange(
                                    GL_ELEMENT_ARRAY_BUFFER, 
                                    0, 
                                    sizeof(unsigned int) * index_count, 
                                    GL_MAP_WRITE_BIT);
    memcpy(i_buff, index_buffer, sizeof(unsigned int) * index_count);
    GL(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
}

void GeometryBuffer::init()
{
    vertex_buffer = (Vertex*) malloc(sizeof(Vertex) * VERTEX_BUFFER_CAPACITY);
    index_buffer = (unsigned int*) malloc(
        sizeof(unsigned int) * INDEX_BUFFER_CAPACITY);
    vertex_count = 0;
    index_count = 0;
}

void GeometryBuffer::start_batch()
{
    sprite_vertex = 0;
    sprite_index = 0;
}

Batch GeometryBuffer::end_batch()
{
    Batch batch;
    batch.index = index_count;
    batch.count = sprite_index;
    vertex_count += sprite_vertex;
    index_count += sprite_index;
    return batch;
}

void GeometryBuffer::reset()
{
    vertex_count = 0;
    index_count = 0;
}

void GeometryBuffer::draw_rect(float x, float y, float width, float height)
{
    int vertex = vertex_count + sprite_vertex;
    int index = index_count + sprite_index;

    float start_x = floor(x + 0.5);
    float start_y = floor(y + 0.5);
    float end_x = floor(x + width + 0.5);
    float end_y = floor(y + height + 0.5);

    vertex_buffer[vertex + 0].x = start_x;
    vertex_buffer[vertex + 0].y = end_y;
    vertex_buffer[vertex + 1].x = end_x;
    vertex_buffer[vertex + 1].y = end_y;
    vertex_buffer[vertex + 2].x = start_x;
    vertex_buffer[vertex + 2].y = start_y;
    vertex_buffer[vertex + 3].x = end_x;
    vertex_buffer[vertex + 3].y = start_y;

    index_buffer[index + 0] = 1 + vertex;
    index_buffer[index + 1] = 3 + vertex;
    index_buffer[index + 2] = 2 + vertex;
    index_buffer[index + 3] = 0 + vertex;
    index_buffer[index + 4] = 1 + vertex;
    index_buffer[index + 5] = 2 + vertex;

    sprite_vertex += 4;
    sprite_index += 6;
}
