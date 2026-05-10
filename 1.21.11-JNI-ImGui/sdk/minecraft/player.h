#pragma once
#include "../../utils/java/java.h"
#include "../../utils/class/classloader.h"
#include "../../sdk/minecraft/mappings/mappings.h"
#include <iostream>

class c_player : public c_jobject {
private:
    bool last_sprint_state = false;

public:
    using c_jobject::c_jobject;

    void set_sprinting(bool state) { 
        if (!this->is_valid()) {
            return;
        }

        if (state == last_sprint_state) {
            return;
        }

        jclass entity_class = sdk::classloader::find_class(g_java.env, sdk::mappings::entity_class_sig);
        if (!entity_class) {
            g_java.env->ExceptionClear();
            return;
        }

        jmethodID set_sprinting_mid = g_java.env->GetMethodID(
            entity_class,
            sdk::mappings::set_sprinting_name,
            sdk::mappings::set_sprinting_sig
        );

        if (!set_sprinting_mid) {
            g_java.env->ExceptionClear();
            g_java.env->DeleteLocalRef(entity_class);
            return;
        }

        g_java.env->CallVoidMethod(this->raw(), set_sprinting_mid, (jboolean)state);

        if (g_java.env->ExceptionCheck()) {
            g_java.env->ExceptionDescribe();
            g_java.env->ExceptionClear();
        }
        else {
            last_sprint_state = state;
        }

        g_java.env->DeleteLocalRef(entity_class);
    }

    bool is_sprinting() { 
        if (!this->is_valid()) return false;

        jclass entity_class = sdk::classloader::find_class(g_java.env, sdk::mappings::entity_class_sig);
        if (!entity_class) {
            g_java.env->ExceptionClear();
            return false;
        }

        jmethodID is_sprinting_mid = g_java.env->GetMethodID(
            entity_class,
            sdk::mappings::is_sprinting_name,
            sdk::mappings::is_sprinting_sig
        );

        if (!is_sprinting_mid) {
            g_java.env->ExceptionClear();
            g_java.env->DeleteLocalRef(entity_class);
            return false;
        }

        jboolean result = g_java.env->CallBooleanMethod(this->raw(), is_sprinting_mid);
        g_java.env->DeleteLocalRef(entity_class);

        if (g_java.env->ExceptionCheck()) {
            g_java.env->ExceptionClear();
            return false;
        }

        return result == JNI_TRUE;
    }

    bool is_on_ground() {
        if (!this->is_valid()) return false;

        jclass entity_class = sdk::classloader::find_class(g_java.env, sdk::mappings::entity_class_sig);
        if (!entity_class) {
            g_java.env->ExceptionClear();
            return false;
        }

        jmethodID is_on_ground_mid = g_java.env->GetMethodID(
            entity_class,
            sdk::mappings::is_on_ground_name,
            sdk::mappings::is_on_ground_sig
        );

        if (!is_on_ground_mid) {
            g_java.env->ExceptionClear();
            g_java.env->DeleteLocalRef(entity_class);
            return false;
        }

        jboolean result = g_java.env->CallBooleanMethod(this->raw(), is_on_ground_mid);
        g_java.env->DeleteLocalRef(entity_class);

        if (g_java.env->ExceptionCheck()) {
            g_java.env->ExceptionClear();
            return false;
        }

        return result == JNI_TRUE;
    }
};