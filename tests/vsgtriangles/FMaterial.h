#pragma once
#include <vsg/core/Inherit.h>
#include <vsg/state/material.h>

struct FMaterialExtraData
{
    VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
    VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
    VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    float depthBiasConstantFactor = 1.0f;
    float depthBiasClamp = 0.0f;
    float depthBiasSlopeFactor = 1.0f;
    float lineWidth = 1.0f;
    bool depthTest = true;
    bool depthBiasEnable = false;
    bool depthClampEnable = false;
    bool rasterizerDiscardEnable = false;
};

class FPhongMaterial;
class FPbrMaterial;

class FMaterial : public vsg::Inherit<vsg::Object, FMaterial>
{
public:
    virtual FPhongMaterial* phong_material() { return nullptr; }
    virtual FPbrMaterial* pbr_material() { return nullptr; }

private:
    std::unique_ptr<FMaterialExtraData> data;
};

class FPhongMaterial : public FMaterial
{
public:
    FPhongMaterial* phong_material() override { return this; }
    FPbrMaterial* pbr_material() override { return nullptr; }

private:
    vsg::PhongMaterial material;
};

class FPbrMaterial : public FMaterial
{
public:
    FPhongMaterial* phong_material() override { return nullptr; }
    FPbrMaterial* pbr_material() override { return this; }

private:
    vsg::PbrMaterial material;
};

namespace vsg
{
    VSG_type_name(FMaterial);
    VSG_type_name(FPhongMaterial);
    VSG_type_name(FPbrMaterial);
}