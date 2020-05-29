#pragma once
#include <utils/di_global.h>
#include <memory>
#include <string>
NAMESPACE_BEGIN(DR)
class Image
{
    public:
    int width_ = 0;
    int height_ = 0;
    int nChannels_ = 0;
    PicType type_ = PicType::kJPG; //default
    Image(){
        image_ = nullptr;
    }
    Image(const Image& ) = delete;
    Image(const void* data,int height,int width,PicType type,int channels = 3) // for write
    {
        this->image_ = data;
        this->type_ = type;
        this->height_ = height;
        this->width_ = width;
        this->nChannels_ = channels;
    }
    Image(Image&& other)
    {
        this->image_ = other.image_;
        other.image_ = nullptr;
        this->width_ = other.width_;
        this->height_ = other.height_;
        this->nChannels_ = other.nChannels_;
    }

    bool write_image(const std::string &path, bool flip = false);
    bool load_image(const std::string &path, bool flip = false);

  private:
    const void *image_;
};
NAMESPACE_END(DR)