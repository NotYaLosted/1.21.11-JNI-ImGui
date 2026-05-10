#pragma once
#include <jni.h>
#include <memory>
#include <string>

class c_jni {
private:
    JavaVM* jvm;
public:
    JNIEnv* env;

    bool init() {
        jsize count;
        if (JNI_GetCreatedJavaVMs(&jvm, 1, &count) != JNI_OK || count == 0)
            return false;

        jint res = jvm->GetEnv((void**)&env, JNI_VERSION_21);

        if (res == JNI_EDETACHED) {
            if (jvm->AttachCurrentThread((void**)&env, nullptr) != JNI_OK)
                return false;
        }

        return true;
    }

    void detach() {
        if (jvm) {
            jvm->DetachCurrentThread();
        }
    }
};

inline c_jni g_java;

class c_jobject {
protected:
    struct deleter {
        void operator()(_jobject* obj) const {
            if (obj && g_java.env) {
                g_java.env->DeleteGlobalRef(obj);
            }
        }
    };

    std::unique_ptr<_jobject, deleter> instance;

public:
    c_jobject() = default;

    c_jobject(jobject obj) {
        if (obj && g_java.env) {
            jobject globalRef = g_java.env->NewGlobalRef(obj);
            if (globalRef) {
                instance.reset(globalRef);
            }
            g_java.env->DeleteLocalRef(obj);
        }
    }

    jobject raw() const { return instance.get(); }
    bool is_valid() const { return instance != nullptr; }
};