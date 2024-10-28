#pragma once

struct room
{
    room(int id) : _id(id), _selected(false){}
    int _id;
    bool _selected;
};
