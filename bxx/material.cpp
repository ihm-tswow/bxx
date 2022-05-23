#include "material.hpp"
#include "exec.hpp"

#include "makesdna/DNA_material_types.h"
#include "makesdna/DNA_Node_types.h"

#include "fmt/core.h"

std::string bxx::node_type_to_string(bxx::material_node_type type)
{
    switch (type)
    {
    case material_node_type::ShaderNodeAddShader: return "ShaderNodeAddShader";
    case material_node_type::ShaderNodeAmbientOcclusion: return "ShaderNodeAmbientOcclusion";
    case material_node_type::ShaderNodeAttribute: return "ShaderNodeAttribute";
    case material_node_type::ShaderNodeBackground: return "ShaderNodeBackground";
    case material_node_type::ShaderNodeBevel: return "ShaderNodeBevel";
    case material_node_type::ShaderNodeBlackbody: return "ShaderNodeBlackbody";
    case material_node_type::ShaderNodeBrightContrast: return "ShaderNodeBrightContrast";
    case material_node_type::ShaderNodeBsdfAnisotropic: return "ShaderNodeBsdfAnisotropic";
    case material_node_type::ShaderNodeBsdfDiffuse: return "ShaderNodeBsdfDiffuse";
    case material_node_type::ShaderNodeBsdfGlass: return "ShaderNodeBsdfGlass";
    case material_node_type::ShaderNodeBsdfGlossy: return "ShaderNodeBsdfGlossy";
    case material_node_type::ShaderNodeBsdfHair: return "ShaderNodeBsdfHair";
    case material_node_type::ShaderNodeBsdfHairPrincipled: return "ShaderNodeBsdfHairPrincipled";
    case material_node_type::ShaderNodeBsdfPrincipled: return "ShaderNodeBsdfPrincipled";
    case material_node_type::ShaderNodeBsdfRefraction: return "ShaderNodeBsdfRefraction";
    case material_node_type::ShaderNodeBsdfToon: return "ShaderNodeBsdfToon";
    case material_node_type::ShaderNodeBsdfTranslucent: return "ShaderNodeBsdfTranslucent";
    case material_node_type::ShaderNodeBsdfTransparent: return "ShaderNodeBsdfTransparent";
    case material_node_type::ShaderNodeBsdfVelvet: return "ShaderNodeBsdfVelvet";
    case material_node_type::ShaderNodeBump: return "ShaderNodeBump";
    case material_node_type::ShaderNodeCameraData: return "ShaderNodeCameraData";
    case material_node_type::ShaderNodeClamp: return "ShaderNodeClamp";
    case material_node_type::ShaderNodeCombineHSV: return "ShaderNodeCombineHSV";
    case material_node_type::ShaderNodeCombineRGB: return "ShaderNodeCombineRGB";
    case material_node_type::ShaderNodeCombineXYZ: return "ShaderNodeCombineXYZ";
    case material_node_type::ShaderNodeCustomGroup: return "ShaderNodeCustomGroup";
    case material_node_type::ShaderNodeDisplacement: return "ShaderNodeDisplacement";
    case material_node_type::ShaderNodeEeveeSpecular: return "ShaderNodeEeveeSpecular";
    case material_node_type::ShaderNodeEmission: return "ShaderNodeEmission";
    case material_node_type::ShaderNodeFloatCurve: return "ShaderNodeFloatCurve";
    case material_node_type::ShaderNodeFresnel: return "ShaderNodeFresnel";
    case material_node_type::ShaderNodeGamma: return "ShaderNodeGamma";
    case material_node_type::ShaderNodeGroup: return "ShaderNodeGroup";
    case material_node_type::ShaderNodeHairInfo: return "ShaderNodeHairInfo";
    case material_node_type::ShaderNodeHoldout: return "ShaderNodeHoldout";
    case material_node_type::ShaderNodeHueSaturation: return "ShaderNodeHueSaturation";
    case material_node_type::ShaderNodeInvert: return "ShaderNodeInvert";
    case material_node_type::ShaderNodeLayerWeight: return "ShaderNodeLayerWeight";
    case material_node_type::ShaderNodeLightFalloff: return "ShaderNodeLightFalloff";
    case material_node_type::ShaderNodeLightPath: return "ShaderNodeLightPath";
    case material_node_type::ShaderNodeMapRange: return "ShaderNodeMapRange";
    case material_node_type::ShaderNodeMapping: return "ShaderNodeMapping";
    case material_node_type::ShaderNodeMath: return "ShaderNodeMath";
    case material_node_type::ShaderNodeMixRGB: return "ShaderNodeMixRGB";
    case material_node_type::ShaderNodeMixShader: return "ShaderNodeMixShader";
    case material_node_type::ShaderNodeNewGeometry: return "ShaderNodeNewGeometry";
    case material_node_type::ShaderNodeNormal: return "ShaderNodeNormal";
    case material_node_type::ShaderNodeNormalMap: return "ShaderNodeNormalMap";
    case material_node_type::ShaderNodeObjectInfo: return "ShaderNodeObjectInfo";
    case material_node_type::ShaderNodeOutputAOV: return "ShaderNodeOutputAOV";
    case material_node_type::ShaderNodeOutputLight: return "ShaderNodeOutputLight";
    case material_node_type::ShaderNodeOutputLineStyle: return "ShaderNodeOutputLineStyle";
    case material_node_type::ShaderNodeOutputMaterial: return "ShaderNodeOutputMaterial";
    case material_node_type::ShaderNodeOutputWorld: return "ShaderNodeOutputWorld";
    case material_node_type::ShaderNodeParticleInfo: return "ShaderNodeParticleInfo";
    case material_node_type::ShaderNodePointInfo: return "ShaderNodePointInfo";
    case material_node_type::ShaderNodeRGB: return "ShaderNodeRGB";
    case material_node_type::ShaderNodeRGBCurve: return "ShaderNodeRGBCurve";
    case material_node_type::ShaderNodeRGBToBW: return "ShaderNodeRGBToBW";
    case material_node_type::ShaderNodeScript: return "ShaderNodeScript";
    case material_node_type::ShaderNodeSeparateHSV: return "ShaderNodeSeparateHSV";
    case material_node_type::ShaderNodeSeparateRGB: return "ShaderNodeSeparateRGB";
    case material_node_type::ShaderNodeSeparateXYZ: return "ShaderNodeSeparateXYZ";
    case material_node_type::ShaderNodeShaderToRGB: return "ShaderNodeShaderToRGB";
    case material_node_type::ShaderNodeSqueeze: return "ShaderNodeSqueeze";
    case material_node_type::ShaderNodeSubsurfaceScattering: return "ShaderNodeSubsurfaceScattering";
    case material_node_type::ShaderNodeTangent: return "ShaderNodeTangent";
    case material_node_type::ShaderNodeTexBrick: return "ShaderNodeTexBrick";
    case material_node_type::ShaderNodeTexChecker: return "ShaderNodeTexChecker";
    case material_node_type::ShaderNodeTexCoord: return "ShaderNodeTexCoord";
    case material_node_type::ShaderNodeTexEnvironment: return "ShaderNodeTexEnvironment";
    case material_node_type::ShaderNodeTexGradient: return "ShaderNodeTexGradient";
    case material_node_type::ShaderNodeTexIES: return "ShaderNodeTexIES";
    case material_node_type::ShaderNodeTexImage: return "ShaderNodeTexImage";
    case material_node_type::ShaderNodeTexMagic: return "ShaderNodeTexMagic";
    case material_node_type::ShaderNodeTexMusgrave: return "ShaderNodeTexMusgrave";
    case material_node_type::ShaderNodeTexNoise: return "ShaderNodeTexNoise";
    case material_node_type::ShaderNodeTexPointDensity: return "ShaderNodeTexPointDensity";
    case material_node_type::ShaderNodeTexSky: return "ShaderNodeTexSky";
    case material_node_type::ShaderNodeTexVoronoi: return "ShaderNodeTexVoronoi";
    case material_node_type::ShaderNodeTexWave: return "ShaderNodeTexWave";
    case material_node_type::ShaderNodeTexWhiteNoise: return "ShaderNodeTexWhiteNoise";
    case material_node_type::ShaderNodeUVAlongStroke: return "ShaderNodeUVAlongStroke";
    case material_node_type::ShaderNodeUVMap: return "ShaderNodeUVMap";
    case material_node_type::ShaderNodeValToRGB: return "ShaderNodeValToRGB";
    case material_node_type::ShaderNodeValue: return "ShaderNodeValue";
    case material_node_type::ShaderNodeVectorCurve: return "ShaderNodeVectorCurve";
    case material_node_type::ShaderNodeVectorDisplacement: return "ShaderNodeVectorDisplacement";
    case material_node_type::ShaderNodeVectorMath: return "ShaderNodeVectorMath";
    case material_node_type::ShaderNodeVectorRotate: return "ShaderNodeVectorRotate";
    case material_node_type::ShaderNodeVectorTransform: return "ShaderNodeVectorTransform";
    case material_node_type::ShaderNodeVertexColor: return "ShaderNodeVertexColor";
    case material_node_type::ShaderNodeVolumeAbsorption: return "ShaderNodeVolumeAbsorption";
    case material_node_type::ShaderNodeVolumeInfo: return "ShaderNodeVolumeInfo";
    case material_node_type::ShaderNodeVolumePrincipled: return "ShaderNodeVolumePrincipled";
    case material_node_type::ShaderNodeVolumeScatter: return "ShaderNodeVolumeScatter";
    case material_node_type::ShaderNodeWavelength: return "ShaderNodeWavelength";
    case material_node_type::ShaderNodeWireframe: return "ShaderNodeWireframe";
    }
}

