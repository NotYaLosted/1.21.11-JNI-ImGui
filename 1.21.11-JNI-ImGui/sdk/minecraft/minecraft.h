#pragma once
#include "player.h"
#include "../../utils/java/java.h"
#include "../../utils/class/classloader.h"
#include "../../sdk/minecraft/mappings/mappings.h"
#include <iostream>

class c_minecraft : public c_jobject {
private:
    inline static bool instance_found = false;
    inline static bool player_found = false;

public:
    using c_jobject::c_jobject;

    static std::unique_ptr<c_minecraft> get() {
        jclass mc_class = sdk::classloader::find_class(g_java.env, sdk::mappings::minecraftclass_sig);
        if (!mc_class) {
            g_java.env->ExceptionClear();
            return nullptr;
        }

        jfieldID instance_fid = g_java.env->GetStaticFieldID(
            mc_class,
            sdk::mappings::minecraftclient_name,
            sdk::mappings::minecraftclient_sig
        );

        if (!instance_fid) {
            g_java.env->ExceptionClear();
            g_java.env->DeleteLocalRef(mc_class);
            return nullptr;
        }

        jobject mc = g_java.env->GetStaticObjectField(mc_class, instance_fid);
        g_java.env->DeleteLocalRef(mc_class);

        if (!mc) return nullptr;

        if (!instance_found) {
            std::cout << "[+] Got Minecraft instance\n";
            instance_found = true;
        }

        return std::make_unique<c_minecraft>(mc);
    }

    std::unique_ptr<c_player> get_local_player() {
        if (!this->is_valid()) return nullptr;

        jclass mc_class = sdk::classloader::find_class(g_java.env, sdk::mappings::minecraftclass_sig);
        if (!mc_class) return nullptr;

        jfieldID player_fid = g_java.env->GetFieldID(
            mc_class,
            sdk::mappings::player_name,
            sdk::mappings::player_sig
        );

        if (!player_fid) {
            g_java.env->ExceptionClear();
            g_java.env->DeleteLocalRef(mc_class);
            return nullptr;
        }

        jobject p = g_java.env->GetObjectField(this->raw(), player_fid);
        g_java.env->DeleteLocalRef(mc_class);

        if (!p) return nullptr;

        if (!player_found) {
            std::cout << "[+] Got LocalPlayer\n";
            player_found = true;
        }

        return std::make_unique<c_player>(p);
    }

    static void reset() {
        instance_found = false;
        player_found = false;
    }
};