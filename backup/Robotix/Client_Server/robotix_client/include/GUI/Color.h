#ifndef COLOR_H_
#define COLOR_H_

namespace GUI
{
/**
 * Color used by the drawer to differentiate teams.
 */
class Color
{
public:
    /**
     * Ctor. 
     */
    Color(const float& r, const float& g,const float& b);
    
    /**
     * Returns a color with random RGB values.
     */
    static Color* getRandomColor(); 

    /*
     * Return the Red value.
     */
    const float& getR() const;

    /**
     * Return the Green value.
     */
    const float& getG() const;

    /**
     * Return the Blue value.
     */
    const float& getB() const;
private:
    float m_R;
    float m_G;
    float m_B;
};
}
#endif
