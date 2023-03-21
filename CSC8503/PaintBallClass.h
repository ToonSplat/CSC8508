#pragma once
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif

#include "ToonGameObject.h"

#define MAX_BULLETS 100

namespace NCL {
    namespace CSC8503 {
        class Team;
        class ToonGameWorld;
        class ToonLevelManager;
        class PaintBallClass {
        
        public:
            enum statusTypes {
                isFiring,
                isIdle,
                isReloading
            };

            PaintBallClass();
            PaintBallClass(ToonGameWorld* gameWorld, ToonLevelManager* levelManager, int _maxAmmoInUse, int _maxAmmoHeld, float _fireRate, float _reloadTime, float _maxShootDist);
            ~PaintBallClass();

            bool Update (float dt, PlayerControl* playerControls);
            void NPCUpdate (float dt);
            void FireBullet(reactphysics3d::Vector3 position, reactphysics3d::Vector3 orientation);

            void SetOwner(ToonGameObject* owner) { this->owningObject = owner; }
            void SetTeam(Team* team) { this->team = team; };

            float getFireRate() const { return 1 / fireRate; }
            float getShootTimer() const { return shootTimer; }
            float getShootSpread() const { return shootSpread; }

            void SetStatus(statusTypes newStatus) { status = newStatus; }

            PaintBallClass MakeInstance();           
            void UpdateTrajectory(float dt, PlayerControl* playerControls);
            void HideTrajectory();              //Trajectory
            std::string GetTeamName() { return team->GetTeamName(); }

        protected:
            // Weapon Stats
            int ammoInUse; // Current ammo to use
            int ammoHeld; // Total ammo currently held
            int maxAmmoHeld; // Max ammo allowed to be held
            int maxAmmoInUse; // Max ammo before reloading
            float fireRate; // time between shot
            float reloadTime; // time between reloads
            float maxShootDistance; // max dist can be shot
            PaintBallProjectile* bullet[MAX_BULLETS];    //Trajectory
            int trajectoryPoints = MAX_BULLETS;          //Trajectory
            Vector3 bulletVelocity;

            float shootSpread;
            float shootSpreadAdd;
            float shootSpreadMin;
            float shootSpreadMax;

            void Reload(float dt);
            void PickUpAmmo(int amt);

            float GetYCoordinate(int x, int initialVelocity);
            Vector3 CalculateBulletVelocity(Vector3 target, Vector3 origin, float t);
            void CalculateBulletPositionOrientation(const short& pitch, Vector3& positionFinal, Vector3& orientationFinal);

            Team* team;
            ToonGameWorld* gameWorld;
            ToonLevelManager* levelManager;

            //Variables
            bool trajectoryDetected;
            float shootTimer;
            float reloadTimer;

            ToonGameObject* owningObject;            

            statusTypes status;

            float m_ForceAppliedOnPaintBall = 400.0f;
        };
    }
}
