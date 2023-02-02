#pragma once
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif

#include "GameObject.h"

namespace NCL {
    namespace CSC8503 {
        class PaintBallClass {
        public:
            PaintBallClass(int _maxAmmoInUse, int _maxAmmoHeld, int _fireRate, int _reloadTime, float _maxShootDist, GameWorld* world, ShaderBase* basicShader, MeshGeometry* sphereMesh, GameObject* cameraTargetObject);
            ~PaintBallClass();

            void Update(float dt);
            void UpdateTargetObject(GameObject* targetObject, GameWorld* world, Vector3 collisionAt);

        protected:
            // Weapon Stats
            int ammoInUse; // Current ammo to use
            int ammoHeld; // Total ammo currently held
            int maxAmmoHeld; // Max ammo allowed to be held
            int maxAmmoInUse; // Max ammo before reloading
            float fireRate; // time between shot
            float reloadTime; // time between reloads
            float maxShootDistance; // max dist can be shot

            void Shoot(float dt);
            void Reload(float dt);
            void PickUpAmmo(int amt);
            void CreateBullet();

            //Variables
            float shootTimer;
            float reloadTimer;
            GameWorld* m_World;
            ShaderBase* m_BasicShader;
            MeshGeometry* m_SphereMesh;
            GameObject* m_CameraTargetObject;
            GameObject* m_TargetObjet;
            Vector3 m_CollisionAt;

            enum statusTypes {
                isFiring,
                isIdle,
                isReloading
            };

            statusTypes status;

        private:
            Vector3 GetDirection(Vector3 fromPosition, Vector3 toPosition);
        };
    }
}
