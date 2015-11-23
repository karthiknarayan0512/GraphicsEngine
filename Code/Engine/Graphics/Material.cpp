#include "Material.h"

#include <fstream>

void eae6320::Graphics::Material::LoadMaterial(const char *i_materialPath)
{
	std::ifstream materialBinary(i_materialPath, std::ofstream::binary);

	materialBinary.seekg(0, materialBinary.end);
	int length = (int)materialBinary.tellg();
	materialBinary.seekg(0, materialBinary.beg);

	char * buffer = new char[length];
	materialBinary.read(buffer, length);

	char* effectPath = new char[strlen(buffer) + 1];
	memcpy_s(effectPath, strlen(buffer) + 1, buffer, strlen(buffer) + 1);

	m_Effect.CreateEffect(effectPath);

	buffer += strlen(buffer) + 1;

	m_UniformCount = *reinterpret_cast<uint8_t *>(buffer);

	buffer += sizeof(m_UniformCount);
	m_Uniforms = new sParameterToSet[m_UniformCount];

	for (uint8_t i = 0; i < m_UniformCount; i++)
	{
		char* uniformName = new char[strlen(buffer) + 1];
		memcpy_s(uniformName, strlen(buffer) + 1, buffer, strlen(buffer) + 1);

		buffer += strlen(buffer) + 1;
		memcpy_s(&m_Uniforms[i], sizeof(sParameterToSet), buffer, sizeof(sParameterToSet));

		m_Uniforms[i].uniformHandle = m_Effect.getUniformHandle(ShaderTypes::Fragment, uniformName);
		buffer += sizeof(sParameterToSet);
	}
}

void eae6320::Graphics::Material::SetMaterial(Math::cMatrix_transformation i_localToWorldTransform, Camera &i_Camera)
{
	m_Effect.SetEffect();
	m_Effect.SetTransforms(i_localToWorldTransform, i_Camera);

	for (uint8_t i = 0; i < m_UniformCount; i++)
		m_Effect.SetUniforms(m_Uniforms[i].shaderType, m_Uniforms[i].uniformHandle, m_Uniforms[i].values, m_Uniforms[i].valueCountToSet);
}
	