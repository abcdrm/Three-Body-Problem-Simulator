#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 ModelMatrix;
uniform mat4 VPMatrix;

void main()
{	
	FragPos = vec3(ModelMatrix * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(ModelMatrix))) * normal;
    TexCoords = texCoords;
    gl_Position = VPMatrix * vec4(FragPos, 1.0f);
}