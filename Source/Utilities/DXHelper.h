#ifndef DXHELPER_H
#define DXHELPER_H

#include <exception>
#include <cstdio>
#include <winerror.h>

// A COM Pointer Exception handler when something goes awry with any of the D3D11 Hooks.
namespace DX {
    // Helper class for COM exceptions
    class com_exception : public std::exception {
    public:
        com_exception(HRESULT hr) : result(hr) {
        }
        const char* what() const noexcept override {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X",
                      static_cast<unsigned int>(result));
            return s_str;
        }
    private:
        HRESULT result;
    };
    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr) {
        if (FAILED(hr)) {
            throw com_exception(hr);
        }
    }

    template<typename T>
    struct Release
    {
        void operator()(T* obj) const
        {
            if (obj)
            {
                obj->Release();
            }
        }
    };
}
#endif //DXHELPER_H
