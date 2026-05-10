#pragma once
#include <jni.h>

namespace sdk {
    namespace classloader {

        bool init(JNIEnv* env);


        jclass find_class(JNIEnv* env, const char* class_name);


        bool is_fabric();


        void cleanup(JNIEnv* env);
    }
}