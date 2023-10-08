#include <glm/glm.hpp>
#include "Wrappers.hpp"

struct InterfaceMap
{
	enum Resource { M, MVP, CAMPOS, SHINE, LIGHTS, N_RESOURCES };
	std::array<GLint, N_RESOURCES> map;
	GLProgram& program;
	InterfaceMap(GLProgram& program)
		: program(program)
	{
		map[M] = program.GetResourceLocation(GL_UNIFORM, "M");
		map[MVP] = program.GetResourceLocation(GL_UNIFORM, "MVP");
		map[CAMPOS] = program.GetResourceLocation(GL_UNIFORM, "camPos");
		map[SHINE] = program.GetResourceIndex(GL_UNIFORM, "shine");
		map[LIGHTS] = program.GetResourceIndex(GL_SHADER_STORAGE_BLOCK, "Lights");
	}
	bool needsLights() { return map[LIGHTS] == -1; }
	void SetM(glm::mat4 mat)
	{
		if (map[M] != -1)
			program.UniformMatrix4fv(map[M], 1, false, glm::value_ptr(mat));
	}
	void SetCamPos(glm::vec3 vec)
	{
		if (map[CAMPOS] != -1)
			program.Uniform3f(map[CAMPOS], vec);
	}
	void SetMVP(glm::mat4 mat)
	{
		if (map[MVP] != -1)
			program.UniformMatrix4fv(map[MVP], 1, false, glm::value_ptr(mat));
	}
	void SetShine(float shine)
	{
		if (map[SHINE] != -1)
			program.Uniform1f(map[SHINE], shine);
	}
};