#ifndef POSITION_H_
#define POSITION_H_

namespace Math
{
class Position
{
public:
    /**
     * Initialize at position 0.0,0.0 with orientation 0.0;
     */
    Position();

    /**
     * Initialize with location (xpos,ypos) and orientation 'orient'.
     */
    Position(const float& xpos, const float& ypos, const float& orient);

    /**
     * Return the location on the X axis.
     */
    const float& getX() const;

    /**
     * Return the location on the Y axis.
     */
    const float& getY() const;

    /**
     * Return the orientation of the object.
     */
    const float& getOrient() const;

    /**
     * Set the orientation of the object.
     */
    void setOrient(const float& orient);

    /**
     * Set the location on the X axis.
     */
    void setX(const float& xloc);

    /**
     * Set the location on the Y axis.
     */
    void setY(const float& yloc);

    /**
     * Generate a Position with a random X and Y location, and 0.0 orientation.
     */
    static Position* randomPosition(const float& worldSize, const float& numGrids, const int& gridid);

private:
    //Location on X axis.
    float m_Xpos;

    //Location on y axis.
    float m_Ypos;

    //Orientation.
    float m_Orient;
};
}

#endif
