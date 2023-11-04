#ifndef GEOMETRY_BUFFER_HPP
#define GEOMETRY_BUFFER_HPP

#include "defines.hpp"

#define INDEX_BUFFER_CAPACITY 2000
#define VERTEX_BUFFER_CAPACITY 2000

struct Vertex {
    float x;
    float y;
};

struct Batch {
    u32 index;
    u32 count;
};

struct GeometryBuffer {
    Vertex* vertex_buffer;
    unsigned int* index_buffer;

    u32 vertex_count;
    u32 index_count;
    u32 sprite_vertex;
    u32 sprite_index;

    void init();
    void start_batch();
    Batch end_batch();
    void reset();

    void draw_rect(float x, float y, float width, float height);
    void update_gpu_buffers(unsigned int buffer);
};

unsigned int setup_gpu_buffers();

#endif
