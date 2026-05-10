#pragma once
#include "../../utils/java/java.h"
#include "../../utils/class/classloader.h"
#include "../../sdk/minecraft/mappings/mappings.h"

class c_box : public c_jobject
{
public:
    using c_jobject::c_jobject;

    double get_min_x()
    {
        if (!this->is_valid())
            return 0.0;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return 0.0;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_min_x_name, sdk::mappings::box_min_x_sig);
        if (!fid)
        {
            g_java.env->ExceptionClear();
            g_java.env->DeleteLocalRef(box_class);
            return 0.0;
        }

        jdouble result = g_java.env->GetDoubleField(this->raw(), fid);
        g_java.env->DeleteLocalRef(box_class);
        return result;
    }

    double get_max_x()
    {
        if (!this->is_valid())
            return 0.0;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return 0.0;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_max_x_name, sdk::mappings::box_max_x_sig);
        if (!fid)
        {
            g_java.env->ExceptionClear();
            g_java.env->DeleteLocalRef(box_class);
            return 0.0;
        }

        jdouble result = g_java.env->GetDoubleField(this->raw(), fid);
        g_java.env->DeleteLocalRef(box_class);
        return result;
    }

    double get_min_y()
    {
        if (!this->is_valid())
            return 0.0;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return 0.0;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_min_y_name, sdk::mappings::box_min_y_sig);
        if (!fid)
        {
            g_java.env->ExceptionClear();
            g_java.env->DeleteLocalRef(box_class);
            return 0.0;
        }

        jdouble result = g_java.env->GetDoubleField(this->raw(), fid);
        g_java.env->DeleteLocalRef(box_class);
        return result;
    }

    double get_max_y()
    {
        if (!this->is_valid())
            return 0.0;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return 0.0;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_max_y_name, sdk::mappings::box_max_y_sig);
        if (!fid)
        {
            g_java.env->ExceptionClear();
            g_java.env->DeleteLocalRef(box_class);
            return 0.0;
        }

        jdouble result = g_java.env->GetDoubleField(this->raw(), fid);
        g_java.env->DeleteLocalRef(box_class);
        return result;
    }

    double get_min_z()
    {
        if (!this->is_valid())
            return 0.0;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return 0.0;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_min_z_name, sdk::mappings::box_min_z_sig);
        if (!fid)
        {
            g_java.env->ExceptionClear();
            g_java.env->DeleteLocalRef(box_class);
            return 0.0;
        }

        jdouble result = g_java.env->GetDoubleField(this->raw(), fid);
        g_java.env->DeleteLocalRef(box_class);
        return result;
    }

    double get_max_z()
    {
        if (!this->is_valid())
            return 0.0;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return 0.0;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_max_z_name, sdk::mappings::box_max_z_sig);
        if (!fid)
        {
            g_java.env->ExceptionClear();
            g_java.env->DeleteLocalRef(box_class);
            return 0.0;
        }

        jdouble result = g_java.env->GetDoubleField(this->raw(), fid);
        g_java.env->DeleteLocalRef(box_class);
        return result;
    }

    void set_min_x(double value)
    {
        if (!this->is_valid())
            return;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_min_x_name, sdk::mappings::box_min_x_sig);
        if (fid)
        {
            g_java.env->SetDoubleField(this->raw(), fid, value);
        }
        else
        {
            g_java.env->ExceptionClear();
        }

        g_java.env->DeleteLocalRef(box_class);
    }

    void set_max_x(double value)
    {
        if (!this->is_valid())
            return;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_max_x_name, sdk::mappings::box_max_x_sig);
        if (fid)
        {
            g_java.env->SetDoubleField(this->raw(), fid, value);
        }
        else
        {
            g_java.env->ExceptionClear();
        }

        g_java.env->DeleteLocalRef(box_class);
    }

    void set_min_y(double value)
    {
        if (!this->is_valid())
            return;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_min_y_name, sdk::mappings::box_min_y_sig);
        if (fid)
        {
            g_java.env->SetDoubleField(this->raw(), fid, value);
        }
        else
        {
            g_java.env->ExceptionClear();
        }

        g_java.env->DeleteLocalRef(box_class);
    }

    void set_max_y(double value)
    {
        if (!this->is_valid())
            return;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_max_y_name, sdk::mappings::box_max_y_sig);
        if (fid)
        {
            g_java.env->SetDoubleField(this->raw(), fid, value);
        }
        else
        {
            g_java.env->ExceptionClear();
        }

        g_java.env->DeleteLocalRef(box_class);
    }

    void set_min_z(double value)
    {
        if (!this->is_valid())
            return;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_min_z_name, sdk::mappings::box_min_z_sig);
        if (fid)
        {
            g_java.env->SetDoubleField(this->raw(), fid, value);
        }
        else
        {
            g_java.env->ExceptionClear();
        }

        g_java.env->DeleteLocalRef(box_class);
    }

    void set_max_z(double value)
    {
        if (!this->is_valid())
            return;

        jclass box_class = sdk::classloader::find_class(g_java.env, "net/minecraft/class_238");
        if (!box_class)
        {
            g_java.env->ExceptionClear();
            return;
        }

        jfieldID fid = g_java.env->GetFieldID(box_class, sdk::mappings::box_max_z_name, sdk::mappings::box_max_z_sig);
        if (fid)
        {
            g_java.env->SetDoubleField(this->raw(), fid, value);
        }
        else
        {
            g_java.env->ExceptionClear();
        }

        g_java.env->DeleteLocalRef(box_class);
    }
};