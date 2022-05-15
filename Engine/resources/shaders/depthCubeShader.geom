#version 450 core
layout (triangles) in;						   // Receive triangles
layout (triangle_strip, max_vertices=18) out;  // 6 Triangles

uniform mat4 shadowMatrices[6][1];

out vec4 FragPos;	// Send to FS

void main()
{
	// Build each face of the cubeDepthMap and transform to light-space
	for (int face = 0; face < 6; face++)
	{
		// Specify wich face we render (only for cube maps)
		gl_Layer = face;

		for (int i = 0; i < 3; i++)
		{
			FragPos = gl_in[i].gl_Position;

			// Transform to light_space
			gl_Position = shadowMatrices[face][0] * FragPos;

			EmitVertex();
		}
		
		EndPrimitive();
	}
}