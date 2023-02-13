#pragma once
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif

#include "ToonGameObject.h"

namespace NCL {
    namespace CSC8503 {
        class Team;
        class ToonGameWorld;
        class ToonLevelManager;
        class PaintBallClass {
        public:
            PaintBallClass();
            PaintBallClass(ToonGameWorld* gameWorld, ToonLevelManager* levelManager, int _maxAmmoInUse, int _maxAmmoHeld, float _fireRate, float _reloadTime, float _maxShootDist);
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
            Vector3 bulletVelocity;

            void Shoot(float dt);
            void Reload(float dt);
            void PickUpAmmo(int amt);
            void FireBullet();

            float GetYCoordinate(int x, int initialVelocity);
            Vector3 CalculateBulletVelocity(Vector3 target, Vector3 origin, float t);
            void DrawTrajectory(NCL::Maths::Vector3 force);   //Trajectory
            void HideTrajectory();              //Trajectory

            Team* team;
            ToonGameWorld* gameWorld;
            ToonLevelManager* levelManager;

            //Variables
            bool trajectoryDetected;
            float shootTimer;
            float reloadTimer;
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
