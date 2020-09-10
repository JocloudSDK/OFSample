#pragma once

namespace __yycomimp
{
#pragma pack(push, 8)
    template <bool _INTERLOCK = true> struct STD_REF;
    template <> struct STD_REF<false>
    {
        explicit STD_REF(ULONG x) : _ref(x) {}
        ULONG Inc() {return ++_ref;}
        ULONG Lock() {
            if(_ref) {
                return ++_ref;
            } else {
                return 0;
            }
        }
        ULONG Dec() {return --_ref;}
        private: LONG _ref;
        STD_REF();
        STD_REF(const STD_REF&);
        void operator()(const STD_REF&);
    };
    template <> struct STD_REF<true>
    {
        explicit STD_REF(ULONG x) : _ref(x) {}
        ULONG Inc() {return ::InterlockedIncrement((LONG*)&_ref);}
        ULONG Lock() {
            while(true) {
                long tmp = static_cast< long const volatile& >(_ref);
                if( tmp == 0 ) return 0;
                if( ::InterlockedCompareExchange( (volatile LONG*) &_ref, tmp + 1, tmp ) == tmp ) return tmp + 1;
            }
        }
        ULONG Dec() {return ::InterlockedDecrement((LONG*)&_ref);}
        private: ULONG _ref;
        STD_REF();
        STD_REF(const STD_REF&);
        void operator()(const STD_REF&);
    };
#pragma pack(pop)

    template <class T = void>
    struct STD_MALLOC
    {
        static void* malloc(size_t n) {return ::malloc(n);}
        static void free(void* p) {::free(p);}
    };

    //CComObjPtr±ØÐëµÄ¹¹Ôì²ÎÊý
    enum _enum_new_inst {new_inst};
} //namespace __yycomimp

using __yycomimp::STD_REF;
using __yycomimp::STD_MALLOC;

//指定默认CComObjPtr模板参数：CComObjPtr<T, INTERLOCK_REF=true, MALLOC=STD_MALLOC<T> >
#ifndef _DEF_COMOBJ_INTERLOCK_REF
	#define _DEF_COMOBJ_INTERLOCK_REF	true
#endif

#ifndef _DEF_COMOBJ_MALLOC
	#define _DEF_COMOBJ_MALLOC	STD_MALLOC
#endif

#define NOVTABLE    __declspec(novtable)

//////////////////////////////////////////////////////////////////////////
//»ù±¾ÐÞÊÎ·û
#ifndef interface
    #define interface   struct
#endif
