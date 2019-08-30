#include "PostProcessor.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include <glad/glad.h>

#include "ResourceManager.h"
#include "shader.h"

PostProcessor::PostProcessor(float p_QuadWidth, float p_QuadHeight) 
	: m_QuadWidth(p_QuadWidth), m_QuadHeight(p_QuadHeight) {
	m_Shader = ResourceManagerInstance.GetShader("postProcessingEffects");

	std::cout << "Texture width: " << p_QuadWidth << "\t" << "Texture height: " << p_QuadHeight;

	glGenFramebuffers(1, &m_FrameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);

	// Create a colour attachment texture.
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)p_QuadWidth, (GLsizei)p_QuadHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

	// Create a renderbuffer object for the depth and stencil attachment, as we won't be sampling these.
	glGenRenderbuffers(1, &m_RenderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)p_QuadWidth, (GLsizei)p_QuadHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferObject);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	InitializeRenderData();

	// Set uniforms.
	m_Shader->Use();
	m_Shader->SetInt("scene", 0);
	float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{ -offset,  offset  },  // Top-left.
		{  0.0f,    offset  },  // Top-center.
		{  offset,  offset  },  // Top-right.
		{ -offset,  0.0f    },  // Center-left.
		{  0.0f,    0.0f    },  // Center-center.
		{  offset,  0.0f    },  // Center-right.
		{ -offset, -offset  },  // Bottom-left.
		{  0.0f,   -offset  },  // Bottom-center.
		{  offset, -offset  }   // Bottom-right.
	};
	glUniform2fv(glGetUniformLocation(m_Shader->GetID(), "offsets"), 9, (GLfloat*)offsets);

	int edgeKernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(m_Shader->GetID(), "edgeKernel"), 9, edgeKernel);

	float blurKernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};
	glUniform1fv(glGetUniformLocation(m_Shader->GetID(), "blurKernel"), 9, &blurKernel[0]);
}

PostProcessor::~PostProcessor() {
	// Set the Frame Buffer Object back to the default one.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clean up the memory.
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteRenderbuffers(1, &m_RenderBufferObject);
	glDeleteFramebuffers(1, &m_FrameBufferObject);
}

void PostProcessor::InitializeRenderData() {
	float vertices[] = {
		// Positions  // Texture Coordinates.
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), NULL);

	// Unbind.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PostProcessor::Update(float p_DeltaTime) {
	m_AccumulatedTime += p_DeltaTime;

	m_Shader->Use();
	m_Shader->SetFloat("time", m_AccumulatedTime);
}

void PostProcessor::BeginRender() {
	//glBindFramebuffer(GL_FRAMEBUFFER, m_MultisampledFrameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessor::Render() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_Shader->Use();
	// Set uniforms.
	m_Shader->SetBool("shake", m_Shake);
	m_Shader->SetBool("invertColours", m_InvertColours);
	m_Shader->SetBool("chaos", m_Chaos);
	
	// Render the textured quad.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Unbind the VAO.
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}