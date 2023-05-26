#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include<ResourcesManager.hpp>

#include <vectorM.hpp>
class Shader;
class Texture;
class Material :
    public IResource
{
public: //for now
    Vectorf3 ambient{ 1.f, 1.f, 1.f };
    Vectorf3 diffuse{ 1.f, 1.f, 1.f };
    Vectorf3 specular{ 1.f, 1.f, 1.f };
    float shininess = 1.f;
    int diffuse2DMap = 0; //No map (null) is a valid entry
    int specular2DMap = 0; //No map (null) is a valid entry

    //Should go in Renderer
    void InitShader(Shader& _lightShader);
    Material();
    Material(Vectorf3 _ambient, Vectorf3 _diffuse, Vectorf3 _specular, float _shiny);
    Material(float _ambientX, float _ambientY, float _ambientZ, float _diffuseX, float _diffuseY, float _diffuseZ, float _specularX, float _specularY, float _specularZ, float _shiny);
    void AttachDiffuseMap(Texture* _diffuseMap);
    void AttachDiffuseMap(unsigned int _diffuseMapID);
    void DetachDiffuseMap();

    void AttachSpecularMap(Texture* _specularMap);
    void AttachSpecularMap(unsigned int _specularMapID);
    void DetachSpecularMap();

private:
    // Inherited from IResource
    virtual void LoadResource(const char* _name) override;
    virtual void UnloadResource() override;
};
namespace material
{
    static Material none;
    static Material emerald(0.0215f, 0.1745f, 0.0215f, 0.07568f, 0.61424f, 0.07568f, 0.633f, 0.727811f, 0.633f, 0.6);
    static Material jade(0.135f, 0.2225f, 0.1575f, 0.54f, 0.89f, 0.63f, 0.316228f, 0.316228f, 0.316228f, 0.1);
    static Material obsidian(0.05375f, 0.05f, 0.06625f, 0.18275f, 0.17f, 0.22525f, 0.332741f, 0.328634f, 0.346435f, 0.3);
    static Material pearl(0.25f, 0.20725f, 0.20725f, 1.f, 0.829f, 0.829f, 0.296648f, 0.296648f, 0.296648f, 0.088);
    static Material ruby(0.1745f, 0.01175f, 0.01175f, 0.61424f, 0.04136f, 0.04136f, 0.727811f, 0.626959f, 0.626959f, 0.6);
    static Material turquoise(0.1f, 0.18725f, 0.1745f, 0.396f, 0.74151f, 0.69102f, 0.297254f, 0.30829f, 0.306678f, 0.1);
    static Material brass(0.329412f, 0.223529f, 0.027451f, 0.780392f, 0.568627f, 0.113725f, 0.992157f, 0.941176f, 0.807843f, 0.21794872);
    static Material bronze(0.2125f, 0.1275f, 0.054f, 0.714f, 0.4284f, 0.18144f, 0.393548f, 0.271906f, 0.166721f, 0.2);
    static Material chrome(0.25f, 0.25f, 0.25f, 0.4f, 0.4f, 0.4f, 0.774597f, 0.774597f, 0.774597f, 0.6);
    static Material copper(0.19125f, 0.0735f, 0.0225f, 0.7038f, 0.27048f, 0.0828f, 0.256777f, 0.137622f, 0.086014f, 0.1);
    static Material gold(0.24725f, 0.1995f, 0.0745f, 0.75164f, 0.60648f, 0.22648f, 0.628281f, 0.555802f, 0.366065f, 0.4);
    static Material silver(0.19225f, 0.19225f, 0.19225f, 0.50754f, 0.50754f, 0.50754f, 0.508273f, 0.508273f, 0.508273f, 0.4);
    static Material black_plastic(0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f, 0.50f, 0.50f, 0.50f, .25);
    static Material cyan_plastic(0.0f, 0.1f, 0.06f, 0.0f, 0.50980392f, 0.50980392f, 0.50196078f, 0.50196078f, 0.50196078f, .25);
    static Material green_plastic(0.0f, 0.0f, 0.0f, 0.1f, 0.35f, 0.1f, 0.45f, 0.55f, 0.45f, .25);
    static Material red_plastic(0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.7f, 0.6f, 0.6f, .25);
    static Material white_plastic(0.0f, 0.0f, 0.0f, 0.55f, 0.55f, 0.55f, 0.70f, 0.70f, 0.70f, .25);
    static Material yellow_plastic(0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.60f, 0.60f, 0.50f, .25);
    static Material black_rubber(0.02f, 0.02f, 0.02f, 0.01f, 0.01f, 0.01f, 0.4f, 0.4f, 0.4f, .078125);
    static Material cyan_rubber(0.0f, 0.05f, 0.05f, 0.4f, 0.5f, 0.5f, 0.04f, 0.7f, 0.7f, .078125);
    static Material green_rubber(0.0f, 0.05f, 0.0f, 0.4f, 0.5f, 0.4f, 0.04f, 0.7f, 0.04f, .078125);
    static Material red_rubber(0.05f, 0.0f, 0.0f, 0.5f, 0.4f, 0.4f, 0.7f, 0.04f, 0.04f, .078125);
    static Material white_rubber(0.05f, 0.05f, 0.05f, 0.5f, 0.5f, 0.5f, 0.7f, 0.7f, 0.7f, .078125);
    static Material yellow_rubber(0.05f, 0.05f, 0.0f, 0.5f, 0.5f, 0.4f, 0.7f, 0.7f, 0.04f, .078125);

    static Material list[] = { emerald, jade, obsidian, pearl, ruby, turquoise, brass, bronze, chrome, copper, gold, silver, black_plastic, cyan_plastic, green_plastic, red_plastic, white_plastic, yellow_plastic, black_rubber, cyan_rubber, green_rubber, red_rubber, white_rubber, yellow_rubber };
};
#endif // !MATERIAL_H