//用于判断地图工具需要响应的事件存在重叠时没有必要区分UP或者DOWN
//DOWN了必然会UP，UP之前必然会DOWN
#define MF_LBUTTON               ((uint64_t)0x01)
#define MF_MBUTTON               ((uint64_t)0x02)
#define MF_RBUTTON               ((uint64_t)0x04)
#define MF_MOUSEHOVER            ((uint64_t)0x08)
#define MF_MOUSEMOVE             ((uint64_t)0x10)
#define MF_MOUSEWHEEL            ((uint64_t)0x20)


#define MF_VK_ESCAPE             ((uint64_t)0x40)
#define MF_VK_BACK               ((uint64_t)0x80)
#define MF_VK_ENTER              ((uint64_t)0x100)
#define MF_VK_DELETE             ((uint64_t)0x200)
#define MF_VK_LEFT               ((uint64_t)0x400)
#define MF_VK_RIGHT              ((uint64_t)0x800)
#define MF_VK_UP                 ((uint64_t)0x1000)
#define MF_VK_DOWN               ((uint64_t)0x2000)


//以下为键盘按下释放消息，暂时没用上
#define MF_VK_BACK_DOWN          ((uint64_t)0x200)
#define MF_VK_BACK_UP            ((uint64_t)0x400)
#define MF_VK_TAB_DOWN           ((uint64_t)0x800)
#define MF_VK_TAB_UP             ((uint64_t)0x1000)
#define MF_VK_ENTER_DOWN         ((uint64_t)0x800)
#define MF_VK_ENTER_UP           ((uint64_t)0x1000)
#define MF_VK_SHIFT_DOWN         ((uint64_t)0x800)
#define MF_VK_SHIFT_UP           ((uint64_t)0x1000)
#define MF_VK_CONTROL_DOWN       ((uint64_t)0x800)
#define MF_VK_CONTROL_UP         ((uint64_t)0x1000)
#define MF_VK_ESCAPE_DOWN        ((uint64_t)0x800)
#define MF_VK_ESCAPE_UP          ((uint64_t)0x1000)
#define MF_VK_SPACE_DOWN         ((uint64_t)0x800)
#define MF_VK_SPACE_UP           ((uint64_t)0x1000)
#define MF_VK_DELETE_DOWN        ((uint64_t)0x800)
#define MF_VK_DELETE_UP          ((uint64_t)0x1000)
#define MF_VK_INSERT_DOWN        ((uint64_t)0x800)
#define MF_VK_INSERT_UP          ((uint64_t)0x1000)

#define MF_VK_LEFT_DOWN          ((uint64_t)0x800)
#define MF_VK_LEFT_UP            ((uint64_t)0x1000)
#define MF_VK_UP_DOWN            ((uint64_t)0x800)
#define MF_VK_UP_UP              ((uint64_t)0x1000)
#define MF_VK_RIGHT_DOWN         ((uint64_t)0x800)
#define MF_VK_RIGHT_UP           ((uint64_t)0x1000)
#define MF_VK_DOWN_DOWN          ((uint64_t)0x800)
#define MF_VK_DOWN_UP            ((uint64_t)0x1000)

#define MF_VK_PRIOR_DOWN         ((uint64_t)0x800)
#define MF_VK_PRIOR_UP           ((uint64_t)0x1000)
#define MF_VK_NEXT_DOWN          ((uint64_t)0x800)
#define MF_VK_NEXT_UP            ((uint64_t)0x1000)
#define MF_VK_END_DOWN           ((uint64_t)0x800)
#define MF_VK_END_UP             ((uint64_t)0x1000)
#define MF_VK_HOME_DOWN          ((uint64_t)0x800)
#define MF_VK_HOME_UP            ((uint64_t)0x1000)
