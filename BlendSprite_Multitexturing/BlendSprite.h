#pragma once

#include "FSprite.h"
#include "FTimeframe.h"

/**
 * BlendSprite currently only supports textures with the same size and located in the same spritesheet.
 */
class BlendSprite : public FSprite
{
public:
	BlendSprite();

	static BlendSprite* CreateWithSpriteFrameName(const std::string& texture1, const std::string& texture2, const Vec2& position = Vec2(0, 0));

public:
	void SetBlendValue(float value);
	void StartFromTo(float time, float from, float to);

private: 
	static BlendSprite* CreateWithSpriteFrame(SpriteFrame* spriteFrame);

	virtual void OnInit() final;
	virtual void update(float delta) final;

private:
	static const char* GLSL_BLEND_VAL;
	static const char* GLSL_TEXTURE_OFFSET;

private:
	FTimeframe timeframe_;
    GLProgramState* glprogramstate_;

	float blend_value_;
	float anim_from_;
	float anim_to_;
};

FORCEINLINE void BlendSprite::SetBlendValue(float value) { blend_value_ = value; }

FORCEINLINE void BlendSprite::StartFromTo(float time, float from, float to)
{
    anim_from_ = from;
    anim_to_ = to;
    timeframe_.Start(time);
}