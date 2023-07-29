#include "Renderer.h"
#include "Tetris.h"
#include "Tetromino.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
Renderer::Renderer() {
  this->BLOCKSIZE = this->PLAYFIELD_HEIGHT / Tetris::TETRIS_PLAYFIELD_HEIGHT;
  int result = SDL_Init(SDL_INIT_VIDEO);
  if (result < 0) {
    std::cerr << SDL_GetError() << std::endl;
    throw;
  }
  this->WINDOW = SDL_CreateWindow("Tetris", 0, 0, Renderer::WINDOW_WIDTH,
                                  Renderer::WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
  SDL_GLContext ctx = SDL_GL_CreateContext(Renderer::WINDOW);
  if (ctx == NULL) {
    std::cerr << SDL_GetError() << std::endl;
    throw;
  }
  int res = SDL_GL_MakeCurrent(Renderer::WINDOW, ctx);
  if (res < 0) {
    std::cerr << SDL_GetError() << std::endl;
    throw;
  }
  GLenum glew_init_res = glewInit();
  if (glew_init_res != GLEW_OK) {
    std::cerr << "Unable to initialize GLEW" << std::endl;
    std::cerr << glewGetErrorString(glew_init_res) << std::endl;
    throw;
  }
  this->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  std::ifstream vertex;
  vertex.open("shaders/vertex_shader.glsl");
  if (vertex.is_open()) {
    std::string v_src;
    while (!vertex.eof()) {
      v_src += vertex.get();
    }
    vertex.close();
    int length = v_src.length();
    const GLchar *src = v_src.c_str();
    glShaderSource(Renderer::vertex_shader, 1, &src, &length);
    glCompileShader(Renderer::vertex_shader);
    int status;
    glGetShaderiv(Renderer::vertex_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
      std::cerr << "COMPILATION FAILED" << std::endl;
      int info_log_length;
      glGetShaderiv(Renderer::vertex_shader, GL_INFO_LOG_LENGTH,
                    &info_log_length);
      std::string info_log(info_log_length, ' ');
      glGetShaderInfoLog(Renderer::vertex_shader, info_log.length(),
                         &info_log_length, info_log.data());
      std::cerr << info_log << std::endl;
      throw;
    }
  } else {
    std::cerr << "Shader failed to open" << std::endl;
    throw;
  }
  this->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  std::ifstream fragment;
  fragment.open("shaders/fragment_shader.glsl");
  if (fragment.is_open()) {
    std::string f_src;
    while (!fragment.eof()) {
      f_src += fragment.get();
    }
    fragment.close();
    int length = f_src.length();
    const GLchar *src = f_src.c_str();
    glShaderSource(Renderer::fragment_shader, 1, &src, &length);
    glCompileShader(Renderer::fragment_shader);
    int status;
    glGetShaderiv(Renderer::fragment_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
      std::cerr << "COMPILATION FAILED" << std::endl;
      int info_log_length;
      glGetShaderiv(Renderer::fragment_shader, GL_INFO_LOG_LENGTH,
                    &info_log_length);
      std::string info_log(info_log_length, ' ');
      glGetShaderInfoLog(Renderer::fragment_shader, info_log.length(),
                         &info_log_length, info_log.data());
      std::cerr << info_log << std::endl;
      throw;
    }
  } else {
    std::cerr << "Fragment Shader failed to open" << std::endl;
    throw;
  }
}
std::vector<float> Renderer::convert_coords_to_vertices(const Tetromino &t) {
  std::vector<float> vertices{
      t.x1 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_WIDTH - 1,
      -(t.y1 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT - 1),
      (t.x1 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
              (float)this->PLAYFIELD_WIDTH -
          1,
      -(t.y1 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT - 1),
      t.x1 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_WIDTH - 1,
      -((t.y1 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
            (float)this->PLAYFIELD_HEIGHT -
        1),
      (t.x1 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
              (float)this->PLAYFIELD_WIDTH -
          1,
      -((t.y1 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
            (float)this->PLAYFIELD_HEIGHT -
        1),

      t.x2 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_WIDTH - 1,
      -(t.y2 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT - 1),
      (t.x2 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
              (float)this->PLAYFIELD_WIDTH -
          1,
      -(t.y2 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT - 1),
      t.x2 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_WIDTH - 1,
      -((t.y2 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
            (float)this->PLAYFIELD_HEIGHT -
        1),
      (t.x2 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
              (float)this->PLAYFIELD_WIDTH -
          1,
      -((t.y2 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
            (float)this->PLAYFIELD_HEIGHT -
        1),

      t.x3 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_WIDTH - 1,
      -(t.y3 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT - 1),
      (t.x3 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
              (float)this->PLAYFIELD_WIDTH -
          1,
      -(t.y3 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT - 1),
      t.x3 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_WIDTH - 1,
      -((t.y3 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
            (float)this->PLAYFIELD_HEIGHT -
        1),
      (t.x3 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
              (float)this->PLAYFIELD_WIDTH -
          1,
      -((t.y3 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
            (float)this->PLAYFIELD_HEIGHT -
        1),

      t.x4 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_WIDTH - 1,
      -(t.y4 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT - 1),
      (t.x4 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
              (float)this->PLAYFIELD_WIDTH -
          1,
      -(t.y4 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT - 1),
      t.x4 * this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_WIDTH - 1,
      -((t.y4 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
            (float)this->PLAYFIELD_HEIGHT -
        1),
      (t.x4 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
              (float)this->PLAYFIELD_WIDTH -
          1,
      -((t.y4 * this->BLOCKSIZE + this->BLOCKSIZE) * 2 /
            (float)this->PLAYFIELD_HEIGHT -
        1),
  };
  return vertices;
}
void Renderer::update_playfield(int32_t row) {
  for (size_t vbo_index = 0; vbo_index < this->vertex_buffer_objects.size() - 1;
       ++vbo_index) {
    if (!glIsVertexArray(this->vertex_array_objects.at(vbo_index))) {
      continue;
    }
    glBindVertexArray(this->vertex_array_objects.at(vbo_index));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_objects.at(vbo_index));
    GLint buffer_length;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_length);
    buffer_length /= sizeof(float);
    std::vector<float> data(buffer_length);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * data.size(),
                       data.data());
    for (size_t data_index = 0; data_index < data.size(); data_index += 8) {
      int32_t first = ((-data[data_index + 1] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE;
      int32_t second = ((-data[data_index + 3] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE;
      int32_t third = ((-data[data_index + 5] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE;
      int32_t fourth = ((-data[data_index + 7] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE;
      //int32_t first = std::round(((-data[data_index + 1] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE);
      //int32_t second = std::round(((-data[data_index + 3] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE);
      //int32_t third = std::round(((-data[data_index + 5] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE);
      //int32_t fourth = std::round(((-data[data_index + 7] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE);
      if (first < row + 1 &&
          second < row + 1 &&
          third < row + 1 &&
          fourth < row + 1) {
        data[data_index + 1] -=
            this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT;
        data[data_index + 3] -=
            this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT;
        data[data_index + 5] -=
            this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT;
        data[data_index + 7] -=
            this->BLOCKSIZE * 2 / (float)this->PLAYFIELD_HEIGHT;
      }
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * buffer_length,
                    data.data());
  }
}
void Renderer::delete_row(int32_t row) {
  for (size_t i = 0; i < this->vertex_buffer_objects.size() - 1; ++i) {
    if (!glIsVertexArray(this->vertex_array_objects.at(i))) {
      continue;
    }
    glBindVertexArray(this->vertex_array_objects.at(i));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_objects.at(i));
    GLint buffer_length;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_length);
    buffer_length /= sizeof(float);
    std::vector<float> data(buffer_length);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * data.size(),
                       data.data());
    std::vector<float> new_data;
    for (size_t data_index = 0; data_index < data.size(); data_index += 8) {
      int32_t first = ((-data[data_index + 1] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE;
      int32_t second = ((-data[data_index + 3] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE;
      int32_t third = ((-data[data_index + 5] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE;
      int32_t fourth = ((-data[data_index + 7] + 1.0f) * this->PLAYFIELD_HEIGHT / 2.0f) / this->BLOCKSIZE;
      if (first == row &&
          second == row &&
           third == row + 1 &&
           fourth == row + 1) {
        continue;
      }
      new_data.insert(new_data.end(), data.begin() + data_index,
          data.begin() + data_index + 8);
    }
    if (new_data.size() < data.size()) {
      if (!new_data.empty()) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_data.size(),
            new_data.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
            this->element_buffer_objects.at(i).first);
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE,
            &buffer_length);
        buffer_length /= sizeof(int32_t);
        std::vector<int32_t> indices(buffer_length);
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
            sizeof(int32_t) * indices.size(), indices.data());
        indices.erase(indices.end() - (data.size() - new_data.size()) / 8 * 6,
            indices.end());
        this->element_buffer_objects.at(i).second = indices.size();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int32_t) * indices.size(),
            indices.data(), GL_DYNAMIC_DRAW);
      } else {
        glDeleteVertexArrays(1, &this->vertex_array_objects.at(i));
        glDeleteBuffers(1, &this->vertex_buffer_objects.at(i));
        glDeleteBuffers(1, &this->element_buffer_objects.at(i).first);
      }

    }
  }
}
void Renderer::render_tetromino(const Tetromino &t) {
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  std::vector<float> vertices = this->convert_coords_to_vertices(t);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
               vertices.data(), GL_DYNAMIC_DRAW);

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  int32_t indices[] = {
      0,  1,  2,  1,  3,  2,

      4,  5,  6,  5,  7,  6,

      8,  9,  10, 9,  11, 10,

      12, 13, 14, 13, 15, 14,
  };
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_DYNAMIC_DRAW);

  this->vertex_buffer_objects.push_back(vbo);
  this->element_buffer_objects.push_back(
      std::pair<GLuint, size_t>{ebo, 24});

  this->create_program();

  GLint location = glGetAttribLocation(this->current_program, "pos");
  if (location == -1) {
    std::cerr << "pos's location could not be found" << std::endl;
    throw;
  }
  glVertexAttribPointer(location, 2, GL_FLOAT, false, 2 * sizeof(float),
                        (void *)0);
  glEnableVertexAttribArray(location);

  GLint out_color_location =
      glGetUniformLocation(this->current_program, "out_color");
  if (out_color_location == -1) {
    std::cerr << "out_color's location could not be found" << std::endl;
    throw;
  }
  glUseProgram(this->current_program);
  glUniform4f(out_color_location, t.red / 255.0f, t.green / 255.0f,
              t.blue / 255.0f, 1.0f);
  if (glIsVertexArray(this->current_vao)) {
    this->vertex_array_objects.push_back(this->current_vao);
  }
  this->current_vao = vao;
}
void Renderer::create_program() {
  if (glIsProgram(this->current_program))
    this->programs.push_back(this->current_program);
  this->current_program = glCreateProgram();
  glAttachShader(this->current_program, this->vertex_shader);
  glAttachShader(this->current_program, this->fragment_shader);
  glLinkProgram(this->current_program);
}
void Renderer::update_tetromino(const Tetromino &t) {
  glBindVertexArray(this->current_vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_objects.back());
  std::vector<float> vertices = this->convert_coords_to_vertices(t);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(),
                  vertices.data());
}
void Renderer::render() {
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(this->current_program);
  glBindVertexArray(this->current_vao);
  glDrawElements(GL_TRIANGLES, this->element_buffer_objects.back().second,
                 GL_UNSIGNED_INT, 0);
  for (size_t idx = 0; idx < this->vertex_array_objects.size(); ++idx) {
    if (!glIsVertexArray(this->vertex_array_objects.at(idx))) {
      continue;
    }
    glUseProgram(this->programs.at(idx));
    glBindVertexArray(this->vertex_array_objects.at(idx));
    glDrawElements(GL_TRIANGLES, this->element_buffer_objects.back().second,
                   GL_UNSIGNED_INT, 0);
  }
  SDL_GL_SwapWindow(this->WINDOW);
}
Renderer::~Renderer() {
  SDL_DestroyWindow(Renderer::WINDOW);
  SDL_Quit();
}
