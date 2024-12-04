#ifndef XIN_PICTURE_H
#define XIN_PICTURE_H

#include <iostream>
#include <string.h>

typedef unsigned int uint32;

class Picture
{
 public:
    Picture():
     width_(0),
     height_(0),
     data_(nullptr)
    {}

    Picture(uint32 w,uint32 h)
    {
        init(w,h);
    }

    ~Picture()
    {
        delete [] data_;
    }

    Picture(const char* const* pic,uint32 len)
    {
        uint32 maxWidth = 0;
        for(int i = 0; i < len; ++i)
        {
            maxWidth = max(maxWidth,strlen(pic[i]));
        }

        init(maxWidth,len);

        for(int row = 0; row < height_; ++row)
        {
            const char* src = pic[row];
            int col = 0;
            while(col < width_ ||  *src)
            {
                if(*src)
                    position(row,col) = *src++;
                // else
                //     position(row,col) = ' ';
                ++col;
            }
        }
    }

    Picture(const Picture& other):
     width_(other.width_),
     height_(other.height_),
     data_(new char[width_ * height_])
    {
        copy(0,0,other);
    }

    Picture& operator=(const Picture& other)
    {
        if(this != &other)
        {
            delete [] data_;
            init(other.width_,other.height_);
            copy(0,0,other);
        }
        return *this;
    }

    Picture& operator=(Picture&& other)
    {
        if(this != &other)
        {
            if(data_)
                delete [] data_;
            width_ = other.width_;
            height_ = other.height_;
            data_ = other.data_;
            other.data_ = nullptr;
            other.width_ = 0;
            other.height_ = 0;
        }
        return *this;
    }

    Picture(Picture&& other)
    {
        width_ = other.width_;
        height_ = other.height_;
        data_ = other.data_;
        other.data_ = nullptr;
        other.width_ = 0;
        other.height_ = 0;
    }

    uint32 width() const { return width_;}
    uint32 height() const { return height_;}
    Picture frame() const
    {
        Picture ret(width_ + 2,height_ + 2);
        ret.copy(1,1,*this);
        int retw = ret.width();
        int reth = ret.height();
        for(int i = 1; i < retw - 1; ++i)
        {
            ret.position(0,i) = '-';
            ret.position(reth-1,i) = '-';
        }

        for(int i = 1; i < reth - 1; ++i)
        {
            ret.position(i,0) = '|';
            ret.position(i,retw-1) = '|';
        }
        ret.position(0,0) = '+';
        ret.position(0,retw-1) = '+';
        ret.position(reth-1,0) = '+';
        ret.position(reth-1,retw-1) = '+';
        return ret;
    }

    char& position(uint32 r,uint32 c)
    {
        return data_[r * width_ + c];
    }
    char position(uint32 r,uint32 c) const
    {
        return data_[r * width_ + c];
    }

    void copy(uint32 x,uint32 y,const Picture& other)
    {
        int w = other.width();
        int h = other.height();

        for(int row = 0; row < h; ++row)
        {
            for(int col = 0; col < w; ++col)
            {
                position(x + row,y + col) = other.position(row,col);
            }
        }
    }

 protected:
    uint32 max(uint32 x,uint32 y)
    {
        return x > y ? x : y;
    }

    void init(uint32 w,uint32 h)
    {
        width_ = w;
        height_ = h;
        data_ = new char[w * h];
        memset(data_,' ',w*h);
    }
 private:
    uint32 width_;
    uint32 height_;
    char* data_;
};

std::ostream& operator<<(std::ostream& os,const Picture& pic)
{
    int w = pic.width();
    int h = pic.height();

    for(int i = 0; i < h; ++i)
    {
        for(int j = 0; j < w; ++j)
        {
            os << pic.position(i,j);
        }
        os << '\n';
    }
    return os;
}

Picture frame(const Picture& p)
{
    return p.frame();
}

Picture operator&(const Picture& left,const Picture& right)
{
    int height = left.height() + right.height();
    int width = left.width() > right.width() ? left.width() : right.width();

    Picture ret(width,height);
    ret.copy(0,0,left);
    ret.copy(left.height(),0,right);
    return ret;
}

Picture operator|(const Picture& left,const Picture& right)
{
    int width = left.width() + right.width();
    int height = left.height() > right.height() ? left.height() : right.height();

    Picture ret(width,height);
    ret.copy(0,0,left);
    ret.copy(0,left.width(),right);
    return ret;
}

#endif//!XIN_PICTURE_H