bxx::material_node::material_node(bxx::material const& parent, bl_material_node* node)
    : m_parent(parent)
    , m_raw(node)
{}

void bxx::material_node::set_location(mathutils::vec2 const& vec)
{
    set_float_prop("location", vec.x, vec.y);
}

bl_material_node* bxx::material_node::get_raw()
{
    return m_raw;
}

bxx::material bxx::material_node::get_parent() const
{
    return m_parent;
}

std::string bxx::material_node::get_name() const
{
    return m_raw->name;
}

std::string bxx::material_node::get_full_name() const
{
    return fmt::format("{}.node_tree.nodes['{}']",get_parent().get_full_name(), get_name());
}

bxx::material bxx::material::create(std::string const& name)
{
    return eval_ptr<bl_material>({
        fmt::format("out = bpy.data.materials.new(name='{}').as_pointer()", name)
    });
}

bxx::material::material(bl_material* raw)
    : m_raw(raw)
{}

void bxx::material::set_use_nodes(bool use_nodes)
{
    exec(fmt::format("{}.use_nodes = {}", get_full_name(), use_nodes ? "True" : "False"));
}

std::string bxx::material::get_name() const
{
    return m_raw->id.name + 2;
}

std::string bxx::material::get_full_name() const
{
    return fmt::format("bpy.data.materials['{}']", get_name());
}

