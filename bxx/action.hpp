#pragma once

#include "id.hpp"

typedef struct bAction bl_action;
typedef struct FCurve bl_fcurve;
typedef struct BezTriple bl_keyframe;

namespace bxx
{
    class fcurve;

    class action : public id
    {
    public:
        action(bl_action* raw);
        fcurve find_fcurve(std::string const& name, int index = 0);
        fcurve add_fcurve(std::string const& group, std::string const& name, int index = 0);
        std::string get_name() const override;
        std::string get_full_name() const override;
        bl_action* get_raw();
        static bxx::action create(std::string const& name);
    private:
        bl_action* m_raw;
    };

    class keyframe
    {
    public:
        keyframe(bl_keyframe* raw);
        void set(float time, float value);
        void set_time(float time);
        void set_value(float value);
        float get_time();
        float get_value();
        bl_keyframe* get_raw();
    private:
        bl_keyframe* m_raw;
    };

    class fcurve : public id
    {
    public:
        fcurve(bxx::action const& parent, bl_fcurve* raw);
        void add_points(int amount);
        int num_points() const;
        keyframe get_point(int index);
        bl_fcurve* get_raw();
        std::string get_data_path() const;
        std::string get_name() const override;
        std::string get_full_name() const override;
        int get_index() const;
    private:
        bxx::action m_parent;
        bl_fcurve* m_raw;
    };
}