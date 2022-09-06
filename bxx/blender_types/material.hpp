#pragma once

#include <bxx/blender_types/iterables.hpp>
#include <bxx/blender_types/blender_types.hpp>
#include <bxx/objects/id.hpp>
#include <bxx/mathutils.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_material_types.h>
#include <makesdna/DNA_Node_types.h>
#pragma warning(pop)

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

    std::string node_type_to_string(material_node_type type);

    class material_node;
    class material : public id<bl_material>
    {
    public:
        using id<bl_material>::id;
        material_node add_node(std::string const& type, mathutils::vec2 const& location = { 0,0 });
        material_node add_node(material_node_type type, mathutils::vec2 const& location = { 0,0 });
        material_node get_node(std::string const& name);
        void connect(material_node const& output_node, int output_socket, material_node const& input_node, int input_socket);
        PYFIELD(bool,use_nodes)
    };

    class material_node : public blender_py_struct<bl_material_node>
    {
    public:
        PYFIELD(mathutils::vec2, location)
        material_node(material const& parent, python_object const& obj);
        material get_parent();
        std::string get_name() const;
        void set_default_input(int index, std::string const& value);
        void set_default_input(int index, int value);
        void set_default_input(int index, float v1);
        void set_default_input(int index, bool value);
        void set_default_input(int index, std::initializer_list<float> const& list);
    private:
        void _set_default_input_code(int index, std::string const& code);
        material m_parent;
    };

    class material_data : public blender_py_iterable<material>
    {
    public:
        using blender_py_iterable<material>::blender_py_iterable;
        material create(std::string const& name);
        material find(std::string const& name);
    };
}
