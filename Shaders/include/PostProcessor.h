#pragma once

#include <memory>
#include <array>

class Shader;

class PostProcessor {
private:
	float m_QuadWidth;
	float m_QuadHeight;
	float m_AccumulatedTime = 0;
	
	unsigned int m_FrameBufferObject;
	unsigned int m_TextureID;
	unsigned int m_RenderBufferObject;	
	unsigned int m_VAO;
	unsigned int m_VBO;

	std::shared_ptr<Shader> m_Shader;
	bool m_Shake = false;
	bool m_InvertColours = false;
	bool m_Chaos = false;

	void InitializeRenderData();

public:
	PostProcessor(float p_QuadWidth, float p_QuadHeight);
	~PostProcessor();

	void Update(float p_DeltaTime);	

	void BeginRender();
	void Render();

	bool GetShakeState() const {
		return m_Shake;
	}
	void SetShakeState(bool p_ShakeState) {
		m_Shake = p_ShakeState;
	}

	bool GetInvertColoursState() const {
		return m_InvertColours;
	}
	void SetInvertColoursState(bool p_InvertColoursState) {
		m_InvertColours = p_InvertColoursState;
		if (m_InvertColours) {
			m_Chaos = false;
		}
	}

	bool GetChaosState() const {
		return m_Chaos;
	}
	void SetChaosState(bool p_ChaosState) {
		m_Chaos = p_ChaosState;
		if (m_Chaos) {
			m_InvertColours = false;
		}
	}

	void SetTextureSize(float p_QuadWidth, float p_QuadHeight) {
		m_QuadWidth = p_QuadWidth;
		m_QuadHeight = p_QuadHeight;
	}
};