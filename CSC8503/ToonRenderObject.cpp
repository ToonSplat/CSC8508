#include "ToonRenderObject.h"
#include "ToonTransform.h"
#include "MeshGeometry.h"

using namespace NCL::CSC8503;
using namespace NCL;

ToonRenderObject::ToonRenderObject(ToonTransform* parentTransform, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader, MeshGeometry* minimapMesh) {
	this->transform	= parentTransform;
	this->mesh		= mesh;
	this->texture	= tex;
	this->shader	= shader;
	this->colour	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	this->minimapMesh = minimapMesh;
}

NCL::CSC8503::ToonRenderObject::ToonRenderObject(MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader)
{
	this->transform = nullptr;
	this->mesh = mesh;
	this->texture = tex;
	this->shader = shader;
	this->colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

ToonRenderObject::~ToonRenderObject() {

}