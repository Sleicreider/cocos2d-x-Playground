varying vec2 v_texCoord;

uniform vec2 sprite_sheet_v1;
uniform vec2 sprite_sheet_v2;
uniform float lerp_val;

float v1 = 0.4;
float v2 = 1.1;

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
