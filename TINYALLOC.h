#ifndef __TINYALLOC__
#define __TINYALLOC__

#include <iostream>
#include <new>
#include <cstddef>    //for ptrdiff_t, size_t
#include <climits>    //for UINT_MAX
#include <cstdlib>    //for exit()


namespace TinySTL
{
    //利用::operator new分配内存空间
    template<typename T>
    inline T* _allocate(size_t n, T*)
    {
        std::set_new_handler(0);
        n = n?1:n;                                    //解决传入的_n为0的问题
        T* ptr = (T*)(::operator new(n*sizeof(T)));   //利用::operator分配空间
        if(ptr==0)                                    //如果未分配成功则打印错误信息并返回异常
        {
            std::cerr<<"out of memory!";
            exit(1);
        }
        return ptr;
    }

    //利用::operator delete释放内存空间
    template <typename T>
    inline void _deallocate(T* p)
    {
        ::operator delete(p);
    }

    //为分配的空间用placement new构造对象，注意T1与T2为不同类型
    template <typename T1, typename T2>
    inline void _construct(T1* p, const T2& value)
    {
        new(p) T1(value);
    }

    //通过调用自己的析构函数摧毁对象
    template <typename T>
    inline void _destroy(T* p)
    {
        //p->~p();
        p->~T();
    }

    template <typename T>
    class allocator
    {
    public:
        typedef T          value_type;
        typedef T*         pointer;
        typedef T&         reference;
        typedef const T*   const_pointer;
        typedef const T&   const_reference;
        typedef size_t     size_type;
        typedef ptrdiff_t  difference_type;

        template <typename U>
        struct rebind
        {
            typedef allocator<U> other;
        };

        //hint used for locality
        pointer allocate(size_t n, const void* hint=0)     { return _allocate(n, (pointer)0); }

        //注意第二个参数n
        void deallocate(pointer ptr, size_t n){ _deallocate(ptr); }
        //构造某个对象
        void construct(pointer ptr, const_reference value) { _construct(ptr, value); }
        //销毁某个对象
        void destroy(pointer ptr)            { _destroy(ptr); }
        void destroy(pointer first, pointer last)
        {
            for(;first!=last;++first)
                first->~T();
        }
        //返回某个对象的地址
        pointer address(reference x)         { return (pointer)(&x); }
        const_pointer const_address(const_reference x)     { return (const_pointer)(&x); }

        //返回可成功配置的最大空间.仅仅是象征性地实现该函数，目前实际意义
        size_type max_size()const            { return size_type(UINT_MAX/sizeof(T)); }
    };
}

#endif