bl_material* bxx::material::get_raw()
{
    return m_raw;
}

bxx::material_node bxx::material::add_node(std::string const& type, mathutils::vec2 const& location)
{
    set_use_nodes(true);
    material_node node = bxx::material_node(*this,
        eval_ptr<bl_material_node>({
            fmt::format("out = {}.node_tree.nodes.new('{}').as_pointer()", get_full_name(), type)
        })
    );
    node.set_location(location);
    return node;
}

bxx::material_node bxx::material::add_node(bxx::material_node_type type, mathutils::vec2 const& location)
{
    return add_node(node_type_to_string(type), location);
}

bxx::material_node bxx::material::get_node(std::string const& name)
{
    return bxx::material_node(*this,
        eval_ptr<bl_material_node>({
            fmt::format("out = {}.node_tree.nodes['{}'].as_pointer()", get_full_name(), name)
        })
    );
}


void bxx::material_node::_set_default_input_code(int index, std::string const& code)
{
    exec({
        "{}.inputs[{}].default_value = {code}"
    });
}

void bxx::material_node::set_default_input(int index, std::string const& value)
{
    _set_default_input_code(index, fmt::format("\"{}\"", value));
}

void bxx::material_node::set_default_input(int index, int value)
{
    _set_default_input_code(index, fmt::format("{}", value));
}

void bxx::material_node::set_default_input(int index, float v1)
{
    _set_default_input_code(index, fmt::format("{}", v1));
}

void bxx::material_node::set_default_input(int index, bool value)
{
    _set_default_input_code(index, fmt::format("{}", value));
}

void bxx::material_node::set_default_input(int index, std::initializer_list<float> const& list)
{
    std::string code = "(";
    for (float f : list) code += std::to_string(f);
    code += ")";
    _set_default_input_code(index, code);
}

void bxx::material::connect(material_node const& output_node, int output_socket, material_node const& input_node, int input_socket)
{
    exec({
        fmt::format("{}.node_tree.links.new({}.outputs[{}], {}.inputs[{}])"
            , get_full_name()
            , output_node.get_full_name()
            , output_socket
            , input_node.get_full_name()
            , input_socket
        )
    });
}

