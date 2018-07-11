#pragma once

#include "FSprite.h"
#include "FTimeframe.h"
#include "FTimeDelegate.h"

class LightsweepSprite : public FSprite
{
public:
    LightsweepSprite();


    //=============================================================================================
    // Cocos2d-x node creation statics
public:
    static LightsweepSprite* CreateWithSpriteFrameName(const std::string& filename, const Vec2& position = Vec2(0, 0));
private:
    static LightsweepSprite* CreateWithSpriteFrame(SpriteFrame* spriteFrame);


    //=============================================================================================
    // General functoins
public:
    void SetInterval(float interval, bool immediate_swipe = true);
    void SetSwipesPerInterval(int8_t swipes);
    void SetTimeScale(float time_scale);
    void Start(bool immediate_swipe = false);
    void Stop();

private:
    void Swipe();

    
    //=============================================================================================
    // Delegate functoins
private:
    void OnSwipeFinished();
    void OnIntervalFinished();


    //=============================================================================================
    // Derived functions
protected: 
    virtual void OnInit() override;
    virtual void update(float dt) override;


    //=============================================================================================
    // Gerneral variables
private:
    FTimeDelegate delegate_;
    FTimeframe timeframe_;

    GLProgramState* glprogramstate_;
    GLProgram* glp;
    int32_t gl_lerp_loc_;

    float time_scale_;
    float interval_;
    int8_t swipes_per_interval_;
    int8_t current_interval_swipe_count_;
    bool bImmediateSwipe_ : 1;

    // COLOR BIT in one color32 calculator
};

FORCEINLINE void LightsweepSprite::SetInterval(float interval, bool immediate_swipe /* = true */) { interval_ = interval; bImmediateSwipe_ = immediate_swipe; }
FORCEINLINE void LightsweepSprite::SetSwipesPerInterval(int8_t swipes) { swipes_per_interval_ = swipes; }
FORCEINLINE void LightsweepSprite::SetTimeScale(float time_scale) { time_scale_ = time_scale; }
