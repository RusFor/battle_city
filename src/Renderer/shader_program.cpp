#include "shader_program.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace RenderEngine{
ShaderProgram::ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader){
	GLuint vertex_shader_id;
	if( !CreateShader(vertex_shader, GL_VERTEX_SHADER, vertex_shader_id) ){
		std::cerr << "VERTEX SHADER compile-time error" << std::endl;
		return;
	}

	GLuint fragment_shader_id;
	if( !CreateShader(fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_id) ){
		std::cerr << "FRAGMENT SHADER compile-time error" << std::endl;
		glDeleteShader(vertex_shader_id);
		return;
	}

	id_ = glCreateProgram();
	glAttachShader(id_, vertex_shader_id);
	glAttachShader(id_, fragment_shader_id);
	glLinkProgram(id_);

	GLint success;
	glGetProgramiv(id_, GL_LINK_STATUS, &success);

	if( !success ){
		GLchar info_log[1024];
		glGetShaderInfoLog(id_, 1024, nullptr, info_log);
		std::cerr << "ERROR::SHADER:Link-time error:\n" << info_log << std::endl;
	} else{
		is_compiled_ = true;
	}

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shader_program) noexcept{
	glDeleteProgram(id_);
	id_ = shader_program.id_;
	is_compiled_ = shader_program.is_compiled_;

	shader_program.id_ = 0;
	shader_program.is_compiled_ = false;
	return *this;
}

ShaderProgram::ShaderProgram(ShaderProgram&& shader_program) noexcept{
	id_ = shader_program.id_;
	is_compiled_ = shader_program.is_compiled_;

	shader_program.id_ = 0;
	shader_program.is_compiled_ = false;
}

ShaderProgram::~ShaderProgram(){
	glDeleteProgram(id_);
}

void ShaderProgram::Use() const{
	glUseProgram(id_);
}
void ShaderProgram::SetInt(const std::string& name, const GLint value){
	glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}
void ShaderProgram::SetFloat(const std::string& name, const GLfloat value){
	glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}
void ShaderProgram::SetMatrix4(const std::string& name, const glm::mat4& matrix){
	glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
bool ShaderProgram::CreateShader(const std::string& source, const GLenum shader_type, GLuint& shader_ID){
	shader_ID = glCreateShader(shader_type);
	const char* code{source.c_str()};
	glShaderSource(shader_ID, 1, &code, nullptr);
	glCompileShader(shader_ID);

	GLint success;
	glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);
	if( !success ){
		GLchar info_log[1024];
		glGetShaderInfoLog(shader_ID, 1024, nullptr, info_log);
		std::cerr << "ERROR::SHADER:Compile-time error:\n" << info_log << std::endl;
		return false;
	}
	return true;
}
}
