#pragma once

template<uint32_t Bit>
bool has(const uint32_t& flags)
{
    return (flags & Bit) != 0;
}

template<uint32_t Bit>
void set(uint32_t& flags, bool v)
{
    if (v)
        flags |= Bit;
    else
        flags &= ~Bit;
}

template<uint32_t Bit>
void enable(uint32_t& flags)
{
    flags |= Bit;
}

template<uint32_t Bit>
void disable(uint32_t& flags)
{
    flags &= ~Bit;
}

template<uint32_t Bit>
void toggle(uint32_t& flags)
{
    flags ^= Bit;
}

struct FNodeState
{
    uint32_t flags = 0;

    enum : uint32_t
    {
        USE_VISIBLE = 1 << 0,
        USE_DEPTH_BIAS = 1 << 1,
        USE_MATERIAL = 1 << 2,
        USE_MATRIX = 1 << 3,
        VISIBLE = 1 << 4,
        SELECTED = 1 << 5,
    };

    FNodeState()
    {
        setVisible(true);
    }

    // --------------------------------------------------

    bool visible() const
    {
        return has<VISIBLE>(flags);
    }

    void setVisible(bool v)
    {
        set<VISIBLE>(flags, v);
    }

    bool useVisible() const
    {
        return has<USE_VISIBLE>(flags);
    }

    void setUseVisible(bool v)
    {
        set<USE_VISIBLE>(flags, v);
    }

    bool useDepthBias() const
    {
        return has<USE_DEPTH_BIAS>(flags);
    }

    void setUseDepthBias(bool v)
    {
        set<USE_DEPTH_BIAS>(flags, v);
    }

    bool useMaterial() const
    {
        return has<USE_MATERIAL>(flags);
    }

    void setUseMaterial(bool v)
    {
        set<USE_MATERIAL>(flags, v);
    }

    bool useMatrix() const
    {
        return has<USE_MATRIX>(flags);
    }

    void setUseMatrix(bool v)
    {
        set<USE_MATRIX>(flags, v);
    }
};
