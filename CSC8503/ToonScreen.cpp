#include "ToonScreen.h"
#include "ToonAssetManager.h"

NCL::CSC8503::ToonScreen::ToonScreen(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld) : PaintableObject(RP3D_World, gameWorld)
{
	index = 0;
	maxIndex = 15;

	numberOfRows = 4;		//4x4 Texture

	updateTimer = 0.0f;
	updateTimerLimit = 0.1f;
}

NCL::CSC8503::ToonScreen::~ToonScreen()
{
}

void NCL::CSC8503::ToonScreen::Update(float dt)
{
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F5))
		renderObject->GetShader()->ReloadShader();

	updateTimer += dt;
	if (updateTimer >= updateTimerLimit)
	{
		index++;
		index %= maxIndex;

		updateTimer = 0.0f;
	}
}

void NCL::CSC8503::ToonScreen::Draw(OGLRenderer& r, bool isMinimap)
{
	if (!renderObject || !renderObject->GetMesh())
		return;

	if (isMinimap)
	{
		OGLMesh* minimapMesh = (OGLMesh*)renderObject->GetMinimapMesh();
		r.BindMesh(minimapMesh);

		for (int i = 0; i < (int)minimapMesh->GetSubMeshCount(); ++i)
			r.DrawBoundMesh(i);

		return;
	}

	OGLMesh* boundMesh = (OGLMesh*)renderObject->GetMesh();
	OGLShader* shader = r.GetBoundShader();
	if (shader == (OGLShader*)renderObject->GetShader())
	{
		int r = glGetUniformLocation(shader->GetProgramID(), "numberOfRows");
		glUniform1f(r, (float)numberOfRows);

		Vector2 texOffset = GetTextureOffset();
		int offsetLoc = glGetUniformLocation(shader->GetProgramID(), "offset");
		glUniform2fv(offsetLoc, 1, (float*)&texOffset);
	}

	r.BindMesh(boundMesh);
	for (int i = 0; i < (int)boundMesh->GetSubMeshCount(); ++i)
	{
		/*if (renderObject->GetMaterial() != nullptr && (int)renderObject->GetMaterial()->GetDiffuseTextures().size() > 0)
			r.BindTextureToShader((NCL::Rendering::OGLTexture*)renderObject->GetMaterial()->GetDiffuseTextures()[i], "mainTex", 0);*/

		if (renderObject->GetMaterial() != nullptr && (int)renderObject->GetMaterial()->GetSubMaterials().size() > 0) {
			int materialIndexLocations = glGetUniformLocation(r.GetBoundShader()->GetProgramID(), "materialIndex");
			int index = renderObject->GetMaterial()->GetMaterialIndex()[i];
			glUniform1i(materialIndexLocations, index);
		}

		r.DrawBoundMesh(i);
	}
}

Vector2 NCL::CSC8503::ToonScreen::GetTextureOffset()
{
	int column = index % numberOfRows;
	int row = index % numberOfRows;

	Vector2 texOffset;
	texOffset.x = (float)column / numberOfRows;
	texOffset.y = (float)row / numberOfRows;

	return texOffset;
}
