#include "LightsweepSprite.h"
#include "FrameworkScene.h"

LightsweepSprite::LightsweepSprite()
	:gl_lerp_loc_(-1)
	,time_scale_(1.f)
	,interval_(0)
	,swipes_per_interval_(1)
	,current_interval_swipe_count_(0)
{

}

void LightsweepSprite::OnInit()
{
    glp = new GLProgram();
    glp->autorelease();

    if (!glp->initWithFilenames("lightsweep.vsh", "lightsweep.fsh"))
    {
        DEBUG_BREAK;
    }

    glp->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    glp->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
    glp->link();
    glp->updateUniforms();
    setGLProgram(glp);

    auto rect = getTextureRect();
    auto sprite_sheet_pos = rect.origin;
    auto texture_size = rect.size;
    auto sprite_sheet_texture = getTexture();
    auto tex_width = sprite_sheet_texture->getPixelsWide();
    auto tex_height = sprite_sheet_texture->getPixelsHigh();

    float pos_x = sprite_sheet_pos.x / tex_width;
    float pos_y = sprite_sheet_pos.y / tex_height;
    float width = texture_size.width / tex_width;
    float height = texture_size.height / tex_height;

    float x1 = pos_x;
    float x2 = pos_x + width;
    float y1 = pos_y;
    float y2 = pos_y + height;

    glprogramstate_ = GLProgramState::getOrCreateWithGLProgram(glp);
    setGLProgramState(glprogramstate_);

    auto loc = glGetUniformLocation(glp->getProgram(), "sprite_sheet_v1");
    glprogramstate_->setUniformVec2(loc, Vec2(x1, y1));

    loc = glGetUniformLocation(glp->getProgram(), "sprite_sheet_v2");
    glprogramstate_->setUniformVec2(loc, Vec2(x2, y2));

    gl_lerp_loc_ = glGetUniformLocation(glp->getProgram(), "lerp_val");
    glprogramstate_->setUniformFloat(gl_lerp_loc_, 0.f);

    //auto __gl_error_code = glGetError();
    //       if (__gl_error_code != GL_NO_ERROR)
    //       {
    //           CCLOG("error: 0x%x  : ", (int)__gl_error_code);
    //       }



	FrameworkScene::GetActiveScene()->AddTickable(timeframe_);
	FrameworkScene::GetActiveScene()->AddTickable(delegate_);
}

LightsweepSprite* LightsweepSprite::CreateWithSpriteFrameName(const std::string& filename, const Vec2& position /* = Vec2(0, 0)*/)
{
	SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);

#if COCOS2D_DEBUG > 0
	char msg[256] = { 0 };
	sprintf(msg, "Invalid spriteFrameName: %s", filename.c_str());
	CCASSERT(frame != nullptr, msg);
#endif
	auto sprite = CreateWithSpriteFrame(frame);
	sprite->setAnchorPoint(Vec2(0.5, 0.5));
	sprite->setPosition(position);

	return sprite;
}

//TODO TEMPLATE THIS
LightsweepSprite* LightsweepSprite::CreateWithSpriteFrame(SpriteFrame* spriteFrame)
{
	LightsweepSprite* sprite = new (std::nothrow) LightsweepSprite();
	if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	{
		sprite->OnInit();
		sprite->autorelease();
		return sprite;
	}
	// sprite->setAnchorPoint(Vec2(0.5, 0.5));
	// sprite->setPosition(position);

	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void LightsweepSprite::update(float dt)
{
	FSprite::update(dt);

	if (timeframe_.IsRunning())
	{
		glprogramstate_->setUniformFloat(gl_lerp_loc_, timeframe_.GetCurrentTime());
	}

}

void LightsweepSprite::Start(bool immediate_swipe /* = false */)
{
	current_interval_swipe_count_ = 0;

	if (interval_ > 0.f)
	{
		delegate_.Start(interval_, std::bind(&LightsweepSprite::OnIntervalFinished, this));
	}

	if (bImneduateSwipe_ || immediate_swipe)
	{
		Swipe();
	}
}

void LightsweepSprite::Swipe()
{
	timeframe_.SetTimeScale(time_scale_);
	timeframe_.Start(1.f, std::bind(&LightsweepSprite::OnSwipeFinished, this));

	current_interval_swipe_count_++;
}

void LightsweepSprite::OnSwipeFinished()
{ 
	if(current_interval_swipe_count_ < swipes_per_interval_)
	{
		Swipe();
	}
}

void LightsweepSprite::OnIntervalFinished()
{
	Start(true);
}