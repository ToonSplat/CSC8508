#include "ToonScreen.h"
#include "ToonAssetManager.h"

NCL::CSC8503::ToonScreen::ToonScreen(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld) : PaintableObject(RP3D_World, gameWorld)
{
	renderObject->SetShader(ToonAssetManager::Instance().GetShader("sceneScreen"));
	renderObject->SetDefaultTexture(ToonAssetManager::Instance().GetTexture("screenTex"));

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
	if (isMinimap) return;
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
