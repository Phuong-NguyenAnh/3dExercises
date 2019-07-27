uniform mat4 uTransform;
attribute vec3 aPosition;
attribute vec2 aTexCoord;
varying vec2 vTexCoord;
void main()
{
	vTexCoord = aTexCoord;
	gl_Position = uTransform * vec4(aPosition, 1.0);
}