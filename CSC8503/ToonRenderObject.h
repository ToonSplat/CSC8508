#pragma once
#include "TextureBase.h"
#include "ShaderBase.h"
#include "Vector4.h"
#include "MeshMaterial.h"
#include "ToonMeshMaterial.h"
#include <reactphysics3d/reactphysics3d.h>

namespace NCL {
	using namespace NCL::Rendering;

	class MeshGeometry;
	namespace CSC8503 {
		class ToonTransform;
		class ToonGameObject;
		using namespace Maths;

		class ToonRenderObject
		{
		public:
			ToonRenderObject(ToonTransform* parentTransform, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader, MeshGeometry* minimapMesh = nullptr);
			ToonRenderObject(ToonTransform* parentTransform, MeshGeometry* mesh, ToonMeshMaterial* mat, ShaderBase* shader, MeshGeometry* minimapMesh = nullptr);
			ToonRenderObject(MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader);
			~ToonRenderObject();

			void SetDefaultTexture(TextureBase* t) {
				texture = t;
			}

			TextureBase* GetDefaultTexture() const {
				return texture;
			}

			MeshGeometry*	GetMesh() const {
				return mesh;
			}
			MeshGeometry* GetMinimapMesh() const {
				return minimapMesh;
			}

			void SetMesh(MeshGeometry* mesh)		{ this->mesh = mesh; }
			void SetMinimapMesh(MeshGeometry* mesh) { this->minimapMesh = mesh; }

			ToonTransform*		GetTransform() const {
				return transform;
			}

			ShaderBase*		GetShader() const {
				return shader;
			}

			void SetColour(const Vector4& c) {
				colour = c;
			}

			Vector4 GetColour() const {
				return colour;
			}

			void SetMinimapColour(const Vector4& c) { minimapColour = c; }
			Vector4 GetMinimapColour() const { return minimapColour; }

			void SetGameObject(ToonGameObject* gameObject) { this->gameObject = gameObject; }

			ToonGameObject* GetGameObject() const {
				return gameObject;
			}

			ToonMeshMaterial* GetMaterial() const { return material; }			

		protected:
			ToonGameObject* gameObject;
			MeshGeometry*	mesh;
			MeshGeometry*	minimapMesh;
			TextureBase*	texture;
			ShaderBase*		shader;
			ToonTransform*	transform;
			Vector4			colour;
			Vector4			minimapColour;

			ToonMeshMaterial*	material;
			std::vector<const Rendering::TextureBase*> mTexturesDiffuse;
			std::vector<const Rendering::TextureBase*> mTexturesBump;
		};
	}
}
