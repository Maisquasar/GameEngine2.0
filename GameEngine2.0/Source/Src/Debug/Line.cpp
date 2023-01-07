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
}

Debug::Line::~Line() {}

void Debug::Line::Initialize()
{
	_shader = Application.GetResourceManager()->GetDefaultShader();
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) + 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float), &_point1);
	glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float), 3 * sizeof(float), &_point2);


	// position attribute
	glVertexAttribPointer(0U, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0U);

	_initialized = true;
}

void Debug::Line::Draw(Math::Vector3 p1, Math::Vector3 p2)
{
	if (!_initialized)
		Initialize();
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float), &p1);
	glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float), 3 * sizeof(float), &p2);

	glDepthRange(0, 0.01);
	float defaultWidth;
	glGetFloatv(GL_LINE_WIDTH, &defaultWidth);
	glLineWidth(_lineWidth);
	glBindVertexArray(_VAO);
	glUseProgram(_shader->Program);
	auto MVP = Application.GetScene()->GetVPMatrix();
	glUniformMatrix4fv(_shader->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	glUniform1i(_shader->GetLocation(Resources::Location::L_ENABLE_TEXTURE), false);
	glUniform4f(_shader->GetLocation(Resources::Location::L_COLOR), 1, 1, 1, 1);

	glDraw(GL_LINES, 0, 9);
	glLineWidth(defaultWidth);
	glDepthRange(0.01, 1);
}
