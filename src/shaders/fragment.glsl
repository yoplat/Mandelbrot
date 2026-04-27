#version 330 core

out vec4 FragColor;

uniform vec2 u_min;
uniform vec2 u_max;
uniform int max_iter;
uniform vec2 resolution;
uniform vec2 offset;
uniform vec2 zoom;

const vec3 palette[16] = vec3[16](
    vec3(66, 30, 15),
    vec3(25, 7, 26),
    vec3(9, 1, 47),
    vec3(4, 4, 73),
    vec3(0, 7, 100),
    vec3(12, 44, 138),
    vec3(24, 82, 177),
    vec3(57, 125, 209),
    vec3(134, 181, 229),
    vec3(211, 236, 248),
    vec3(241, 233, 191),
    vec3(248, 201, 95),
    vec3(255, 170, 0),
    vec3(204, 128, 0),
    vec3(153, 87, 0),
    vec3(106, 52, 3)
);

void main()
{
  vec2 center = (u_min + u_max) * 0.5 + offset;
  vec2 size = (u_max - u_min) * zoom;

  vec2 uv = (gl_FragCoord.xy + 0.5) / resolution;
  vec2 c = center + (uv - 0.5) * size;
  vec2 z = vec2(0.0);

  int iter = 0;
  while (iter < max_iter && dot(z, z) < 4.0) {
    z = vec2(
      z.x * z.x - z.y * z.y + c.x,
      2.0 * z.x * z.y + c.y
    );
    iter++;
  }

  if (iter == max_iter) {
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  } else {
    int index = iter % 16;
    vec3 color = palette[index] / 255.0;
    FragColor = vec4(color, 1.0);
  }
}
