@echo off
echo 测试修复后的程序行为
echo =====================
echo.

echo 测试1: 正常登录流程
echo 1 | SocialPlatform.exe
echo.

echo 测试2: 输入非数字选项
(
    echo abc
    echo 1
    echo QQ002
    echo 002002
    echo 0
    echo 0
) | SocialPlatform.exe
echo.

echo 测试完成