#pragma once
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
            int ammo; // Current ammo to use
            float fireRate; // time between shot
            float reloadTime; // time between reloads
            int maxAmmoHeld; // Max ammo allowed to be held
            int maxAmmoInUse; // Max ammo before reloading
            float maxShootDistance; // max dist can be shot


            void Shoot(float dt);
            void Reload();
            void PickUpAmmo();


            //Variables
            float timer;

            enum status {
                isFiring,
                isIdle,
                isReloading
            };
        };
    }
}
