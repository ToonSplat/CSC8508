#pragma once
#include "PaintBallClass.h"

namespace NCL
{
    namespace CSC8503
    {
        class PaintBallClassNPC : public PaintBallClass
        {
        public:
            PaintBallClassNPC();
            ~PaintBallClassNPC() {};

            virtual void Update(float dt) override;
        };
    }
}