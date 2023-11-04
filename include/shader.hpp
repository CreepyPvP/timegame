#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/mat4x4.hpp>
#include <string>

struct DefaultShader {
    unsigned int id;
    unsigned int projection;
};


int load_default(const char* vert, const char* frag, DefaultShader* shader);

void set_mat4(unsigned int uniformId, glm::mat4* matrix);

void set_vec2(unsigned int uniformId, glm::vec2* vec);

#endif
