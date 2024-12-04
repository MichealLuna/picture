#ifndef XIN_PICTURE_H
#define XIN_PICTURE_H

#include <iostream>
#include <string.h>

class Picture;

void pad(std::ostream& os,int x,int y)
{
    for(int i = x; i < y; ++i)
        os << ' ';
}
class P_Node
{
    friend class Picture;
 protected:
    int width_;
    int height_;
    int useCount_;

    P_Node():
     width_(0),
     height_(0),
     useCount_(1)
    {}

    virtual ~P_Node() = default;
 public:
    int width() const { return width_; }
    int height() const { return height_; }
    virtual void display(std::ostream& os,int row,int wd) = 0;
};

class StringPic : public P_Node
{
 private:
    char** data_;
    int* datalen_;
    int size_;
 public:
    StringPic(const char* const* str,int len)
     :data_(nullptr),
     size_(len)
    {
        data_ = new char*[len];
        datalen_ = new int[len];
        width_ = 0;
        height_ = len;

        for(int i = 0; i < len; ++i)
        {
            int ilen = strlen(str[i]);
            datalen_[i] = ilen;
            data_[i] = new char[ilen + 1];
            strcpy(data_[i],str[i]);
            width_ = width_ > ilen ? width_ : ilen;
        }
    }

    void display(std::ostream& os,int row,int wd) override
    {
        if(row >= 0 && row < height_)
        {
            os << data_[row];
            pad(os,datalen_[row],wd);
        }else{
            pad(os,0,wd);
        }
    }

    ~StringPic()
    {
        for(int i = 0; i < height_; ++i)
        {
            delete[] data_[i];
        }
        delete [] data_;
        delete [] datalen_;
    }
};
class Picture
{
    friend std::ostream& operator<<(std::ostream& os,const Picture& pic);
    friend Picture frame(const Picture& pic);
    friend Picture operator &(const Picture& t,const Picture& b);
    friend Picture operator |(const Picture& l,const Picture& r);
 public:
    Picture(const char* const* str,int len):
     p(new StringPic(str,len))
    {
    }

    Picture(const Picture& pic):
     p(pic.p)
    {
        ++(p->useCount_);
    }

    Picture& operator=(const Picture& pic)
    {
        if(this != &pic)
        {
            ++(pic.p->useCount_);
            if(--(p->useCount_) == 0)
            {
                delete p;
            }
            p = pic.p;
        }
        return *this;
    }
    
    Picture():p(nullptr){}

    ~Picture()
    {
        if(--(p->useCount_) == 0) delete p;
    }

    int width() const { return p->width(); }
    int height() const { return p->height(); }
    void display(std::ostream& os,int row,int wd) const
    { 
        p->display(os,row,wd); 
    }
    
 private:
    Picture(P_Node* other):p(other){}

 private:
    P_Node* p;
};

class FramePic : public P_Node
{
 private:
    Picture pic_;
 public:
    FramePic(const Picture& pic):
     pic_(pic)
    {
        width_ = pic.width() + 2;
        height_ = pic.height() + 2;
    }

    void display(std::ostream& os,int row,int wd) override
    {
        //top or bottom
        if(row == 0 || row == height_ - 1)
        {
            os << '+';
            for(int i = 1; i < width_; ++i)
            {
                os << '-';
            }
            os << '+';
        }
        else if(row > 0 && row < height_)
        {
            os << '|';
            pic_.display(os,row - 1,wd - 1);
            os << '|';
        }else{
            pad(os,0,wd);
        }
    }
};

class VCatPic : public P_Node
{
 private:
    Picture top_;
    Picture bottom_;
 public:
    VCatPic(const Picture& top,const Picture& bottom):
     top_(top),
     bottom_(bottom)
    {
        width_ = top_.width() > bottom.width() ? top.width() : bottom.width();
        height_ = top_.height() + bottom.height();
    }

    void display(std::ostream& os,int row,int wd) override
    {
        if(row >= 0 && row < top_.height())
        {
            top_.display(os,row,wd);
        }
        else if(row >= top_.height() && row < height_)
        {
            //don't forget row is a gobal val of two cat picture,
            //it may greater than its height.
            bottom_.display(os,row - top_.height(),wd);
        }else{
            pad(os,0,wd);
        }
    }
};

class HCatPic : public P_Node
{
 private:
    Picture left_;
    Picture right_;
 public:
    HCatPic(const Picture& l,const Picture& r):
     left_(l),
     right_(r)
    {
        width_ = l.width() + r.width();
        height_ = l.height() > r.height() ? l.height() : r.height();
    }

    void display(std::ostream& os,int row,int wd) override
    {
        if(row >= 0 && row < height_)
        {
            left_.display(os,row,left_.width());
            right_.display(os,row,right_.width());
        }else{
            pad(os,0,wd);
        }
    }
};

/*
*   the type of const object can only call the const method!
*/
std::ostream& operator<<(std::ostream& os,const Picture& pic)
{
    int height = pic.height();
    int width  = pic.width();
    for(int i = 0; i < height; ++i)
    {
        pic.display(os,i,width);
        os << '\n';
    }
    return os;
}

Picture frame(const Picture& pic)
{
    return new FramePic(pic);
}

Picture operator &(const Picture& t,const Picture& b)
{
    return new VCatPic(t,b);
}

Picture operator |(const Picture& l,const Picture& r)
{
    return new HCatPic(l,r);
}


#endif//!XIN_PICTURE_H