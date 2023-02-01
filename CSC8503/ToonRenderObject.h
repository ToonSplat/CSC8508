#pragma once
#include "TextureBase.h"
#include "ShaderBase.h"
#include "Vector4.h"
#include <reactphysics3d/reactphysics3d.h>

namespace NCL {
	using namespace NCL::Rendering;

	class MeshGeometry;
	namespace CSC8503 {
		class ToonTransform;
		using namespace Maths;

		class ToonRenderObject
		{
		public:
			ToonRenderObject(ToonTransform* parentTransform, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader);
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

		protected:
			MeshGeometry*	mesh;
			TextureBase*	texture;
			ShaderBase*		shader;
			ToonTransform*	transform;			
			Vector4			colour;
		};
	}
}
