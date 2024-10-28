#pragma once

struct inputField
{
    inputField(bool state = false) : _editing(state){}
    bool _editing;
};