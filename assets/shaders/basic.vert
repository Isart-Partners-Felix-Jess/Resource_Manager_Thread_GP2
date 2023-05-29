#version 330 core
layout(location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord; // the color variable has attribute position 2
layout (location = 3) in vec3 aNormal;

out vec3 ourColor;
out vec2 TexCoord;
out vec3 FragPos;  
out vec3 Normal;

uniform mat4 MVP;
uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
     ourColor = aColor; 
     TexCoord = aTexCoord;
     FragPos = vec3(model * vec4(aPos,1.0));
     Normal = normalMatrix * aNormal;
}