#include "Material.hpp"



#include <Shader.hpp>
void Material::InitShader(Shader& _lightShader)
{
	_lightShader.SetVec3("material.ambient", ambient);
	_lightShader.SetVec3("material.diffuse", diffuse);
	_lightShader.SetVec3("material.specular", specular);
	_lightShader.SetFloat("material.shininess", shininess * 128.f); // *128 for OpenGL
}

Material::Material(Vectorf3 _ambient, Vectorf3 _diffuse, Vectorf3 _specular, float _shiny)
{
	ambient = _ambient;
	diffuse = _diffuse;
	specular = _specular;
	shininess = _shiny;
}

Material::Material(float _ambientX, float _ambientY, float _ambientZ, float _diffuseX, float _diffuseY, float _diffuseZ, 	float _specularX, float _specularY, float _specularZ, float _shiny): Material({ _ambientX ,_ambientY,_ambientZ },{ _diffuseX, _diffuseY, _diffuseZ},{ _specularX ,_specularY, _specularZ },_shiny)
{}

void Material::LoadResource(const char* _name)
{
}

void Material::UnloadResource()
{
}
