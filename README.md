# Oculus World Demo 

This is OculusWorldDemo with CMake. (OculusWorldDemo is included in Oculus SDK). Enable Win32+GL. Based on Oculus SDK 0.2.4.

## Build
* If you want enable Win32 with GL, edit CMakeLists.txt. 1=Enable/0=Disable
```
# enable win32+GL
#ADD_DEFINITIONS( -DOVR_WIN32_GL=1 )
ADD_DEFINITIONS( -DOVR_WIN32_GL=0 )
```

* Copy ```Assets``` from Oculus SDK. (Assets are too large to include repository)
