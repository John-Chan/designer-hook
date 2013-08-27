#ifndef TypeCastH
#define TypeCastH

#include <assert.h>
namespace rdc
{

    /// usage CheckPtrType<TYPE*>(ptr);
    template <typename ToPtr, typename From>
    inline bool    CheckPtrType(From* ptr)
    {
        return NULL != dynamic_cast<ToPtr>(ptr);
    }
    template<typename To, typename From>
    inline To implicit_cast(From const &f) {
      return f;
    }
    
    //usage DownCast<TYPE*>(ptr);
    template<typename ToPtr, typename From>     // use like this: down_cast<T*>(foo);
    inline ToPtr DownCast(From f) {
        // so we only accept pointers
        // Ensures that To is a sub-type of From *.  This test is here only
        // for compile-time type checking, and has no overhead in an
        // optimized build at run-time, as it will be optimized away
        // completely.
        //if (false) {
        //    implicit_cast<From*, ToPtr>(0);
        //}

#if defined(_DEBUG)
      //assert(f == NULL || dynamic_cast<ToPtr>(f) != NULL);  // RTTI: debug mode only!
#endif
        //return static_cast<ToPtr>(f);
        return (ToPtr)f;
    }
    //usage UpCast<TYPE*>(ptr);
    template <typename ToPtr, typename From>
    inline ToPtr    UpCast(From* ptr)
    {
        return dynamic_cast<ToPtr>(ptr);
    }

    //usage UpCast<TYPE*>(ptr);
    template <typename ToPtr, typename From>
    inline ToPtr    ForceCast(From ptr)
    {
        return (ToPtr)(ptr);
    }
}//namespace
#endif;
