#pragma once
class nocopy
{
public:
    nocopy()
    {}
    ~nocopy(){}
    nocopy(const nocopy&) = delete;
    const nocopy& operator=(const nocopy&) = delete;
};