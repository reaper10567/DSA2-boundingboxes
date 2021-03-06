#version 330
in vec3 Position_b;
in vec3 UV_b;
in vec3 Normal_b;
in vec3 Binormal_b;
in vec3 Tangent_b;
in vec3 Color_b;

uniform mat4 MVP;
uniform mat4 mModelToWorld;
uniform vec3 CameraPosition;

out vec3 Normal_W;
out vec3 Tangent_W;
out vec3 Binormal_W;
out vec3 Position_W;
out vec3 Eye_W;
out vec2 UV;
out vec3 Color;

void main()
{
	gl_Position = MVP * vec4(Position_b, 1);
	
	UV = UV_b.xy;
	Color = Color_b;

	Position_W =	(mModelToWorld * vec4(Position_b,1)).xyz;
	Normal_W =		(mModelToWorld * vec4(Normal_b,0)).xyz;
	Tangent_W =		(mModelToWorld * vec4(Tangent_b, 0.0)).xyz;
	Binormal_W =	(mModelToWorld * vec4(Binormal_b, 0.0)).xyz;
	Eye_W = CameraPosition;

}