/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#include <iostream>
#include <student/gpu.hpp>

#include <string.h>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <cstring>

#include <glm/gtc/matrix_transform.hpp>

#include <student/gpu.hpp>
#include <framework/framebuffer.hpp>
#include <tests/testCommon.hpp>

struct Triangle{
      OutVertex points[3];
    };
float area(glm::vec2 const&A,glm::vec2 const&B,glm::vec2 const&C){
  auto a=glm::length(B-A);
  auto b=glm::length(C-B);
  auto c=glm::length(A-C);
  auto s = (a+b+c)/2.f;
  return glm::sqrt(s*glm::abs(s-a)*glm::abs(s-b)*glm::abs(s-c));
}
glm::vec3 barycentrics(OutVertex const&a,OutVertex const&b,OutVertex const&c,glm::vec2 x,uint32_t w,uint32_t h){
  glm::vec4 shrT[3]={a.gl_Position,b.gl_Position,c.gl_Position};
  float T = area(shrT[0],shrT[1],shrT[2]);
  glm::vec3 l;
  for(int i=0;i<3;++i)
      l[i] = area(shrT[(i+1)%3],shrT[(i+2)%3],x)/T;
  return l;
}
glm::vec3 barycenticPerspective(glm::vec3 l,OutVertex const&a,OutVertex const&b,OutVertex const&c,glm::vec2 x,uint32_t w,uint32_t h)
{
    
  
  glm::vec4 shrT[3]={a.gl_Position,b.gl_Position,c.gl_Position};
  for(int i=0;i<3;++i){
        l[i] /= shrT[i].w;
  }
  l /= glm::dot(l,glm::vec3(1.f));
  return l;


}
void assignData(VertexArray const&vao,InVertex * invertex)
{
        for (int l = 0; l<maxAttributes;l++){
            if (vao.vertexAttrib[l].bufferData){
                switch (vao.vertexAttrib[l].type){
                    case AttributeType::FLOAT:
                        invertex->attributes[l].v1 = *(float*)(((uint8_t*)vao.vertexAttrib[l].bufferData)+
                                                           vao.vertexAttrib[l].stride*invertex->gl_VertexID+
                                                                 vao.vertexAttrib[l].offset);
                        break;
                    case AttributeType::VEC2:
                        invertex->attributes[l].v2 = *(glm::vec2*)(((uint8_t*)vao.vertexAttrib[l].bufferData)+
                                                             vao.vertexAttrib[l].stride*invertex->gl_VertexID+
                                                                 vao.vertexAttrib[l].offset);
                        break;
                    case AttributeType::VEC3:
                        invertex->attributes[l].v3 = *(glm::vec3*)(((uint8_t*)vao.vertexAttrib[l].bufferData)+
                                                             vao.vertexAttrib[l].stride*invertex->gl_VertexID+
                                                                 vao.vertexAttrib[l].offset);
                        break;
                    case AttributeType::VEC4 :
                       invertex->attributes[l].v4 = *(glm::vec4*)(((uint8_t*)vao.vertexAttrib[l].bufferData)+
                                                            vao.vertexAttrib[l].stride*invertex->gl_VertexID+
                                                                vao.vertexAttrib[l].offset);
                       break;

                    default :
                        break;
                }
            }
        }
}

uint32_t computeVertexID(VertexArray const&vao, uint32_t shaderInvocation)
{
    if (!vao.indexBuffer)
        return shaderInvocation;
     if (vao.indexType == IndexType::UINT32){
        uint32_t * ind = (uint32_t*)vao.indexBuffer;
        return ind [shaderInvocation];
    }else if (vao.indexType == IndexType::UINT16){
        uint16_t * ind = (uint16_t*)vao.indexBuffer;
        return ind[shaderInvocation];
    }else{
        uint8_t * ind = (uint8_t*)vao.indexBuffer;
        return ind[shaderInvocation];
    }
}
void loadVertex(InVertex *  invertex,VertexArray const&vao,uint32_t shaderInvocation)
{
   invertex->gl_VertexID =  computeVertexID(vao,shaderInvocation); 
   assignData(vao,invertex);
}

void loadTriangle(Triangle&triangle,Program const&prg,VertexArray const&vao,uint32_t tId){
      for(int i = tId * 2;i < tId*2+3;i++){ // smyčka přes vrcholy trojúhelníku
        InVertex inVertex;
        loadVertex(&inVertex,vao,tId+i);
        prg.vertexShader(triangle.points[i -2*tId],inVertex,prg.uniforms);
      }
    }
