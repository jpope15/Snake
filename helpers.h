#ifndef HELPERS_H
#define HELPERS_H

class Helpers {
public: 
    static int roundUp(int numToRound, int multiple){
        if (multiple == 0)
            return numToRound;

        int remainder = numToRound % multiple;
        if (remainder == 0)
            return numToRound;

        return numToRound + multiple - remainder;
    }

};

#endif