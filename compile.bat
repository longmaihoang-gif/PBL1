@echo off
echo ====================================================
echo   Dang tien hanh bien dich...
echo ====================================================

:: Kiem tra g++ co ton tai trong PATH hay khong
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo Khong tim thay trinh bien dich 'g++' trong bien moi truong (PATH).
    echo Vui long cai dat MinGW/GCC hoac them duong dan g++ vao bien moi truong cua he dieu hanh!
    pause
    exit /b 1
)

echo Dang bien dich ma nguon PBL1_UI voi thu vien ImGui va GLFW...
g++ -std=c++11 -g -finput-charset=utf-8 -fexec-charset=utf-8 ^
    PBL1_UI.cpp ^
    icon.o ^
    imgui-master/imgui-master/imgui.cpp ^
    imgui-master/imgui-master/imgui_draw.cpp ^
    imgui-master/imgui-master/imgui_tables.cpp ^
    imgui-master/imgui-master/imgui_widgets.cpp ^
    imgui-master/imgui-master/backends/imgui_impl_glfw.cpp ^
    imgui-master/imgui-master/backends/imgui_impl_opengl3.cpp ^
    -I imgui-master/imgui-master ^
    -I glfw-3.4.bin.WIN64/include ^
    -L glfw-3.4.bin.WIN64/lib-mingw-w64 ^
    -lglfw3 -lopengl32 -lgdi32 ^
    -o PBL1_UI.exe

if %errorlevel% equ 0 (
    echo ====================================================
    echo   Bien dich THANH CONG!
    echo ====================================================
) else (
    echo ====================================================
    echo   Bien dich THAT BAI! Hay kiem tra loi o tren.
    echo ====================================================
)

pause
