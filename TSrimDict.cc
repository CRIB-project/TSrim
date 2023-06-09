// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TSrimDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "TSrim.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_TSrim(void *p = nullptr);
   static void *newArray_TSrim(Long_t size, void *p);
   static void delete_TSrim(void *p);
   static void deleteArray_TSrim(void *p);
   static void destruct_TSrim(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSrim*)
   {
      ::TSrim *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSrim >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TSrim", ::TSrim::Class_Version(), "TSrim.h", 3,
                  typeid(::TSrim), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSrim::Dictionary, isa_proxy, 4,
                  sizeof(::TSrim) );
      instance.SetNew(&new_TSrim);
      instance.SetNewArray(&newArray_TSrim);
      instance.SetDelete(&delete_TSrim);
      instance.SetDeleteArray(&deleteArray_TSrim);
      instance.SetDestructor(&destruct_TSrim);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSrim*)
   {
      return GenerateInitInstanceLocal((::TSrim*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSrim*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TSrim::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TSrim::Class_Name()
{
   return "TSrim";
}

//______________________________________________________________________________
const char *TSrim::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSrim*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TSrim::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSrim*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSrim::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSrim*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSrim::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSrim*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TSrim::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSrim.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSrim::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSrim::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSrim(void *p) {
      return  p ? new(p) ::TSrim : new ::TSrim;
   }
   static void *newArray_TSrim(Long_t nElements, void *p) {
      return p ? new(p) ::TSrim[nElements] : new ::TSrim[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSrim(void *p) {
      delete ((::TSrim*)p);
   }
   static void deleteArray_TSrim(void *p) {
      delete [] ((::TSrim*)p);
   }
   static void destruct_TSrim(void *p) {
      typedef ::TSrim current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSrim

namespace ROOT {
   static TClass *vectorlETF1gR_Dictionary();
   static void vectorlETF1gR_TClassManip(TClass*);
   static void *new_vectorlETF1gR(void *p = nullptr);
   static void *newArray_vectorlETF1gR(Long_t size, void *p);
   static void delete_vectorlETF1gR(void *p);
   static void deleteArray_vectorlETF1gR(void *p);
   static void destruct_vectorlETF1gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TF1>*)
   {
      vector<TF1> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TF1>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TF1>", -2, "vector", 493,
                  typeid(vector<TF1>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETF1gR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TF1>) );
      instance.SetNew(&new_vectorlETF1gR);
      instance.SetNewArray(&newArray_vectorlETF1gR);
      instance.SetDelete(&delete_vectorlETF1gR);
      instance.SetDeleteArray(&deleteArray_vectorlETF1gR);
      instance.SetDestructor(&destruct_vectorlETF1gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TF1> >()));

      ::ROOT::AddClassAlternate("vector<TF1>","std::__1::vector<TF1, std::__1::allocator<TF1> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TF1>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETF1gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TF1>*)nullptr)->GetClass();
      vectorlETF1gR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETF1gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETF1gR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TF1> : new vector<TF1>;
   }
   static void *newArray_vectorlETF1gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TF1>[nElements] : new vector<TF1>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETF1gR(void *p) {
      delete ((vector<TF1>*)p);
   }
   static void deleteArray_vectorlETF1gR(void *p) {
      delete [] ((vector<TF1>*)p);
   }
   static void destruct_vectorlETF1gR(void *p) {
      typedef vector<TF1> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TF1>

namespace {
  void TriggerDictionaryInitialization_TSrimDict_Impl() {
    static const char* headers[] = {
"TSrim.h",
nullptr
    };
    static const char* includePaths[] = {
"/opt/homebrew/Cellar/root/6.26.06_2/include/root",
"/Users/hayakawa/Dropbox/hayakawa/calc/srim/SRIM-2013/root_class/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TSrimDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$TSrim.h")))  TSrim;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TSrimDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "TSrim.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TSrim", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TSrimDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TSrimDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TSrimDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TSrimDict() {
  TriggerDictionaryInitialization_TSrimDict_Impl();
}
