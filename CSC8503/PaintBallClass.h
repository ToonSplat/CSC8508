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
            PaintBallClass();
            ~PaintBallClass();

            void Update (float dt);

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
            void PickUpAmmo();


            //Variables
            float shootTimer;
            float reloadTimer;

            enum statusTypes {
                isFiring,
                isIdle,
                isReloading
            };

            statusTypes status;
        };
    }
}
