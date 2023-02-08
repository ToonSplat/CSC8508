#pragma once
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif

#include "ToonGameObject.h"

namespace NCL {
    namespace CSC8503 {
        class Team;
        class PaintBallClass {
        public:
            PaintBallClass();
            PaintBallClass(int _maxAmmoInUse, int _maxAmmoHeld, float _fireRate, float _reloadTime, float _maxShootDist, ShaderBase* basicShader, MeshGeometry* sphereMesh);
            ~PaintBallClass();

            void Update (float dt);

            void SetOwner(ToonGameObject* owner) { this->owningObject = owner; }
            void SetTeam(Team* team) { this->team = team; };

            PaintBallClass MakeInstance();

        protected:
            // Weapon Stats
            int ammoInUse; // Current ammo to use
            int ammoHeld; // Total ammo currently held
            int maxAmmoHeld; // Max ammo allowed to be held
            int maxAmmoInUse; // Max ammo before reloading
            float fireRate; // time between shot
            float reloadTime; // time between reloads
            float maxShootDistance; // max dist can be shot
            PaintBallProjectile* bullet[20];    //Trajectory
            int trajectoryPoints = 20;          //Trajectory

            void Shoot(float dt);
            void Reload(float dt);
            void PickUpAmmo(int amt);
            void FireBullet();

            float GetYCoordinate(int x, int initialVelocity);
            void DrawTrajectory(float force);   //Trajectory
            void HideTrajectory();              //Trajectory

            Team* team;

            //Variables
            float shootTimer;
            float reloadTimer;
            ShaderBase* m_BasicShader;
            MeshGeometry* m_SphereMesh;
            ToonGameObject* owningObject;

            enum statusTypes {
                isFiring,
                isIdle,
                isReloading
            };

            statusTypes status;
        };
    }
}
