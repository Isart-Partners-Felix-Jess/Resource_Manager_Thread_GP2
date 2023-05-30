#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <ResourcesManager.hpp>

class Texture : public IResource
{
private:
    int m_Width, m_Height, m_Channels;
public:
    Texture();
    Texture(const char* _filepath);
    ~Texture();

    unsigned int GetID() const;

    // Hérité via IResource
    virtual void LoadResource(const char* _name) override;
    virtual void UnloadResource() override;
};
#endif //TEXTURE_H