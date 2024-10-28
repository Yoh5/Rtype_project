#pragma once

struct networkedObject
{
    networkedObject(int id, int local_id) : _net_id(id), _local_id(local_id){}
    int _net_id;
    int _local_id;
};
