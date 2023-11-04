#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.hpp>

#include "defines.hpp"

#define FILE_BUFFER 10000

// content of dest is null terminated
static int read_file(const char* file, char* dest, i32 bytes)
{
    int status = 0;
    FILE* fptr = fopen(file, "r");
    if (fptr == NULL) {
        printf("Failed to read file: %s\n", file);
        return 1;
    }
    fseek(fptr, 0, SEEK_END);
    i32 len = ftell(fptr);
    if (len > bytes - 1) {
        status = 2;
        printf("File content exceeds buffer size: %s, buffer: %u", file, bytes);
        goto close_file;
    }
    fseek(fptr, 0, SEEK_SET);
    fread(dest, 1, len, fptr);
    dest[len] = '\0';
close_file:
    fclose(fptr);
    return status;
}

static int create_shader(const char* v_file, 
                         const char* f_file, 
                         unsigned int* shader)
{
    char buffer[FILE_BUFFER];
    const GLchar* buffer_ptr = buffer;

    unsigned int v_prog, f_prog;
    char info_log[512];
    int status;

    if (read_file(v_file, buffer, FILE_BUFFER))
        return 2;
    v_prog = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_prog, 1, &buffer_ptr, NULL);
    glCompileShader(v_prog);
    glGetShaderiv(v_prog, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(v_prog, 512, NULL, info_log);
        printf("Error compiling vertex shader: %s\n", info_log);
        return 1;
    }

    if (read_file(f_file, buffer, FILE_BUFFER))
        return 2;
    f_prog = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_prog, 1, &buffer_ptr, NULL);
    glCompileShader(f_prog);
    glGetShaderiv(f_prog, GL_COMPILE_STATUS, &status);
    if(!status) {
        glGetShaderInfoLog(f_prog, 512, NULL, info_log);
        printf("Error compiling fragment shader: %s\n", info_log);
        return 1;
    }

    *shader = glCreateProgram();
    glAttachShader(*shader, v_prog);
    glAttachShader(*shader, f_prog);
    glLinkProgram(*shader);
    glGetProgramiv(*shader, GL_LINK_STATUS, &status);
    if(!status) {
        glGetProgramInfoLog(*shader, 512, NULL, info_log);
        printf("Error linking shader: %s\n", info_log);
        return 3;
    }

    glDeleteShader(v_prog);
    glDeleteShader(f_prog);
    return 0;
}

int load_default(const char* vert, const char* frag, DefaultShader* shader)
{
    if (create_shader(vert, frag, &shader->id))
        return 1;
    shader->projection = glGetUniformLocation(shader->id, "projection");
    return 0;
}

void set_mat4(unsigned int uniform, glm::mat4* mat) 
{
    glUniformMatrix4fv(uniform, 1, GL_FALSE, &(*mat)[0][0]);
}

void set_vec2(unsigned int uniform, glm::vec2* vec)
{
    glUniform2fv(uniform, 1, &(*vec)[0]);
}
