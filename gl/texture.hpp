#ifndef TEXTURE_HPP__
#define TEXTURE_HPP__

#include "global.hpp"
#include <string>

namespace GL
{
   class Framebuffer;

   struct StaticSampler;
   class Sampler : public ContextListener, public ContextResource
   {
      public:
         Sampler() { ContextListener::init(); }
         ~Sampler() { deinit(); }

         enum Type
         {
            PointWrap = 0,
            PointClamp,
            BilinearWrap,
            BilinearClamp,
            TrilinearWrap,
            TrilinearClamp,
            ShadowLinear,
            ShadowPoint,

            NumSamplers
         };

         void init(Type type);

         void reset() override;
         void destroyed() override;

         void bind(unsigned unit);
         void unbind(unsigned unit);

         static Sampler& get(Type type);
         static void bind(unsigned unit, Type type);
         static void unbind(unsigned unit, Type type);

      private:
         Type type = BilinearClamp;
         GLuint id = 0;

         GLenum type_to_wrap(Type type);
         GLenum type_to_compare(Type type);
         GLenum type_to_min(Type type);
         GLenum type_to_mag(Type type);

         static StaticSampler samplers[NumSamplers];
   };

   struct StaticSampler : Sampler
   {
      StaticSampler(Type type) { init(type); }
   };

   class Texture : public ContextListener, public ContextResource
   {
      public:
         Texture() { init(); }
         ~Texture() { deinit(); }

         enum Type
         {
            TextureNone = 0,
            Texture2D,
            Texture2DArray,
            TextureCube
         };

         struct Desc2D
         {
            Type type;
            unsigned levels;
            GLenum internal_format;
            unsigned width;
            unsigned height;
            unsigned array_size;
         };

         struct Resource
         {
            Type type;
            std::vector<std::string> paths;
            bool generate_mipmaps;
         };

         static unsigned size_to_miplevels(unsigned width, unsigned height);

         void init_2d(const Desc2D& desc);
         void load_texture_2d(const Resource& res);

         void bind(unsigned unit);
         void unbind(unsigned unit);

         void reset() override;
         void destroyed() override;

         const Desc2D& get_desc() const { return desc; }
         friend class Framebuffer;

      private:
         GLuint id = 0;
         GLenum texture_type = 0;

         Desc2D desc;
         Resource res;

         void setup();

         void load_texture_data();
         void upload_texture_data();

         GLenum type_to_gl(Type type);

         struct TextureData
         {
            std::vector<uint8_t> data;
            unsigned width;
            unsigned height;
         };
         std::vector<TextureData> data;
   };
}

#endif

