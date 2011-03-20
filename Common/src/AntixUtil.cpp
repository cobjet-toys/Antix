#include "AntixUtil.h"

namespace Antix
{

//returns a value and ensures the first bit is always 0 before returning it
unsigned int readId(unsigned int id)
{
    //only works on systems where int size = 32 bits
    return ((id > 2147483647U) ? (id ^ 2147483648U) :  id);
}

//return 1 if first bit is 1, 0 otherwise
unsigned int getType(unsigned int id)
{
    return ((id > 2147483647U) ? 1 : 0); 
}

//returns id, ensuring the first bit is equal to type
unsigned int writeId(unsigned int id, unsigned short int type)
{
    if(getType(id) == type)
    {
        //if its already the type you want to set it to dont change it
        return id;         
    }
    else
    {    
        return (id ^ 2147483648U); //flip it
        
        //this will always make a 1 a 0, and a 0 a 1 for the 32nd bit.
        //and since it only changes it when its different, it doesnt need
        //to know whether its flipping a 1 or 0 for it to be correct.
    }
}
    

}
