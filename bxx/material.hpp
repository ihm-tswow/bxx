#pragma once

#include "id.hpp"
#include "mathutils.hpp"

typedef struct Material bl_material;
typedef struct bNode bl_material_node;

namespace bxx
{
    enum class material_node_type
    {
        ShaderNodeAddShader,
        ShaderNodeAmbientOcclusion,
        ShaderNodeAttribute,
        ShaderNodeBackground,
        ShaderNodeBevel,
        ShaderNodeBlackbody,
        ShaderNodeBrightContrast,
        ShaderNodeBsdfAnisotropic,
        ShaderNodeBsdfDiffuse,
        ShaderNodeBsdfGlass,
        ShaderNodeBsdfGlossy,
        ShaderNodeBsdfHair,
        ShaderNodeBsdfHairPrincipled,
        ShaderNodeBsdfPrincipled,
        ShaderNodeBsdfRefraction,
        ShaderNodeBsdfToon,
        ShaderNodeBsdfTranslucent,
        ShaderNodeBsdfTransparent,
        ShaderNodeBsdfVelvet,
        ShaderNodeBump,
        ShaderNodeCameraData,
        ShaderNodeClamp,
        ShaderNodeCombineHSV,
        ShaderNodeCombineRGB,
        ShaderNodeCombineXYZ,
        ShaderNodeCustomGroup,
        ShaderNodeDisplacement,
        ShaderNodeEeveeSpecular,
        ShaderNodeEmission,
        ShaderNodeFloatCurve,
        ShaderNodeFresnel,
        ShaderNodeGamma,
        ShaderNodeGroup,
        ShaderNodeHairInfo,
        ShaderNodeHoldout,
        ShaderNodeHueSaturation,
        ShaderNodeInvert,
        ShaderNodeLayerWeight,
        ShaderNodeLightFalloff,
        ShaderNodeLightPath,
        ShaderNodeMapRange,
        ShaderNodeMapping,
        ShaderNodeMath,
        ShaderNodeMixRGB,
        ShaderNodeMixShader,
        ShaderNodeNewGeometry,
        ShaderNodeNormal,
        ShaderNodeNormalMap,
        ShaderNodeObjectInfo,
        ShaderNodeOutputAOV,
        ShaderNodeOutputLight,
        ShaderNodeOutputLineStyle,
        ShaderNodeOutputMaterial,
        ShaderNodeOutputWorld,
        ShaderNodeParticleInfo,
        ShaderNodePointInfo,
        ShaderNodeRGB,
        ShaderNodeRGBCurve,
        ShaderNodeRGBToBW,
        ShaderNodeScript,
        ShaderNodeSeparateHSV,
        ShaderNodeSeparateRGB,
        ShaderNodeSeparateXYZ,
        ShaderNodeShaderToRGB,
        ShaderNodeSqueeze,
        ShaderNodeSubsurfaceScattering,
        ShaderNodeTangent,
        ShaderNodeTexBrick,
        ShaderNodeTexChecker,
        ShaderNodeTexCoord,
        ShaderNodeTexEnvironment,
        ShaderNodeTexGradient,
        ShaderNodeTexIES,
        ShaderNodeTexImage,
        ShaderNodeTexMagic,
        ShaderNodeTexMusgrave,
        ShaderNodeTexNoise,
        ShaderNodeTexPointDensity,
        ShaderNodeTexSky,
        ShaderNodeTexVoronoi,
        ShaderNodeTexWave,
        ShaderNodeTexWhiteNoise,
        ShaderNodeUVAlongStroke,
        ShaderNodeUVMap,
        ShaderNodeValToRGB,
        ShaderNodeValue,
        ShaderNodeVectorCurve,
        ShaderNodeVectorDisplacement,
        ShaderNodeVectorMath,
        ShaderNodeVectorRotate,
        ShaderNodeVectorTransform,
        ShaderNodeVertexColor,
        ShaderNodeVolumeAbsorption,
        ShaderNodeVolumeInfo,
        ShaderNodeVolumePrincipled,
        ShaderNodeVolumeScatter,
        ShaderNodeWavelength,
        ShaderNodeWireframe
    };

    std::string node_type_to_string(bxx::material_node_type type);

    class material_node;
    class material : public id
    {
    public:
        static bxx::material create(std::string const& name);
        material(bl_material* raw);
        std::string get_name() const override;
        std::string get_full_name() const override;
        material_node add_node(std::string const& type, mathutils::vec2 const& location = { 0,0 });
        material_node add_node(bxx::material_node_type type, mathutils::vec2 const& location = { 0,0 });
        material_node get_node(std::string const& name);
        void connect(material_node const& output_node, int output_socket, material_node const& input_node, int input_socket);
        void set_use_nodes(bool use_nodes);
        bl_material* get_raw();
    private:
        bl_material* m_raw;
    };

    class material_node : public id
    {
    public:
        material_node(bxx::material const& parent, bl_material_node* node);
        bl_material_node* get_raw();
        bxx::material get_parent() const;
        std::string get_name() const override;
        std::string get_full_name() const override;
        void set_location(mathutils::vec2 const& location);
        void set_default_input(int index, std::string const& value);
        void set_default_input(int index, int value);
        void set_default_input(int index, float v1);
        void set_default_input(int index, bool value);
        void set_default_input(int index, std::initializer_list<float> const& list);
    private:
        void _set_default_input_code(int index, std::string const& code);
        bxx::material m_parent;
        bl_material_node* m_raw;
    };
}
