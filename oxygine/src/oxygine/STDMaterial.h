#pragma once
#include "oxygine-include.h"
#include "Material.h"
#include "STDRenderer.h"

namespace oxygine
{
    DECLARE_SMART(STDMaterial, spSTDMaterial);
    class STDMaterial : public Material
    {
    public:
        static STDMaterial* instance;

        STDMaterial() {}

        void render(ClipRectActor*,     const RenderState& rs) override;
        void render(MaskedSprite*,      const RenderState& rs) override;
        void doRender(Sprite*,          const RenderState& rs) override;
        void doRender(TextField*,       const RenderState& rs) override;
        void doRender(ColorRectSprite*, const RenderState& rs) override;
        void doRender(ProgressBar*,     const RenderState& rs) override;


    protected:
    };

}