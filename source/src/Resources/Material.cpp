#include <Material.hpp>

Material::Material() 
{
	m_isRead = true;
	m_isLoaded = true;
	*this = material::none;
};

void Material::InitShader(Shader& _lightShader)
{
	_lightShader.SetVec3("material.ambient", ambient);
	_lightShader.SetVec3("material.diffuse", diffuse);
	_lightShader.SetVec3("material.specular", specular);
	_lightShader.SetFloat("material.shininess", shininess * 128.f); // *128 for OpenGL
	if (diffuse2DMap != -1)
		_lightShader.SetInt("material.diffuse2D", diffuse2DMap);
	if (specular2DMap != -1)
		_lightShader.SetInt("material.specular2D", specular2DMap);
}

Material::Material(Vectorf3 _ambient, Vectorf3 _diffuse, Vectorf3 _specular, float _shiny)
{
	ambient = _ambient;
	diffuse = _diffuse;
	specular = _specular;
	shininess = _shiny;
	diffuse2DMap = 0;
}

Material::Material(float _ambientX, float _ambientY, float _ambientZ, float _diffuseX, float _diffuseY, float _diffuseZ, float _specularX, float _specularY, float _specularZ, float _shiny)
{
	ambient = Vectorf3{ _ambientX,_ambientY, _ambientZ };
	diffuse = Vectorf3{ _diffuseX,_diffuseY, _diffuseZ };
	specular = Vectorf3{ _specularX,_specularY, _specularZ };
	shininess = _shiny;
	diffuse2DMap = 0;
};

void Material::AttachDiffuseMap(const Texture* _diffuseMap) {
	diffuse2DMap = _diffuseMap->GetResourceId();
}

void Material::AttachDiffuseMap(unsigned int _diffuseMapID) {
	diffuse2DMap = _diffuseMapID;
}

void Material::DetachDiffuseMap() {
	diffuse2DMap = 0;
}

void Material::AttachSpecularMap(const Texture* _specularMap) {
	specular2DMap = _specularMap->GetResourceId();
}

void Material::AttachSpecularMap(unsigned int _specularMapID) {
	specular2DMap = _specularMapID;
}

void Material::DetachSpecularMap() {
	specular2DMap = 0;
}