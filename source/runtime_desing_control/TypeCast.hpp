#ifndef TypeCastH
#define TypeCastH

#include <assert.h>
namespace rdc
{

    // usage CheckPtrType<TYPE_TO_CHECK*>(ptr);
    template <typename ToPtr, typename From>
    inline bool    CheckPtrType(From* ptr)
    {
        return NULL != dynamic_cast<ToPtr>(ptr);
    }
    
    //usage DownCast<TYPE1*,TYPE2*>(ptr);
    template<typename ToPtr, typename From>     
    inline ToPtr DownCast(From f) {
        return dynamic_cast<ToPtr>(f);
    }

    //usage UpCast<TYPE1*,TYPE2*>(ptr);
    template <typename ToPtr, typename From>
    inline ToPtr    UpCast(From ptr)
    {
        return dynamic_cast<ToPtr>(ptr);
    }

    ////usage ForceCast<TYPE1*,TYPE2*>(ptr);
    template <typename ToPtr, typename From>
    inline ToPtr    ForceCast(From ptr)
    {
        return (ToPtr)(ptr);
    }
}//namespace
#endif;
