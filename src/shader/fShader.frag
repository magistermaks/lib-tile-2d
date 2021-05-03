#version 140
out vec4 LFragment;
void main() {
	LFragment = vec4( gl_FragCoord.x / 1000, 1.0, 1.0, 1.0 );
}