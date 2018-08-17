#include "BlendSprite.h"
#include "FrameworkScene.h"

const char* BlendSprite::GLSL_BLEND_VAL = "blend_val";
const char* BlendSprite::GLSL_TEXTURE_OFFSET = "tex_1_offset";

BlendSprite::BlendSprite()
	:blend_value_(0.f)
{
}

BlendSprite* BlendSprite::CreateWithSpriteFrameName(const std::string& texture1, const std::string& textures2, const Vec2& position /* = Vec2(0, 0)*/)
{
	SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(texture1);

#if COCOS2D_DEBUG > 0
    char msg[256] = { 0 };
    sprintf(msg, "Invalid spriteFrameName: %s", texture1.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    auto sprite = CreateWithSpriteFrame(frame);
    sprite->setAnchorPoint(Vec2(0.5, 0.5));
    sprite->setPosition(position);

    return sprite;
}

// TODO TEMPLATE THIS
BlendSprite* BlendSprite::CreateWithSpriteFrame(SpriteFrame* spriteFrame)
{
	BlendSprite* sprite = new (std::nothrow) BlendSprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->OnInit();
        sprite->autorelease();
        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void BlendSprite::OnInit()
{
    auto glp = new GLProgram();
    glp->autorelease();

    if (!glp->initWithFilenames("defaultsprite.vsh", "blendsprite.fsh"))
    {
        DEBUG_BREAK;
    }

    glp->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    glp->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    glp->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
    glp->link();
    glp->updateUniforms();
    setGLProgram(glp);

    SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("star_yellow.png");

    auto rect = frame->getRect();
    auto sprite_sheet_pos = rect.origin;
    auto texture_size = rect.size;
    auto sprite_sheet_texture = frame->getTexture();
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

    rect = getTextureRect();
    sprite_sheet_pos = rect.origin;
    texture_size = rect.size;
    sprite_sheet_texture = getTexture();
    tex_width = sprite_sheet_texture->getPixelsWide();
    tex_height = sprite_sheet_texture->getPixelsHigh();

    pos_x = sprite_sheet_pos.x / tex_width;
    pos_y = sprite_sheet_pos.y / tex_height;
    width = texture_size.width / tex_width;
    height = texture_size.height / tex_height;

    float x1_1 = pos_x;
    float y1_1 = pos_y;

    glprogramstate_ = GLProgramState::getOrCreateWithGLProgram(glp);
    setGLProgramState(glprogramstate_);

    auto loc = glGetUniformLocation(glp->getProgram(), GLSL_TEXTURE_OFFSET);
    glprogramstate_->setUniformVec2(loc, Vec2(x1 - x1_1, y1 - y1_1));
    glprogramstate_->setUniformFloat(GLSL_BLEND_VAL, blend_value_);

	FrameworkScene::GetActiveScene()->AddTickable(timeframe_);
}

void BlendSprite::update(float delta)
{
    FSprite::update(delta);

    if (timeframe_.IsOrWasrunning())
    {
        // time from 0 to 1
        const auto t = timeframe_.GetCurrentTime() / timeframe_.GetEndTime();

        // interpolate
        auto value = (1.f - t) * anim_from_ + t * anim_to_;

        glprogramstate_->setUniformFloat(GLSL_BLEND_VAL, value);
    }
}