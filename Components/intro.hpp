#pragma once

struct introComponent
{
    introComponent(int last) : _last_frame(last), ended(false){}
    int _last_frame;
    bool ended;
};