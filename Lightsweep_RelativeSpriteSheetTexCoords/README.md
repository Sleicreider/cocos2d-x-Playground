Effect showcase: 


![Alt Text](https://puu.sh/ABhfJ/9f56d9289f.gif)



This is a extremely simple 2D effect which is widely used in mobile games to give elements a nice highlighting to catch the players eye.

This was just an attempt to do with only with shaders to avoid using a separate texture for the light sweep.

Additionally this example also provide a way to work with shaders on sprite sheets which are used a lot for 2d mobile games as an optimisation.
In the past there were often issues with engine and using shaders on sprite sheets since their texcoords will use their position the sprite sheet, which is often annoying when using shaders from someone else or when changing your spirtesheet texture positions.

I'm currently not sure if there is a better and more performant way out there, but the good thing it works as expected and you can just use it as your UV would start from 0 to 1 :)



This is just a class derived from the cocos Sprite class:

	glp = new GLProgram();

        glp->autorelease();

        if (!glp->initWithFilenames("lightsweep.vsh", "lightsweep.fsh"))
        {
            DEBUG_BREAK;
        }

        glp->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        glp->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        glp->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glp->link();
        glp->updateUniforms();

This is needed in order to create a new OpenGL program for our shaders .vsh and .fsh.
And cocos by default needs position colour and texcoords to work with our textures when creating custom shaders.

        auto rect = getTextureRect();
        auto sprite_sheet_pos = rect.origin;
        auto texture_size = rect.size;
        auto sprite_sheet_texture = getTexture();
        auto tex_width = sprite_sheet_texture->getPixelsWide();
        auto tex_height = sprite_sheet_texture->getPixelsHigh();

In order to calculate the relative texcoords postions we need the texture/spritesheet info.

Rect.origin will gibe us the textures position in the sprite sheet and rect.size the size.
But we also need the whole sprite sheet texture size with is stored in getTexture()->GetPixels....


        float pos_x = sprite_sheet_pos.x / tex_width;
        float pos_y = sprite_sheet_pos.y / tex_height;
        float width = texture_size.width / tex_width;
        float height = texture_size.height / tex_height;

Here we just calculate our texture info relative to the sprite sheet texture.
Since UV will also go from 0 to 1 the want to do the same.

        float x1 = pos_x;
        float x2 = pos_x + width;
        float y1 = pos_y;
        float y2 = pos_y + height;


Here just calculate our limits for the texture.
    
        glprogramstate_ = GLProgramState::getOrCreateWithGLProgram(glp);
        setGLProgramState(glprogramstate_);
    
        auto loc = glGetUniformLocation(glp->getProgram(), "sprite_sheet_v1");
        glprogramstate_->setUniformVec2(loc, Vec2(x1,y1));
    
        loc = glGetUniformLocation(glp->getProgram(), "sprite_sheet_v2");
        glprogramstate_->setUniformVec2(loc, Vec2(x2,y2));
    
		loc = glGetUniformLocation(glp->getProgram(), "lerp_val");
		glprogramstate_->setUniformFloat(loc, 0.f);

Send the data to our shader.
lerp_val should be a timer you want to control, which starts from 0 to 1.
You obviously have to set the uniform each tick though, but you don't need to get the location every time just store it somewhere.

    
        //__gl_error_code = glGetError();
        //       if (__gl_error_code != GL_NO_ERROR)
        //       {
        //           CCLOG("error: 0x%x  : ", (int)__gl_error_code);
        //       }

        setGLProgram(glp);

And don't forget to set the program.

The fragment shader:

https://www.shadertoy.com/view/XsGBzW this was my initial test and i moved from there.


`varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 sprite_sheet_v1;
uniform vec2 sprite_sheet_v2;
uniform float lerp_val;

float v1 = 0.4;
float v2 = 1.1;

float min_vis = 0.4;
float max_vis = 1.2;

void main()
{
    float lerp = (1.-lerp_val) * v1 + lerp_val*v2;
	float time = CC_Time.g;
    
	vec2 tex_coord = v_texCoord;
	
	vec2 uv;
	uv.x = (tex_coord.x - sprite_sheet_v1.x) / (sprite_sheet_v2.x - sprite_sheet_v1.x);
	uv.y = (tex_coord.y - sprite_sheet_v1.y) / (sprite_sheet_v2.y - sprite_sheet_v1.y);

	float color = sin(uv.y + uv.x * 3. - lerp*9.) * .9; //0.4 is start, 1.2 end, so 
	
	color *= color * color * 0.6;
    
	color = clamp(color, 0., 1.);
	
	vec4 texture_color = texture2D(CC_Texture0, tex_coord);
	color = clamp(color,0.,texture_color.a);
	
	gl_FragColor = vec4(vec3(texture_color.rgb) + vec3(color),texture_color.a);
}



The uniforms are Is just the data we've send through the glsetUniform function, just make sure the name is exactly the same.

	vec2 uv;
	uv.x = (tex_coord.x - sprite_sheet_v1.x) / (sprite_sheet_v2.x - sprite_sheet_v1.x);
	uv.y = (tex_coord.y - sprite_sheet_v1.y) / (sprite_sheet_v2.y - sprite_sheet_v1.y);

This will calculate the final relative UV for our texture.

This is just a simple (x - x1) / (x2 - x1) calculation used to map my coords from 0 to 1.
And this is all you need for the relative tex coords for your texture in a sprite Sheet for cocos, It would be nicer to control the texcoords sent directly like cocos does it if thats possible but this would require more changes in the engine, what I currently want to avoid.



For the effect itself:

    Since this is just a simple effect I've just used a sin function to calculate some "waves" which will simulate our line swiping over the texture, I haven't mathematically checked it and therefore used visible limits v1 and v2 which indicate start and end for the sin calc for the swipe to be visible., and I just use my timer to move this sin swipe, this could be changed of course, maybe I'll do it in the future, but for now it works fine :D.





 
