#version 450 core

out vec3 FragColor;
in vec2 outTexC;

uniform sampler2D image;
uniform bool horizontal;

float weight[5] = float[] (0.2270270270f, 0.1945945946f, 0.1216216216f, 0.0540540541f, 0.0162162162f);

void main()
{             
     vec2 tex_offset = 1.0f / textureSize(image, 0.0f); // gets size of single texel
     vec3 result = texture(image, outTexC).rgb * weight[0];
     if(horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(image, outTexC + vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
            result += texture(image, outTexC - vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += texture(image, outTexC + vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
             result += texture(image, outTexC - vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
         }
     }
     FragColor = vec3(result);
}