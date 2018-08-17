Effect showcase: 


![Alt Text](https://puu.sh/BfKA1/f8b07e99e5.gif)




I mainly had this inspiration from the fameous multitexturing example often used from graphics programming beginner articles.
It's such an simple example and I often had the case in 2D games where you just want to fade/blend between 2 graphics.
The only issues of course is the texture atlas / sprite sheet again.

Currently this is limited to textures in the same texture atlas, though its simple to extend this to different spritesheet textures, and it is
also limited to the same texture size since this is the most common use case.

I mainly wanted to avoid having two sprite objects for just blending 2 textures even though they actually do the same and jsut visually appear diffently.
Additionally you can mix texture and have different blending states.


This is just a class derived from the cocos Sprite class:

const char* BlendSprite::GLSL_BLEND_VAL = "blend_val";
const char* BlendSprite::GLSL_TEXTURE_OFFSET = "tex_1_offset";

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

Here we need to get the spriteframe data from the second texture which we want to blend with the main CCSprite texture.
And again like in the lightsweep calculate its UV coord position in the sprite sheet texture which is used by the shader.
Though we only need the position and not the min max, since the size will be the same for both anyway.
	
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
    float y1 = pos_y;

	
Do the same for the main CCSprite texture.
	
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
    
This will calculate the offset from the second texture to the main CCSprite texture.  A different approach would be to check in 
the shader if the current fragment is the uv of the main texture and then take the fragment of the uv offset from the second texture,
whcih would be the approach for using different texture sprite sheets anyawy.

	glprogramstate_->setUniformVec2(loc, Vec2(x1 - x1_1, y1 - y1_1));
	
And finally send the blend value to the shader, which will be either 0.f for the first texture and 1.f for the second texture, and everything
between will be a mix from both.

    glprogramstate_->setUniformFloat(GLSL_BLEND_VAL, blend_value_);

	FrameworkScene::GetActiveScene()->AddTickable(timeframe_);
}

To animate it you can use any timer you have available and just update the blend value:

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


The vertex shader is just the default sprite shader as usual with just handing over the coords to the fragment shader.

Fragmentshader:

varying vec2 v_texCoord;

uniform vec2 tex_1_offset;
uniform float blend_val;

void main()
{

we only need CC_Texture0 since both textures are on the same spritesheet texture used by the shader, but we use the texture offset to 
tell it from where to take the texture info.

	vec4 tex1 = texture2D(CC_Texture0, v_texCoord);
	vec4 tex2 = texture2D(CC_Texture0, v_texCoord + tex_1_offset);

and finally just calculate the color contribution for the textures by our blend val where 0.f is tex1 and 1.f is tex2.
	
	gl_FragColor = tex1 * (1. - blend_val) + tex2 * blend_val;
}



For the effect itself:
I haven't done any performance tests how much more effecient it might be instead of having 2 textures sprite objects and use those for blending.
but the idea of it was just interesting enough for using it.




 
