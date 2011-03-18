#ifndef VISIBLEOBJECT_H_
#define VISIBLEOBJECT_H_

namespace Game
{
/**
 * Objects within range and in FOV of other objects are 'VisibleObjects'.
 */
template <class T>
class VisibleObject
{
public:
    /*
     * Store the object in sight, the range of it, and its orientation
     */
    VisibleObject(T object, const float& range, const float& orient);

    /**
     * Get the orientation of the other object.
     */
    const float& getOrient() const;

    /**
     * Get the range of the other object.
     */
    const float& getRange() const;

    /**
     * Return the other object.
     */
    const T getObject() const;
private:
    /**
     * The visible object.
     */
    T m_Object;

    /**
     * The range of the object.
     */
    float m_Range;

    /**
     * The orientation of the object.
     */
    float m_Orientation;
};

template <class T> VisibleObject<T>::VisibleObject(T object, const float& range, const float& orient):m_Object(object), m_Range(range), m_Orientation(orient)
{
}

template <class T> const float& VisibleObject<T>::getOrient() const
{
    return m_Orientation;
}

template <class T> const float& VisibleObject<T>::getRange() const
{
    return m_Range;
}

template <class T> const T VisibleObject<T>::getObject() const
{
    return m_Object;
}
}
#endif