void runPerspectiveDivision(Triangle&triangle)
{
    for (int i = 0; i < 3; i++){
        float temp = triangle.points[i].gl_Position.w;
        triangle.points[i].gl_Position /= triangle.points[i].gl_Position.w;
        triangle.points[i].gl_Position.w =temp;
    }
}
void runViewportTransformation(Triangle&triangle,Frame&frm)
{
    auto res = glm::uvec2 (frm.width,frm.height);
    for(int i=0;i<3;++i)
        triangle.points[i].gl_Position  = glm::vec4((glm::vec2(triangle.points[i].gl_Position)*.5f+.5f)*glm::vec2(res),triangle.points[i].gl_Position.z,triangle.points[i].gl_Position.w);
}
bool isInside(glm::vec3 a,glm::vec3 c)
{
    return ((c.x * a.x) +   (c.y * a.y) + (c.z * a.z) >=0);
}
void rasterize (Frame&frame,Triangle const&triangle,Program const&prg)
{
    glm::uvec2 res = glm::uvec2(frame.width,frame.height); 
    glm::vec2 E[3];
    for(int i=0;i<3;++i){
        E[i]=triangle.points[(i+1)%3].gl_Position-triangle.points[i].gl_Position;
    }
    glm::vec2 N[3];for(int i=0;i<3;++i)N[i]=glm::vec2(-E[i].y,E[i].x);
    glm::vec3 p[3];for(int i=0;i<3;++i)p[i]=glm::vec3(N[i],-dot(N[i],(glm::vec2)triangle.points[i].gl_Position));
    glm::vec4 border = glm::ivec4(res,0,0);
    for (int i=0;i<3;i++){
        border.x=glm::min(border.x,triangle.points[i].gl_Position.x);
        border.y=glm::min(border.y,triangle.points[i].gl_Position.y);
        border.z=glm::max(border.z,triangle.points[i].gl_Position.x);
        border.w=glm::max(border.w,triangle.points[i].gl_Position.y);
    }
    
  border += glm::vec4(-1,-1,+1,+1);
  border = glm::clamp(border,glm::vec4(0,0,0,0),glm::vec4(res,res));
    auto bb = glm::uvec4(border);
    for(uint32_t y=bb.y;y<bb.w;++y)
        for(uint32_t x=bb.x;x<bb.z;++x){
          auto point = glm::vec3(glm::vec2(x,y)+0.5f,1.f);
          bool inside = true;


          for(int i=0;i<3;++i)inside &= (isInside(p[i],point));
          if(inside){
            InFragment inFragment;
            inFragment.gl_FragCoord.x = x+0.5f;
            inFragment.gl_FragCoord.y = y+0.5f;
            
            auto l = barycentrics(triangle.points[0],triangle.points[1],triangle.points[2],inFragment.gl_FragCoord,frame.width,frame.height);
            inFragment.gl_FragCoord.z = triangle.points[0].gl_Position.z * l[0] + triangle.points[1].gl_Position.z * l[1] + triangle.points[2].gl_Position.z * l[2];
            l = barycenticPerspective(l,triangle.points[0],triangle.points[1],triangle.points[2],inFragment.gl_FragCoord,frame.width,frame.height);
; 
            inFragment.attributes[0].v3 = 
                triangle.points[0].attributes[0].v3 * l[0]+
                triangle.points[1].attributes[0].v3 * l[1]+
                triangle.points[2].attributes[0].v3 * l[2];


            OutFragment outFragment;
            


            prg.fragmentShader(outFragment,inFragment,prg.uniforms);
            if (frame.depth[y+0.5*frame.width+x+0.5] > inFragment.gl_FragCoord.z){
                frame.color [4 * (x + frame.width*y) + 0] = 255;
                frame.color [4 * (x + frame.width*y) + 1] = 255;        
                frame.color [4 * (x + frame.width*y) + 2] = 255;
                frame.color [4 * (x + frame.width*y) + 3] = 255;
                frame.depth [y*frame.width +x] = inFragment.gl_FragCoord.z;
            }else{
                frame.color [4 * (x + frame.width*y) + 0] = 0;
                frame.color [4 * (x + frame.width*y) + 1] = 0;        
                frame.color [4 * (x + frame.width*y) + 2] = 0;
                frame.color [4 * (x + frame.width*y) + 3] = 0;


            }
           }
        }

        

}
//! [drawTrianglesImpl]
void drawTrianglesImpl(GPUContext &ctx,uint32_t nofVertices)
{
  (void)ctx;
  (void)nofVertices;

    for (uint32_t t = 0;t < nofVertices/3;t++){
        Triangle triangle;
        loadTriangle(triangle,ctx.prg,ctx.vao,t);
        runPerspectiveDivision(triangle);
        runViewportTransformation(triangle,ctx.frame);
        rasterize(ctx.frame,triangle,ctx.prg);
    } 
    
  /// \todo Tato funkce vykreslí trojúhelníky podle daného nastavení.<br>
  /// ctx obsahuje aktuální stav grafické karty.
  /// Parametr "nofVertices" obsahuje počet vrcholů, který by se měl vykreslit (3 pro jeden trojúhelník).<br>
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
}
//! [drawTrianglesImpl]

/**
 * @brief This function reads color from texture.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_texture(Texture const&texture,glm::vec2 uv){
  if(!texture.data)return glm::vec4(0.f);
  auto uv1 = glm::fract(uv);
  auto uv2 = uv1*glm::vec2(texture.width-1,texture.height-1)+0.5f;
  auto pix = glm::uvec2(uv2);
  //auto t   = glm::fract(uv2);
  glm::vec4 color = glm::vec4(0.f,0.f,0.f,1.f);
  for(uint32_t c=0;c<texture.channels;++c)
    color[c] = texture.data[(pix.y*texture.width+pix.x)*texture.channels+c]/255.f;
  return color;
}

/**
 * @brief This function clears framebuffer.
 *
 * @param ctx GPUContext
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void clear(GPUContext&ctx,float r,float g,float b,float a){
  auto&frame = ctx.frame;
  auto const nofPixels = frame.width * frame.height;
  for(size_t i=0;i<nofPixels;++i){
    frame.depth[i] = 10e10f;
    frame.color[i*4+0] = static_cast<uint8_t>(glm::min(r*255.f,255.f));
    frame.color[i*4+1] = static_cast<uint8_t>(glm::min(g*255.f,255.f));
    frame.color[i*4+2] = static_cast<uint8_t>(glm::min(b*255.f,255.f));
    frame.color[i*4+3] = static_cast<uint8_t>(glm::min(a*255.f,255.f));
  }
}
