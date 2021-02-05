//
// Created by kb on 05/02/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_MYSTACKWALKER_H
#define SIMPLERENDERENGINEPROJECT_MYSTACKWALKER_H
#include "../StackWalker.h"
class MyStackWalker : public StackWalker {
public:
    MyStackWalker() : StackWalker() {};

protected:
    virtual void OnOutput(LPCSTR szText) {
        printf(szText);
        StackWalker::OnOutput(szText);
    }
};
#endif //SIMPLERENDERENGINEPROJECT_MYSTACKWALKER_H
