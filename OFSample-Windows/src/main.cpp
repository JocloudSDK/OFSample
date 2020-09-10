// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include "OrangeFilterUI.h"

int main()
{
    // Setup window
    OrangeFilterUI orangeUI;
    if (orangeUI.OrangeFilterUICreate(1280, 700))
    {
        orangeUI.RendererRun();
    }
    return 0;
}

