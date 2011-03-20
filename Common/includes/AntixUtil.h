namespace Antix
{

//returns a value and ensures the first bit is always 0 before returning it
unsigned int readId(unsigned int id);

//return 1 if first bit is 1, 0 otherwise
unsigned int getType(unsigned int id);

//returns id, ensuring the first bit is equal to type
unsigned int writeId(unsigned int id, unsigned short int type);

}
