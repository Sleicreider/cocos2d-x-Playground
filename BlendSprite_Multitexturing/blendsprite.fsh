varying vec2 v_texCoord;

uniform vec2 tex_1_offset;
uniform float blend_val;

void main()
{
	vec4 tex1 = texture2D(CC_Texture0, v_texCoord);
	vec4 tex2 = texture2D(CC_Texture0, v_texCoord + tex_1_offset);

	gl_FragColor = tex1 * (1. - blend_val) + tex2 * blend_val;
}