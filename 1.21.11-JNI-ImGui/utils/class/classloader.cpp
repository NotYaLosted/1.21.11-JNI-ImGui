#include "classloader.h"
#include <cstring>
#include <string>
#include <algorithm>

namespace sdk {
    namespace classloader {
        static jobject classloader_obj = nullptr;
        static jmethodID findclass_md = nullptr;
        static bool fabric_detected = false;
        static bool initialized = false;

        static bool findClsLoaderByThreads(JNIEnv* env, const char* cls_loader_name) {
            jclass threadCls = env->FindClass("java/lang/Thread");
            if (!threadCls) return false;

            jmethodID allStackTracesMd = env->GetStaticMethodID(threadCls, "getAllStackTraces", "()Ljava/util/Map;");
            if (!allStackTracesMd) {
                env->DeleteLocalRef(threadCls);
                return false;
            }

            jobject threadMap = env->CallStaticObjectMethod(threadCls, allStackTracesMd);
            if (!threadMap) {
                env->DeleteLocalRef(threadCls);
                return false;
            }

            jclass mapCls = env->FindClass("java/util/Map");
            jmethodID entrySetMd = env->GetMethodID(mapCls, "entrySet", "()Ljava/util/Set;");
            jobject entrySet = env->CallObjectMethod(threadMap, entrySetMd);

            jclass setCls = env->FindClass("java/util/Set");
            jmethodID iteratorMd = env->GetMethodID(setCls, "iterator", "()Ljava/util/Iterator;");
            jobject it = env->CallObjectMethod(entrySet, iteratorMd);

            jclass itCls = env->FindClass("java/util/Iterator");
            jmethodID hasNextMd = env->GetMethodID(itCls, "hasNext", "()Z");
            jmethodID nextMd = env->GetMethodID(itCls, "next", "()Ljava/lang/Object;");

            jclass entryCls = env->FindClass("java/util/Map$Entry");
            jmethodID getKeyMd = env->GetMethodID(entryCls, "getKey", "()Ljava/lang/Object;");

            jmethodID getContextMd = env->GetMethodID(threadCls, "getContextClassLoader", "()Ljava/lang/ClassLoader;");

            jclass classCls = env->FindClass("java/lang/Class");
            jmethodID getNameMd = env->GetMethodID(classCls, "getName", "()Ljava/lang/String;");

            while (env->CallBooleanMethod(it, hasNextMd)) {
                jobject entry = env->CallObjectMethod(it, nextMd);
                jobject threadObj = env->CallObjectMethod(entry, getKeyMd);
                jobject loader = env->CallObjectMethod(threadObj, getContextMd);

                if (!loader) {
                    env->DeleteLocalRef(entry);
                    env->DeleteLocalRef(threadObj);
                    continue;
                }

                jclass loaderCls = env->GetObjectClass(loader);
                jstring nameStr = (jstring)env->CallObjectMethod(loaderCls, getNameMd);
                const char* nameC = env->GetStringUTFChars(nameStr, nullptr);

                if (nameC && strcmp(nameC, cls_loader_name) == 0) {
                    classloader_obj = env->NewGlobalRef(loader);
                    env->ReleaseStringUTFChars(nameStr, nameC);
                    env->DeleteLocalRef(nameStr);
                    env->DeleteLocalRef(loaderCls);
                    env->DeleteLocalRef(loader);
                    env->DeleteLocalRef(entry);
                    env->DeleteLocalRef(threadObj);


                    env->DeleteLocalRef(classCls);
                    env->DeleteLocalRef(entryCls);
                    env->DeleteLocalRef(itCls);
                    env->DeleteLocalRef(it);
                    env->DeleteLocalRef(setCls);
                    env->DeleteLocalRef(entrySet);
                    env->DeleteLocalRef(mapCls);
                    env->DeleteLocalRef(threadMap);
                    env->DeleteLocalRef(threadCls);
                    return true;
                }

                env->ReleaseStringUTFChars(nameStr, nameC);
                env->DeleteLocalRef(nameStr);
                env->DeleteLocalRef(loaderCls);
                env->DeleteLocalRef(loader);
                env->DeleteLocalRef(entry);
                env->DeleteLocalRef(threadObj);
            }

            env->DeleteLocalRef(classCls);
            env->DeleteLocalRef(entryCls);
            env->DeleteLocalRef(itCls);
            env->DeleteLocalRef(it);
            env->DeleteLocalRef(setCls);
            env->DeleteLocalRef(entrySet);
            env->DeleteLocalRef(mapCls);
            env->DeleteLocalRef(threadMap);
            env->DeleteLocalRef(threadCls);
            return false;
        }

        bool init(JNIEnv* env) {
            if (!env) return false;
            if (initialized) return true;

            if (findClsLoaderByThreads(env, "net.fabricmc.loader.impl.launch.knot.KnotClassLoader")) {
                fabric_detected = true;
            }
            else if (findClsLoaderByThreads(env, "cpw.mods.modlauncher.TransformingClassLoader")) {
                fabric_detected = false;
            }
            else if (findClsLoaderByThreads(env, "net.minecraft.launchwrapper.LaunchClassLoader")) {
                fabric_detected = false;
            }
            else {
                fabric_detected = false;
            }

            if (classloader_obj) {
                jclass knotCls = env->GetObjectClass(classloader_obj);
                findclass_md = env->GetMethodID(knotCls, "loadClass", "(Ljava/lang/String;Z)Ljava/lang/Class;");
                env->DeleteLocalRef(knotCls);
            }

            initialized = true;
            return true;
        }

        bool is_fabric() {
            return fabric_detected;
        }

        jclass find_class(JNIEnv* env, const char* class_name) {
            if (!env || !class_name) return nullptr;

            if (!initialized) {
                init(env);
            }

            if (classloader_obj && findclass_md) {
                std::string class_name_format(class_name);
                std::replace(class_name_format.begin(), class_name_format.end(), '/', '.');

                jstring jname = env->NewStringUTF(class_name_format.c_str());
                if (!jname) return nullptr;

                jclass cls = reinterpret_cast<jclass>(env->CallObjectMethod(classloader_obj, findclass_md, jname));
                env->DeleteLocalRef(jname);

                if (env->ExceptionCheck()) {
                    env->ExceptionClear();
                    return nullptr;
                }

                return cls;
            }

            return env->FindClass(class_name);
        }

        void cleanup(JNIEnv* env) {
            if (env && classloader_obj) {
                env->DeleteGlobalRef(classloader_obj);
                classloader_obj = nullptr;
            }
            findclass_md = nullptr;
            fabric_detected = false;
            initialized = false;
        }
    }
}