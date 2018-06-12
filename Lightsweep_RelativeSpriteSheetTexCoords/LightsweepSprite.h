#pragma once

#include "FSprite.h"
#include "FTimeframe.h"
#include "FTimeDelegate.h"

class LightsweepSprite : public FSprite
{
public:
	LightsweepSprite();

    void SetInterval(float interval, bool immediate_swipe = true);
    void SetSwipesPerInterval(int8_t swipes);
    void SetTimeScale(float time_scale);
	void Start(bool immediate_swipe = false);
	void Stop()
	{
		//stop in general  use bool running or something else?
		//Stop current delegate function
	}

	static LightsweepSprite* CreateWithSpriteFrameName(const std::string& filename, const Vec2& position = Vec2(0, 0));

private:
	static LightsweepSprite* CreateWithSpriteFrame(SpriteFrame* spriteFrame);

private:
	void Swipe();

	void OnSwipeFinished();
	void OnIntervalFinished();

protected: 
		virtual void OnInit() override;
		virtual void update(float dt) override;

private:
	int32_t gl_lerp_loc_;
    float time_scale_;
    float interval_;
    int8_t swipes_per_interval_;
	int8_t current_interval_swipe_count_;

	bool bImneduateSwipe_ : 1;

	FTimeframe timeframe_;
	FTimeDelegate delegate_;

	GLProgramState* glprogramstate_;
	GLProgram* glp;

    // COLOR BIT in one color32 calculator
};

FORCEINLINE void LightsweepSprite::SetInterval(float interval, bool immediate_swipe /* = true */) { interval_ = interval; bImneduateSwipe_ = immediate_swipe; }
FORCEINLINE void LightsweepSprite::SetSwipesPerInterval(int8_t swipes) { swipes_per_interval_ = swipes; }
FORCEINLINE void LightsweepSprite::SetTimeScale(float time_scale) { time_scale_ = time_scale; }