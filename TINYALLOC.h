#ifndef __TINYALLOC__
#define __TINYALLOC__

#include <iostream>
#include <new>
#include <cstddef>    //for ptrdiff_t, size_t
#include <climits>    //for UINT_MAX
#include <cstdlib>    //for exit()


namespace TinySTL
{
    //����::operator new�����ڴ�ռ�
    template<typename T>
    inline T* _allocate(size_t n, T*)
    {
        std::set_new_handler(0);
        n = n?1:n;                                    //��������_nΪ0������
        T* ptr = (T*)(::operator new(n*sizeof(T)));   //����::operator����ռ�
        if(ptr==0)                                    //���δ����ɹ����ӡ������Ϣ�������쳣
        {
            std::cerr<<"out of memory!";
            exit(1);
        }
        return ptr;
    }

    //����::operator delete�ͷ��ڴ�ռ�
    template <typename T>
    inline void _deallocate(T* p)
    {
        ::operator delete(p);
    }

    //Ϊ����Ŀռ���placement new�������ע��T1��T2Ϊ��ͬ����
    template <typename T1, typename T2>
    inline void _construct(T1* p, const T2& value)
    {
        new(p) T1(value);
    }

    //ͨ�������Լ������������ݻٶ���
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

        //ע��ڶ�������n
        void deallocate(pointer ptr, size_t n){ _deallocate(ptr); }
        //����ĳ������
        void construct(pointer ptr, const_reference value) { _construct(ptr, value); }
        //����ĳ������
        void destroy(pointer ptr)            { _destroy(ptr); }
        void destroy(pointer first, pointer last)
        {
            for(;first!=last;++first)
                first->~T();
        }
        //����ĳ������ĵ�ַ
        pointer address(reference x)         { return (pointer)(&x); }
        const_pointer const_address(const_reference x)     { return (const_pointer)(&x); }

        //���ؿɳɹ����õ����ռ�.�����������Ե�ʵ�ָú�����Ŀǰʵ������
        size_type max_size()const            { return size_type(UINT_MAX/sizeof(T)); }
    };
}

#endif
