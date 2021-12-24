#include <model.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

using namespace glm;
using namespace std;

const aiScene *assimpLoad(const string &path)
{
    Assimp::Importer importer;

    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_COLORS | aiComponent_BONEWEIGHTS |
                                                            aiComponent_ANIMATIONS | aiComponent_LIGHTS | aiComponent_CAMERAS);

    return importer.ReadFile(path,
                             aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices |
                                 aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices |
                                 aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials |
                                 aiProcess_FixInfacingNormals | aiProcess_GenUVCoords | aiProcess_TransformUVCoords |
                                 aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FlipUVs | aiProcess_RemoveComponent);
}

void Model::loadModel(const string &path)
{
    const aiScene *scene = assimpLoad(path);

    dir = path.substr(0, path.find_last_of('/'));

    loadMeshes(scene);
}

void Model::loadMeshes(const aiScene *scene)
{
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        meshes.emplace_back();
        loadMesh(scene->mMeshes[i], meshes.back(), scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]);
    }
}

void Model::loadMesh(aiMesh *mesh, Mesh &myMesh, aiMaterial *material)
{
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex t;
        t.pos = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        t.normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->HasTextureCoords(0))
        {
            t.texCoords = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            t.tan = vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            t.biTan = vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }
        else
        {
            t.texCoords = vec2(0.0f, 0.0f);
            vec3 aux;
            if (t.normal.y != 0.0f || t.normal.z != 0.0f)
                aux = vec3(0.0f, 1.0f, 0.0f);
            else
                aux = vec3(1.0f, 0.0f, 0.0f);
            t.tan = normalize(cross(aux, t.normal));
            t.biTan = normalize(cross(t.tan, t.normal));
        }
        myMesh.push_back(t);
    }
    
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            myMesh.push_back(face.mIndices[j]);
    }

    loadTexFromMat(material, myMesh, aiTextureType_DIFFUSE, DIFFUSE_T);
    loadTexFromMat(material, myMesh, aiTextureType_SPECULAR, SPECULAR_T);
    loadTexFromMat(material, myMesh, aiTextureType_HEIGHT, NORMAL_T);
    loadTexFromMat(material, myMesh, aiTextureType_AMBIENT, HEIGHT_T);
}

void Model::loadTexFromMat(aiMaterial *mat, Mesh &myMesh, aiTextureType type, TextureType myType)
{
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        string name = string(str.C_Str());
        if (texTable.contains(name))
            myMesh.push_back(texTable[name]);
        else
        {
            Texture tex(loadTexFromFile(name, this->dir), myType, name);
            myMesh.push_back(tex);
            texTable[name] = tex;
        }
    }
}