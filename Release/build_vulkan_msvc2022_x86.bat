mkdir build_vulkan_msvc2022_x86
cd build_vulkan_msvc2022_x86
cmake -G "Visual Studio 17 2022" -A Win32 -DCMAKE_INSTALL_PREFIX=../install_vulkan_msvc2022_x86 -DUSE_LLGI=ON -DBUILD_DX12=ON -DBUILD_VULKAN=ON .. -DCMAKE_DEBUG_POSTFIX=d
cmake --build . --config Debug --target INSTALL
cmake --build . --config Release --target INSTALL
