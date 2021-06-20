#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <locale.h>
#include <wctype.h>

static bool int_equals(int left, int right)
{
    return left == right;
}

ViWin*% ViWin_initialize(ViWin*% self, int y, int x, int width, int height, Vi* vi) version 8
{
    var result = inherit(self, y, x, width, height, vi);
    
    result.mark = borrow new map<wchar_t, tuple3<int,int, int>*%>.initialize();

    return result;
}

void ViWin_finalize(ViWin* self) version 5
{
    inherit(self);

    delete self.mark;

    if(self.returnPoint) {
        delete self.returnPoint;
    }
}

void ViWin_markAtCurrentPoint(ViWin* self, wchar_t c) 
{
    var point = borrow new tuple3<int, int, int>;

    point.v1 = self.scroll;
    point.v2 = self.cursorY;
    point.v3 = self.cursorX;
    
    self.mark.insert(c, point);
}

void ViWin_returnAtMarkedPoint(ViWin* self, wchar_t c) 
{
    var point = self.mark.at(c, null);
    
    if(point != null) {
        self.saveReturnPoint();

        self.scroll = point.v1;
        self.cursorY = point.v2;
        self.cursorX = point.v3;
        
        self.modifyUnderCursorYValue();
        self.modifyOverCursorYValue();
        self.modifyOverCursorXValue();
    }
}

void ViWin_returnBack(ViWin* self) 
{
    var point = borrow self.returnPoint;
    
    if(point != null) {
        int cursor_y = self.cursorY;
        int cursor_x = self.cursorX;
        int scroll = self.scroll;
        
        self.cursorY = point.v1;
        self.cursorX = point.v2;
        self.scroll = point.v3;
        
        self.modifyUnderCursorYValue();
        self.modifyOverCursorYValue();
        self.modifyOverCursorXValue();
        
        var return_point = borrow new tuple3<int,int,int>;

        return_point.v1 = cursor_y;
        return_point.v2 = cursor_x;
        return_point.v3 = scroll;

        self.returnPoint = return_point;
    }
}

void ViWin_returnBackOfStack(ViWin* self) 
{
    var point = self.returnPointStack.item(-1, null);
    
    if(point != null) {
        self.cursorY = point.v1;
        self.cursorX = point.v2;
        self.scroll = point.v3;
        
        self.modifyUnderCursorYValue();
        self.modifyOverCursorYValue();
        self.modifyOverCursorXValue();

        self.returnPointStack.delete(-2, -1);
    }
}

Vi*% Vi_initialize(Vi*% self) version 14
{
    var result = inherit(self);

    result.events.replace('.', lambda(Vi* self, int key) 
    {
        self.activeWin.autoInput = true;
        self.activeWin.pressedDot = true;
    });

    result.events.replace('m', lambda(Vi* self, int key) 
    {
        var key2 = self.activeWin.getKey(false);
        
        self.activeWin.markAtCurrentPoint(key2);
    });
    
    result.events.replace('O'-'A'+1, lambda(Vi* self, int key)
    {
        self.activeWin.returnBackOfStack();
    });

    result.events.replace('`', lambda(Vi* self, int key) 
    {
        var key2 = self.activeWin.getKey(false);
        
        if(key2 == '`') {
            self.activeWin.returnBack();
        }
        else {
            self.activeWin.returnAtMarkedPoint(key2);
        }
    });

    return result;
}
