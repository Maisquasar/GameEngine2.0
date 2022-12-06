#include "Include/Debug/Line.h"

#include <glad/glad.h>
#include <vector>
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"

Debug::Line::Line() {}

Debug::Line::Line(Math::Vector3 p1, Math::Vector3 p2, float lineWidth)
{
	_point1 = p1;
	_point2 = p2;
	_lineWidth = lineWidth;
	Initialize();
}

Debug::Line::~Line() {}

void Debug::Line::Initialize()
{
	_shader = Resources::ResourceManager::GetDefaultShader();
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	//glGenBuffers(1, &EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(_VAO);

	std::vector<float> Vertices;
	Vertices.push_back(_point1.x);
	Vertices.push_back(_point1.y);
	Vertices.push_back(_point1.z);
	Vertices.push_back(_point2.x);
	Vertices.push_back(_point2.y);
	Vertices.push_back(_point2.z);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	if (!Vertices.empty()) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0U, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0U);
	}
}

void Debug::Line::Draw()
{
	float defaultWidth;
	glGetFloatv(GL_LINE_WIDTH, &defaultWidth);
	glLineWidth(_lineWidth);
	glBindVertexArray(_VAO);
	glUseProgram(_shader->Program);
	auto MVP = App::GetVPMatrix() * Math::Matrix4::Identity();
	glUniformMatrix4fv(_shader->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	glDepthRange(0.01, 1.0);
	glUniform1i(_shader->GetLocation(Resources::Location::L_ENABLE_TEXTURE), false);
	glUniform4f(_shader->GetLocation(Resources::Location::L_COLOR), 1, 1, 1, 1);

	glDrawArrays(GL_LINES, 0, 9);
	glLineWidth(defaultWidth);
}
