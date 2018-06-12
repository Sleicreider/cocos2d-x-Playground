varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D tex;
uniform vec4 SpriteSheetInfo;
uniform vec2 SpriteSheetSize;
uniform float lerp_val;

float v1 = 0.4;
float v2 = 1.1;





float min_vis = 0.4;
float max_vis = 1.2;
//float calc_lerp = lerp

void main()
{	
float pos_x = SpriteSheetInfo.x / SpriteSheetSize.x;
float pos_y = SpriteSheetInfo.y / SpriteSheetSize.y;
float width = SpriteSheetInfo.z / SpriteSheetSize.x;
float height = SpriteSheetInfo.w / SpriteSheetSize.y;

float x1 = pos_x;
float x2 = pos_x + width;
float y1 = pos_y;
float y2 = pos_y + width;

	float lerp = (1.-lerp_val) * v1 + lerp_val*v2;
	float time = CC_Time.g;
	vec2 tex_coord = v_texCoord;
	
	vec2 uv;
	
	uv.x = (tex_coord.x - x1) / (x2 - x1);
	uv.y = (tex_coord.y - y1) / (y2 - y1);

	
	float color = sin(uv.y + uv.x * 3. - lerp*9.) * .9; //0.4 is start, 1.2 end, so 
	
	color *= color * color * 0.6;
	
	color = clamp(color, 0., 1.);
	
	vec4 texture_color = texture2D(CC_Texture0, tex_coord);
	color = clamp(color,0.,texture_color.a);
	
	gl_FragColor = vec4(vec3(texture_color.rgb) + vec3(color),texture_color.a);
}