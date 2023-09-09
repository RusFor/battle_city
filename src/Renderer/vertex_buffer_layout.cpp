#include "vertex_buffer_layout.h"

Renderer::VertexBufferLayout::VertexBufferLayout() : stride_(0){}

void Renderer::VertexBufferLayout::ReserveElements(const size_t count){
	layout_elements_.reserve(count);
}

void Renderer::VertexBufferLayout::AddElementLayoutFloat(const int32_t count, const bool normalized){
	layout_elements_.push_back({count, GL_FLOAT, normalized, count * sizeof(GLfloat)});
	stride_ += layout_elements_.back().size;
}
