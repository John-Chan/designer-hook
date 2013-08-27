#ifndef RDC_SETTING_H__
#define RDC_SETTING_H__
#include <Graphics.hpp>

namespace rdc
{
    const TColor kGrabHandleColorActive =clRed;   
    const TColor kGrabHandleColorMutiSelect =clLime;
    const TColor kGrabHandleColorNormal = clBlack;

    static const int kGrabHandleSize=4;

    static const int kMoveUpStepByKey=4;
    static const int kMoveDownStepByKey=4;
    static const int kMoveLeftStepByKey=4;
    static const int kMoveRightStepByKey=4;

    
    static const int kHeightStepByKey=4;
    static const int kWidthStepByKey=4;
}//namespace
#endif
