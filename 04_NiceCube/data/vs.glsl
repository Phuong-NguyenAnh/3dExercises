uniform mat4 uTransform;
attribute vec3 aPosition;
attribute vec3 aColor;
varying vec3 vColor;
void main()
{
	vColor = aColor;
	gl_Position = uTransform * vec4(aPosition, 1.0);
}